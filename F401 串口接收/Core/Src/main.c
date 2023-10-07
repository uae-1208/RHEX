#include "stm32f4xx.h"
#include "motor.h"
#include "usart.h"
#include "key.h"
#include "systick.h"
#include "stm32f4xx.h"
#include "tim.h"
#include "led.h"   


u16 Speed_Pid_cal_Task = 5;       //pid计算周期5ms

extern u8 pwm_process_flag;
int main(void)
{
	u16 delay_task = 0;
    short pwm_to_set[6];

	SysTick_Init();
	USART_Config();
	TIMER_Init();
	KEY_Init();	
	MOTOR_TIM_Init();
	
			
  	while(1)
    {
		if(delay_task ==20)      //非阻塞延时3s
		{
			printf("%d %d %d %d %d %d \r\n",pwm_to_set[0],
			pwm_to_set[1],pwm_to_set[2],pwm_to_set[3],pwm_to_set[4],pwm_to_set[5]);
			
			delay_task = 0;
		}
		
		if(pwm_process_flag == SET)
		{
			pwm_process_flag = RESET;          //缓冲区处理标志复位
			Pwm_Data_Process(pwm_to_set);  //处理数据
		}
		

		if(Speed_Pid_cal_Task == TASK_ENABLE)
		{
			Speed_Pid_cal_Task = 5;              
			delay_task++;
		}
		
	}
}

