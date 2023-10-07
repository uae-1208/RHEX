#include "sys.h"
#include "stm32f4xx.h"
#include "motor.h"
#include "encoder.h"
#include "usart.h"
#include "systick.h"
#include "stdlib.h"
#include "tim.h"
#include "led.h"   


u16 Speed_Pid_cal_Task = 5;           //pid��������5ms

enum Task_flag_t
{
	speed_pid_Task,     //�ٶȻ�����
	angle_pid_Task	    //�ǶȻ�����
};



void Judge_task(Motor_t* motor, u8* flag, enum Task_flag_t* Task_flag)
{
	//�ھ���Ŀ��ֵ��Զʱ,�����ٶȻ�����,ʹ�������ת��;�ӽ�Ŀ��ֵʱ,ת��Ϊ�ǶȻ��ȶ���Ŀ��ֵ;������ӽ�ֵ��Ϊ50
	if(abs(motor->pid_angle.target - motor->angle_accmul) > 25)       
	{
		if(*flag == 0)
			*Task_flag = speed_pid_Task;
	}
	else 
	{
		//�ǶȻ��п��ܳ���,���������Ŀ��ֵ50��ͻ����ٶȻ�,Ȼ��һֱ�޷�����Ŀ��ֵ;��������flag,ʹ���ת��Ϊ�ǶȻ�֮�󲻻��ٱ���ٶȻ�
		*Task_flag = angle_pid_Task;
		*flag = 1;
	}	
}

//28 200   3 25
#define MOTOR motor2

//�����ԣ�������ת��Ϊ35������ֵ/5ms
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
	
	//�����ʼ��
	MOTOR_Init(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);	    //zhp:���2������һ�໵��,��ת������
    
	MOTOR.pid_angle.target=640;

	while(1)
    {
		if(Speed_Pid_cal_Task == TASK_ENABLE)
		{
			Speed_Pid_cal_Task = 5;              
			Judge_task(&MOTOR,&flag,&Task_flag);          //�������ѡһ���������
			
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





		
//		if(delay_task ==500)      //��������ʱ3s
//		{
//			motor4.pid_speed.target = -motor4.pid_speed.target;            //�ٶȷ���
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
//			/*�ǵ��޸�fputc*/
////			printf(" %d  %d  %.2f  %.2f \r\n",
////			motor2.pid_speed.target,motor2.pid_speed.measure,motor2.pid_speed.out, motor2.pwm_out);
//		}
		
		
		
//		if(delay_task ==500)      //��������ʱ3s
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
//			/*�ǵ��޸�fputc*/
////			printf("%d  %.2f %d  %d  %.2f  %.2f \r\n",motor4.pid_angle.measure,motor4.pid_angle.out,
////			motor4.pid_speed.target,motor4.pid_speed.measure,motor4.pid_speed.out, motor4.pwm_out);
//		}



