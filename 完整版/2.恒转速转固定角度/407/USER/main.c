#include "sys.h"
#include "stm32f4xx.h"
#include "motor.h"
#include "encoder.h"
#include "usart.h"
#include "systick.h"
#include "stdlib.h"
#include "tim.h"
#include "led.h"   


u16 Speed_Pid_cal_Task = 5;           //pid计算周期5ms

enum Task_flag_t
{
	speed_pid_Task,     //速度环任务
	angle_pid_Task	    //角度环任务
};



void Judge_task(Motor_t* motor, u8* flag, enum Task_flag_t* Task_flag)
{
	//在距离目标值较远时,进行速度环任务,使电机恒速转动;接近目标值时,转变为角度环稳定在目标值;将这个接近值设为50
	if(abs(motor->pid_angle.target - motor->angle_accmul) > 25)       
	{
		if(*flag == 0)
			*Task_flag = speed_pid_Task;
	}
	else 
	{
		//角度环有可能超调,当电机超过目标值50后就会变回速度环,然后一直无法到达目标值;所以设置flag,使电机转变为角度环之后不会再变回速度环
		*Task_flag = angle_pid_Task;
		*flag = 1;
	}	
}

//28 200   3 25
#define MOTOR motor2

//经测试，电机最大转速为35编码器值/5ms
int main(void)
{
	Motor_t motor1,motor2,motor3,motor4,motor5,motor6;
	u16 delay_task = 0;
	u8 flag = 0;

	short Set_PWM_Value[6]={0};
	short Send_F401[8]={'P',0,0,0,0,0,0,'E'};
	
	enum Task_flag_t Task_flag;
	
	SysTick_Init();
	USART_Config();
	BLUETOOTH_Config();
	TIMER_Init();
	MOTOR_TIM_Init();
	ENCODER_TIM_Init();
	
	//电机初始化
	MOTOR_Init(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);	    //zhp:电机2编码器一相坏了,反转有问题
    
	MOTOR.pid_angle.target=640;

	while(1)
    {
		if(Speed_Pid_cal_Task == TASK_ENABLE)
		{
			Speed_Pid_cal_Task = 5;              
			Judge_task(&MOTOR,&flag,&Task_flag);          //这里随便选一个电机即可
			
			if(Task_flag == speed_pid_Task)
			{
				
				MOTOR.pid_speed.target = 3;
					
				MOTOR_Speed_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
			}
			else
			    MOTOR_Angle_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
			
	
			Set_Pwm(Set_PWM_Value,Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
		
//			ANODT_Send_F1(motor4.pid_speed.target, motor4.pid_speed.measure);
//			ANODT_Send_F1(motor4.pid_angle.target, motor4.pid_angle.measure);
			

		}	
	}
}





		
//		if(delay_task ==500)      //非阻塞延时3s
//		{
//			motor4.pid_speed.target = -motor4.pid_speed.target;            //速度反相
//			delay_task = 0;
//		}
//		
//		if(Speed_Pid_cal_Task == TASK_ENABLE)
//		{
//			Speed_Pid_cal_Task = 5;              
//			delay_task++;
//			MOTOR_Speed_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//								
//			Set_Pwm(Set_PWM_Value,Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//			
//			
//			ANODT_Send_F1(motor4.pid_speed.target, motor4.pid_speed.measure);
//			/*记得修改fputc*/
////			printf(" %d  %d  %.2f  %.2f \r\n",
////			motor2.pid_speed.target,motor2.pid_speed.measure,motor2.pid_speed.out, motor2.pwm_out);
//		}
		
		
		
//		if(delay_task ==500)      //非阻塞延时3s
//		{
//			motor4.pid_angle.target += 5600;            
//			delay_task = 0;
//		}
//		
//		
//		if(Speed_Pid_cal_Task == TASK_ENABLE)
//		{
//			Speed_Pid_cal_Task = 5;              
//			delay_task++;
//						
//			MOTOR_Angle_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//			
//			Set_Pwm(Set_PWM_Value,Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//			
//			ANODT_Send_F1(motor4.pid_angle.target, motor4.pid_angle.measure);
//			/*记得修改fputc*/
////			printf("%d  %.2f %d  %d  %.2f  %.2f \r\n",motor4.pid_angle.measure,motor4.pid_angle.out,
////			motor4.pid_speed.target,motor4.pid_speed.measure,motor4.pid_speed.out, motor4.pwm_out);
//		}



