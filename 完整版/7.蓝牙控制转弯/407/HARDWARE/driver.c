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
	
	
	//电机初始化
	MOTOR_Init(motor1,motor2,motor3,motor4,motor5,motor6);	   
    MOTOR_Angle_Init(motor1,motor2,motor3,motor4,motor5,motor6);
	
	SET_CLK(PID_TASK_CLK);            //打开pid任务周期计时clk
	RESET_CLK(STATION_CHANGE_CLK);    //关闭状态切换计时clk
	
}




enum Task_flag_t White_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station)
{
	enum Task_flag_t Task_flag;
	//在距离目标值较远时,进行速度环任务,使电机恒速转动;接近目标值时,转变为角度环稳定在目标值;这个接近值就是缓冲区buffer
	if(abs((int)(motor->pid_angle.target) - motor->angle_accmul) > 200)       
	{
		if(flag == 0)
			Task_flag = speed_pid_Task;
	}
	else 
		//角度环有可能超调,当电机超过目标值buffer后就会变回速度环,然后一直无法到达目标值;所以设置flag,使电机转变为角度环之后不会再变回速度环
		Task_flag = angle_pid_Task;
	
	return Task_flag;
}


enum Task_flag_t Blue_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station)
{
	enum Task_flag_t Task_flag;
	//在距离目标值较远时,进行速度环任务,使电机恒速转动;接近目标值时,转变为角度环稳定在目标值;这个接近值就是缓冲区buffer
	if(abs((int)(motor->pid_angle.target) - motor->angle_accmul) > 200)       
	{
		if(flag == 0)
			Task_flag = speed_pid_Task;
	}
	else 
		//角度环有可能超调,当电机超过目标值buffer后就会变回速度环,然后一直无法到达目标值;所以设置flag,使电机转变为角度环之后不会再变回速度环
		Task_flag = angle_pid_Task;
	
	return Task_flag;
}
