#include "sys.h"
#include "stm32f4xx.h"
#include "motor.h"
#include "encoder.h"
#include "usart.h"
#include "systick.h"
#include "stm32f4xx.h"
#include "tim.h"
#include "led.h"   

u16 Speed_Pid_cal_Task = 5;       //pid计算周期5ms

#define MOTOR motor2

int main(void)
{
	Motor_t motor1,motor2,motor3,motor4,motor5,motor6;
	u16 delay_task = 0;

	short Set_PWM_Value[6]={0};
	short Send_F401[8]={'P',0,0,0,0,0,0,'E'};
	
	SysTick_Init();
	USART_Config();
	BLUETOOTH_Config();
	TIMER_Init();
	MOTOR_TIM_Init();
	ENCODER_TIM_Init();
	
	//电机初始化
	MOTOR_Init(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);	  
    
	//经测试，电机最大转速为35编码器值/5ms
//	MOTOR.pid_speed.target = 30;
	MOTOR.pid_angle.target=2000;

	while(1)
    {
		if(delay_task ==500)      //非阻塞延时3s
		{
			MOTOR.pid_speed.target = -MOTOR.pid_speed.target;            //速度反相
			delay_task = 0;
		}
		
		if(Speed_Pid_cal_Task == TASK_ENABLE)
		{
			Speed_Pid_cal_Task = 5;              
			delay_task++;
			MOTOR_Speed_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
								
			Set_Pwm(Set_PWM_Value,Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
			
			printf(" %d  %d  %d  %d  %d  %d\r\n",motor1.angle_accmul,motor2.angle_accmul,
			motor3.angle_accmul,motor4.angle_accmul,motor5.angle_accmul,motor6.angle_accmul);
			
//			ANODT_Send_F1(MOTOR.pid_speed.target, MOTOR.pid_speed.measure);
			/*记得修改fputc*/
//			printf(" %d  %d  %.2f  %.2f \r\n",
//			motor1.pid_speed.target,motor1.pid_speed.measure,motor1.pid_speed.out, motor1.pwm_out);
		}
		
			
		
//		if(delay_task ==500)      //非阻塞延时3s
//		{
//			MOTOR.pid_angle.target += 2000;            
//			delay_task = 0;
//		}
//		
//		
//		if(Speed_Pid_cal_Task == TASK_ENABLE)
//		{
//			
//			Speed_Pid_cal_Task = 5;              
//			delay_task++;

//			
//			MOTOR_Angle_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//			
//			Set_Pwm(Set_PWM_Value,Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//			
//			ANODT_Send_F1(MOTOR.pid_angle.target, MOTOR.pid_angle.measure);
//			/*记得修改fputc*/
////			printf("%d  %.2f %d  %d  %.2f  %.2f \r\n",motor4.pid_angle.measure,motor4.pid_angle.out,
////			motor4.pid_speed.target,motor4.pid_speed.measure,motor4.pid_speed.out, motor4.pwm_out);
//		}
		
		
	}
}







//1503  -1643  -1662  1500  -1653  -1666
