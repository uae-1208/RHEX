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

//�жϴ�ʱCNT�Ƿ����¼���  ��HAL��˳���ĺ���
#define TIM_IS_TIM_COUNTING_DOWN(TIM)    ((TIM->CR1 &(TIM_CR1_DIR)) == (TIM_CR1_DIR))

u16 Speed_Pid_cal_Task = 5;       //pid��������5ms
u16 Angle_Pid_cal_Task = 5;       //pid��������5ms
u8 Judge_over_flow_Task = RESET;

enum Task_flag_t
{
	speed_pid_Task,     //�ٶȻ�����
	angle_pid_Task	    //�ǶȻ�����
};


void Judge_task(Motor_t* motor, u8* flag, enum Task_flag_t* Task_flag)
{
	//�����0ת��1300,ǰ1250�Ժ㶨�ٶ�ת��,����ȶ���1300,���Ը�50�Ļ��������нǶȿ���
	if(abs(motor->pid_angle.target - motor->angle_accmul) < 50)
		*Task_flag = angle_pid_Task;
	else 
	{
		//���ǶȻ�����,����1350ʱ��Ӧ��Ϊ�ٶȻ�����,���ǽǶȻ��ص���1300,����flag��Ϊ��ʹ�ٶȻ�ת���ɽǶȻ��󲻻��ٱ���ٶȻ�
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
	
	//�����ʼ��
	MOTOR_Init(&motor1,&motor2,&motor2,&motor4,&motor5,&motor6);	
   	motor1.pid_angle.target = 1300;

	while(1)
    {
//		if(delay_task ==600)      //��������ʱ3s:5ms*600
//		{
//			motor1.pid_speed.target = -motor1.pid_speed.target;            //�ٶȷ���
//		    delay_task = 0;
//						printf("%d\r\n",motor1.pid_speed.target);

//		}
		
		
		
		//ͳ�Ʊ��������ֵ����.���������־λ��ԭ��:
		//1.�����ܼ����жϷ������Ĵ��� 2.��������ȫ�ֱ�����ʹ��ָ�봫��  by zhp
		if(Judge_over_flow_Task)              
		{
			if(TIM_IS_TIM_COUNTING_DOWN(TIM4))     //���¼���
				motor1.encoder_overflow_count --;
			else
				motor1.encoder_overflow_count ++;  //���ϼ���
			
			Judge_over_flow_Task = RESET;
		}
		  


		
		Judge_task(&motor1, &flag, &Task_flag);
		
		if(Task_flag == speed_pid_Task)          //Ӧ�������ٶȻ�����
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
		else      //�ǶȻ�����
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








