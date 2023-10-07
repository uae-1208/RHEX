#include "encoder.h"
#include "usart.h"
#include "motor.h"


static void TIM4_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
		
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOB6复用为定时器4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB6	 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); //GPIOB7复用为定时器4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOB7
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB 		
}

static void TIM4_NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void TIM4_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4时钟使能    
	
	TIM_DeInit(TIM4);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //向上计数模式,实际上经过我的测试编码器模式下它没啥用
	TIM_TimeBaseStructure.TIM_Period = 65535;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器4
	
	/*编码器配置*/
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//开启相关中断    

	TIM_SetCounter(TIM4,0);
	 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4							  
}  

  
void ENCODER_TIM_Init(void)	
{
	TIM4_GPIO_Init();
	TIM4_NVIC_Configuration();
	TIM4_IC_Mode_Init();
}


//short ENCODER_READ(u8 motor_num)	       
//{
//	short enc_val;
//	switch(motor_num)
//	{
//		case 1: enc_val = TIM_GetCounter(TIM4);
//			    TIM_SetCounter(TIM4,0);
//			    break;
//		default:break;
//	}
//	
//	return enc_val;
//}

short ENCODER_READ(u8 motor_num)	       
{
	switch(motor_num)
	{
		case 1: return TIM_GetCounter(TIM4);
			    break;
		default:break;
	}
	
	return 0;
}

