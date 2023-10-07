#include "stm32f4xx.h"
#include "tim.h"
#include "usart.h"
#include "systick.h"

#define motor1_dead_band   1300
#define motor2_dead_band   1350
#define motor3_dead_band   1200
#define motor4_dead_band   1250
#define motor5_dead_band   1300
#define motor6_dead_band   1250

extern u8 pwm_process_flag;
int main(void)
{
    short pwm_to_set[6];

	SysTick_Init();
	USART_Config();
	TIM2_Init();
	TIM3_Init();
	TIM4_Init();
  	
	while(1)
	{
	
        if(pwm_process_flag == SET)
		{
			pwm_process_flag = RESET;          //缓冲区处理标志复位
			Pwm_Data_Process(pwm_to_set);  //处理数据
		}


		if(pwm_to_set[0] > 0)
		{
			TIM_SetCompare2(TIM2,pwm_to_set[0]);            
			TIM_SetCompare1(TIM2,0);
		}
		else
        {
			TIM_SetCompare1(TIM2,-pwm_to_set[0]);            
			TIM_SetCompare2(TIM2,0);
		}
		                         TIM_SetCompare4(TIM2,pwm_to_set[1]);
			TIM_SetCompare3(TIM2,0);
		/****************/
		if(pwm_to_set[1] > 0)
		{
			if(pwm_to_set[1] + motor2_dead_band > 4200)
				TIM_SetCompare4(TIM2,4200);            
			else	
				TIM_SetCompare4(TIM2,pwm_to_set[1] + motor2_dead_band);
			
		}
		else
        {
			if(pwm_to_set[1] - motor2_dead_band < -4200)
				TIM_SetCompare3(TIM2,4200);            
			else	
				TIM_SetCompare3(TIM2,-pwm_to_set[1] + motor2_dead_band);            
				
			TIM_SetCompare4(TIM2,0);  
		}
		
		/****************/
		if(pwm_to_set[2] > 0)
		{
			TIM_SetCompare2(TIM3,pwm_to_set[2]);            
			TIM_SetCompare1(TIM3,0);
		}
		else
        {
			TIM_SetCompare1(TIM3,-pwm_to_set[2]);            
			TIM_SetCompare2(TIM3,0);
		}
		
		/****************/
		if(pwm_to_set[3] > 0)
		{
	 
			if(pwm_to_set[3] + motor4_dead_band > 4200)
				TIM_SetCompare3(TIM3,4200);            
			else	
				TIM_SetCompare3(TIM3,pwm_to_set[3] + motor4_dead_band);            
			TIM_SetCompare4(TIM3,0);
		}
		else
        {
			if(pwm_to_set[3] - motor4_dead_band < -4200)
				TIM_SetCompare4(TIM3,4200);            
			else	
				TIM_SetCompare4(TIM3,-pwm_to_set[3] + motor4_dead_band);            
			TIM_SetCompare3(TIM3,0);
		}
		
		
		/****************/
		if(pwm_to_set[4] > 0)
		{
			TIM_SetCompare4(TIM4,pwm_to_set[4]);            
			TIM_SetCompare3(TIM4,0);
		}
		else
        {
			TIM_SetCompare3(TIM4,-pwm_to_set[4]);            
			TIM_SetCompare4(TIM4,0);
		}
		
		
		/****************/
		if(pwm_to_set[5] > 0)
		{
			TIM_SetCompare2(TIM4,pwm_to_set[5]);            
			TIM_SetCompare1(TIM4,0);
		}
		else
        {
			TIM_SetCompare1(TIM4,-pwm_to_set[5]);            
			TIM_SetCompare2(TIM4,0);
		}
		
	}
}

