#include "driver.h"
#include "encoder.h"
#include "usart.h"
#include "systick.h"
#include "tim.h"
#include "stdlib.h"


extern bool PID_TASK_CLK;            
extern bool STATION_CHANGE_CLK;


void System_Init(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
{
	SysTick_Init();
	USART_Config();
	BLUETOOTH_Config();
	TIMER_Init();
	MOTOR_TIM_Init();
	ENCODER_TIM_Init();	
	
	
	//�����ʼ��
	MOTOR_Init(motor1,motor2,motor3,motor4,motor5,motor6);	   
    MOTOR_Angle_Init(motor1,motor2,motor3,motor4,motor5,motor6);
	
	SET_CLK(PID_TASK_CLK);            //��pid�������ڼ�ʱclk
	RESET_CLK(STATION_CHANGE_CLK);    //�ر�״̬�л���ʱclk
	
}




enum Task_flag_t White_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station)
{
	enum Task_flag_t Task_flag;
	//�ھ���Ŀ��ֵ��Զʱ,�����ٶȻ�����,ʹ�������ת��;�ӽ�Ŀ��ֵʱ,ת��Ϊ�ǶȻ��ȶ���Ŀ��ֵ;����ӽ�ֵ���ǻ�����buffer
	if(abs((int)(motor->pid_angle.target) - motor->angle_accmul) > 200)       
	{
		if(flag == 0)
			Task_flag = speed_pid_Task;
	}
	else 
		//�ǶȻ��п��ܳ���,���������Ŀ��ֵbuffer��ͻ����ٶȻ�,Ȼ��һֱ�޷�����Ŀ��ֵ;��������flag,ʹ���ת��Ϊ�ǶȻ�֮�󲻻��ٱ���ٶȻ�
		Task_flag = angle_pid_Task;
	
	return Task_flag;
}


enum Task_flag_t Blue_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station)
{
	enum Task_flag_t Task_flag;
	//�ھ���Ŀ��ֵ��Զʱ,�����ٶȻ�����,ʹ�������ת��;�ӽ�Ŀ��ֵʱ,ת��Ϊ�ǶȻ��ȶ���Ŀ��ֵ;����ӽ�ֵ���ǻ�����buffer
	if(abs((int)(motor->pid_angle.target) - motor->angle_accmul) > 200)       
	{
		if(flag == 0)
			Task_flag = speed_pid_Task;
	}
	else 
		//�ǶȻ��п��ܳ���,���������Ŀ��ֵbuffer��ͻ����ٶȻ�,Ȼ��һֱ�޷�����Ŀ��ֵ;��������flag,ʹ���ת��Ϊ�ǶȻ�֮�󲻻��ٱ���ٶȻ�
		Task_flag = angle_pid_Task;
	
	return Task_flag;
}
