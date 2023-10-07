#include "tim.h"
#include "usart.h"

extern u16 Speed_Pid_cal_Task;

static void TIMER_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



void TIMER_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	
	TIMER_NVIC_Configuration();
	
	TIM_DeInit(TIM5);
	 
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=1000;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM5, ENABLE);  //使能TIM5							  
}  



void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET ) 
	{	

		if(Speed_Pid_cal_Task)
			Speed_Pid_cal_Task--;

		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  		 
	}		 	
}



