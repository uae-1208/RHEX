#include "tim.h"
#include "usart.h"


static void TIM2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2); //GPIOA0����Ϊ��ʱ��2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA0	 
	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2); //GPIOA1����Ϊ��ʱ��2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA1	 
		
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2); //GPIOA2����Ϊ��ʱ��2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;           //GPIOA2
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA2	 

		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2); //GPIOA3����Ϊ��ʱ��2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;           //GPIOA3
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA3	 

}



static void TIM2_OC_Mode_Init(void)
{		 					 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM2ʱ��ʹ��    
	 
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=4200;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ����ʱ��2
	
	//��ʼ��TIM2 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);	

	//��ʼ��TIM2 Channelx PWMģʽ	 
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);	
		
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);	

	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);	

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2										  
}

void TIM2_Init(void)	
{
	TIM2_GPIO_Init();
	TIM2_OC_Mode_Init();
}






static void TIM3_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTA��PORTBʱ��	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOA6����Ϊ��ʱ��3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA5	 
	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOA7����Ϊ��ʱ��3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOA7
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA7	 
		
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); //GPIOB0����Ϊ��ʱ��3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOB0
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB0	 

		
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); //GPIOB1����Ϊ��ʱ��3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOB1
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB1	 

}



static void TIM3_OC_Mode_Init(void)
{		 					 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
	 
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=4200;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��3
	
	//��ʼ��TIM3 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	

	//��ʼ��TIM3 Channelx PWMģʽ	 
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	
		
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	

	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);	

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3										  
}

void TIM3_Init(void)	
{
	TIM3_GPIO_Init();
	TIM3_OC_Mode_Init();
}




static void TIM4_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTBʱ��	
		
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOB6����Ϊ��ʱ��4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB6	 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); //GPIOB7����Ϊ��ʱ��4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOB7
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB7 	

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); //GPIOB8����Ϊ��ʱ��4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIOB8
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB8 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); //GPIOB9����Ϊ��ʱ��4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOB9
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB9 
}

static void TIM4_OC_Mode_Init(void)
{		 					 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4ʱ��ʹ��    
	 
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=4200;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��4
	
	//��ʼ��TIM4 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);	

	//��ʼ��TIM4 Channelx PWMģʽ	 
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);	
		
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);	

	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4										  
}

void TIM4_Init(void)	
{
	TIM4_GPIO_Init();
	TIM4_OC_Mode_Init();
}




