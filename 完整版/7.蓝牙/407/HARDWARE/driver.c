#include "driver.h"
#include "encoder.h"
#include "usart.h"
#include "systick.h"
#include "tim.h"
#include "stdlib.h"


extern bool PID_TASK_CLK;            


void System_Init(Motor_t* motor)
{
	SysTick_Init();
	USART_Config();
	BLUETOOTH_Config();
	TIMER_Init();
	MOTOR_TIM_Init();
	ENCODER_TIM_Init();	
	
	//电机初始化
	MOTOR_Init(motor);	   
    MOTOR_Angle_Init(motor);
	
	SET_CLK(PID_TASK_CLK);            //打开pid任务周期计时clk	
}


u8 Get_Close_To_Target(Motor_t* motor)
{
	u8 i;
	for(i=0;i<6;i++)
		if(abs((int)(motor[i].pid_angle.target) - motor[i].angle_accmul) > 50)
			return 0;
	return 1;
}
