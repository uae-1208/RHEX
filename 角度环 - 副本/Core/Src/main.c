#include "stm32f4xx.h"
#include "stdlib.h"
#include "motor.h"
#include "encoder.h"
#include "usart.h"
#include "key.h"
#include "systick.h"
#include "stm32f4xx.h"
#include "tim.h"
#include "led.h"   

//判断此时CNT是否向下计数  从HAL库顺来的函数
#define TIM_IS_TIM_COUNTING_DOWN(TIM)    ((TIM->CR1 &(TIM_CR1_DIR)) == (TIM_CR1_DIR))

u16 Speed_Pid_cal_Task = 5;       //pid计算周期5ms
u16 Angle_Pid_cal_Task = 5;       //pid计算周期5ms
u8 Judge_over_flow_Task = RESET;

enum Task_flag_t
{
	speed_pid_Task,     //速度环任务
	angle_pid_Task	    //角度环任务
};


void Judge_task(Motor_t* motor, u8* flag, enum Task_flag_t* Task_flag)
{
	//电机从0转到1300,前1250以恒定速度转动,最后稳定在1300,所以给50的缓冲区进行角度控制
	if(abs(motor->pid_angle.target - motor->angle_accmul) < 50)
		*Task_flag = angle_pid_Task;
	else 
	{
		//当角度环超调,超出1350时不应该为速度环控制,而是角度环回调到1300,设置flag是为了使速度环转换成角度环后不会再变回速度环
		if(*flag == 0)
		{
			*Task_flag = speed_pid_Task;
			*flag = 1;
		}
	}	
}

int main(void)
{
	Motor_t motor1,motor2,motor3,motor4,motor5,motor6;
	u16 delay_task = 0;
    enum Task_flag_t Task_flag;
	u8 flag=0;
	
	SysTick_Init();
	USART_Config();
	TIMER_Init();
	KEY_Init();	
	MOTOR_TIM_Init();
	ENCODER_TIM_Init();
	
	//电机初始化
	MOTOR_Init(&motor1,&motor2,&motor2,&motor4,&motor5,&motor6);	
   	motor1.pid_angle.target = 1300;

	while(1)
    {
//		if(delay_task ==600)      //非阻塞延时3s:5ms*600
//		{
//			motor1.pid_speed.target = -motor1.pid_speed.target;            //速度反向
//		    delay_task = 0;
//						printf("%d\r\n",motor1.pid_speed.target);

//		}
		
		
		
		//统计编码器溢出值任务.设立这个标志位的原因:
		//1.尽可能减少中断服务函数的代码 2.尽量减少全局变量而使用指针传递  by zhp
		if(Judge_over_flow_Task)              
		{
			if(TIM_IS_TIM_COUNTING_DOWN(TIM4))     //向下计数
				motor1.encoder_overflow_count --;
			else
				motor1.encoder_overflow_count ++;  //向上计数
			
			Judge_over_flow_Task = RESET;
		}
		  


		
		Judge_task(&motor1, &flag, &Task_flag);
		
		if(Task_flag == speed_pid_Task)          //应当进行速度环任务
		{
			motor1.pid_speed.target = 40;
			if(Speed_Pid_cal_Task == TASK_ENABLE)
			{
				Speed_Pid_cal_Task = 5;              
				Angle_Pid_cal_Task = 5;              
//				delay_task++;
				
				MOTOR_Speed_Task(&motor1,&motor2,&motor2,&motor4,&motor5,&motor6);
				
//			    ANODT_Send_F1(motor1.pid_speed.target, motor1.pid_speed.measure);
				printf("%d  %d  %d  %d %.2f %.2f \r\n",motor1.pid_speed.target, motor1.pid_speed.measure,
				motor1.angle_accmul,motor1.angle, motor1.pid_speed.out, motor1.pwm_out);
			}
		}
		else      //角度环任务
		{
			if(Angle_Pid_cal_Task == TASK_ENABLE)
			{
				Speed_Pid_cal_Task = 5;              
				Angle_Pid_cal_Task = 5;              
//				delay_task++;
				
				MOTOR_Angle_Task(&motor1,&motor2,&motor2,&motor4,&motor5,&motor6);
				
//			    ANODT_Send_F1(motor1.pid_angle.target, motor1.pid_angle.measure);
				printf("%d  %d  %.2f   %d  %.2f %.2f \r\n",motor1.pid_angle.target,  motor1.pid_angle.measure,
				motor1.pid_angle.out, motor1.pid_speed.measure, motor1.pid_speed.out,  motor1.pwm_out);
			}
		}
		
		

	}
}








