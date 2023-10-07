#include "tim.h"
#include "usart.h"
#include "stdbool.h"



static void TIMER_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn; 	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



void TIMER_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  	//TIM7时钟使能    
	
	TIMER_NVIC_Configuration();
	
	TIM_DeInit(TIM7);
	 
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=1000;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseStructure);//初始化定时器7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM7, ENABLE);  //使能TIM7							  
}  

extern bool PID_TASK_CLK;
extern u16 Pid_cal_Task;
extern bool STATION_CHANGE_CLK;
extern u16 Station_change_Task;

void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7,TIM_IT_Update) != RESET ) 
	{	

		if(PID_TASK_CLK == ON)         //管理该任务的时钟是开启状态
		{
			//1ms倒计时一次
			Pid_cal_Task--;
			if(Pid_cal_Task == 0)    	   
				RESET_CLK(PID_TASK_CLK); //计数至0时时钟复位
		}
		
		
		if(STATION_CHANGE_CLK == ON) 
		{
			if(Station_change_Task)
				Station_change_Task--;
						
			if(Station_change_Task == 0)
				RESET_CLK(STATION_CHANGE_CLK); 
		}
		
		
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  		 
	}		 	
}



