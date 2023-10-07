#include "motor.h"
#include "usart.h"
#include "encoder.h"
#include "math.h"

#define TIM_period 4200


//����PWMռ�ձȡ�100%	
#define LimitPwm(input)        			\
    {                          			\
        if (input > TIM_period)         \
        {                      			\
            input = TIM_period;         \
        }                       	    \
        else if (input < -TIM_period)   \
        {                      			\
            input = -TIM_period;        \
        }                      			\
    }	
	
	
//�����ų�ʼ��	
#define MOTOR_num_Init()	      \
{                                 \
	motor1->motor_num = 1;        \
	motor2->motor_num = 2;        \
	motor3->motor_num = 3;        \
	motor4->motor_num = 4;        \
	motor5->motor_num = 5;        \
	motor6->motor_num = 6;        \
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
	 
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //��ʱ����Ƶ                42M/1/4200=10kHZ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=TIM_period-1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��3
	
	//��ʼ��TIM3 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);	

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3										  
}

void MOTOR_TIM_Init(void)	
{
	TIM3_GPIO_Init();
	TIM3_OC_Mode_Init();
}





void MOTOR_Speed_Task(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
{	
	LimitPwm(motor1->pwm_out);
	
	if(motor1->pwm_out > 0)                         //��ת
	{
		TIM_SetCompare4(TIM3,(int)(motor1->pwm_out));
		TIM_SetCompare3(TIM3,0);
	}
	else                                            //��ת
	{
		TIM_SetCompare3(TIM3,(int)(-motor1->pwm_out));
		TIM_SetCompare4(TIM3,0);
	}		
}


