#include "stm32f4xx.h"
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
u8 Judge_over_flow_Task = RESET;

int main(void)
{
	Motor_t motor1,motor2,motor3,motor4,motor5,motor6;
	u16 delay_task = 0;

	SysTick_Init();
	USART_Config();
	TIMER_Init();
	KEY_Init();	
	MOTOR_TIM_Init();
	ENCODER_TIM_Init();
	
	//电机初始化
	MOTOR_Init(&motor1,&motor2,&motor2,&motor4,&motor5,&motor6);	
    
	//经测试，电机最大转速45编码器值/5ms
	motor1.pid_speed.target=-30;
  	while(1)
    {
		if(delay_task ==600)      //非阻塞延时3s
		{
			motor1.pid_speed.target = -motor1.pid_speed.target;            //速度反相
		    delay_task = 0;
		}
		
		//统计编码器溢出值任务.设立这个标志位的原因:
		//1.尽可能减少中断服务函数的代码 2.尽量避免使用全局变量而使用指针传递  by zhp
		if(Judge_over_flow_Task)              
		{
			if(TIM_IS_TIM_COUNTING_DOWN(TIM4))     //向下计数
				motor1.encoder_overflow_count --;
			else
				motor1.encoder_overflow_count ++;  //向上计数
			Judge_over_flow_Task = RESET;
		}
		

		if(Speed_Pid_cal_Task == TASK_ENABLE)
		{
			Speed_Pid_cal_Task = 5;              
			delay_task++;
			MOTOR_Speed_Task(&motor1,&motor2,&motor2,&motor4,&motor5,&motor6);
			
			ANODT_Send_F1(motor1.pid_speed.target, motor1.pid_speed.measure);
//			printf("%d  %d  %d  %d %.2f %.2f \r\n",motor1.pid_speed.target, motor1.pid_speed.measure,
//			motor1.angle_accmul,motor1.angle, motor1.pid_speed.out, motor1.pwm_out);
		}
		
		
	}
}

