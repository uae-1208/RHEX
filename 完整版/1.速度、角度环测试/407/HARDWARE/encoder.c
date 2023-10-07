#include "encoder.h"
#include "usart.h"
#include "motor.h"


/*******************************************************���1������*************************************************/
static void TIM8_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTCʱ��
		
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8); //GPIOC6����Ϊ��ʱ��8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //��ʼ��PC6	 
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); //GPIOC7����Ϊ��ʱ��8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOC7
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //��ʼ��PC7		
}


static void TIM8_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8ʱ��ʹ��    
	
	TIM_DeInit(TIM8);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //���ϼ���ģʽ,ʵ���Ͼ����ҵĲ��Ա�����ģʽ����ûɶ��
	TIM_TimeBaseStructure.TIM_Period = 65535;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//��ʼ����ʱ��8
	
	/*����������*/
	TIM_EncoderInterfaceConfig(TIM8,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM8,0);
	 
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8							  
}  
/*******************************************************************************************************************/

/*******************************************************���2������*************************************************/
static void TIM4_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//ʹ��PORTDʱ��	
		
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOD12����Ϊ��ʱ��4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;           //GPIOD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //��ʼ��PD12	 
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); //GPIOD13����Ϊ��ʱ��4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;           //GPIOD13
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //��ʼ��PD13 		
}


static void TIM4_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4ʱ��ʹ��    
	
	TIM_DeInit(TIM4);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //���ϼ���ģʽ,ʵ���Ͼ����ҵĲ��Ա�����ģʽ����ûɶ��
	TIM_TimeBaseStructure.TIM_Period = 65535;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��4
	
	/*����������*/
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM4,0);
	 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4							  
}  
/*******************************************************************************************************************/

/*******************************************************���3������*************************************************/
static void TIM2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTBʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2); //GPIOA15����Ϊ��ʱ��2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //GPIOA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA15	 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2); //GPIOB3����Ϊ��ʱ��2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;           //GPIOB3
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB3 		
}


static void TIM2_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM2ʱ��ʹ��    
	
	TIM_DeInit(TIM2);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //���ϼ���ģʽ,ʵ���Ͼ����ҵĲ��Ա�����ģʽ����ûɶ��
	TIM_TimeBaseStructure.TIM_Period = 65535;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ����ʱ�2
	
	/*����������*/
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM2,0);
	 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2							  
}  
/*******************************************************************************************************************/

/*******************************************************���4������*************************************************/

static void TIM1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//ʹ��PORTEʱ��	
		
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOE1����Ϊ��ʱ��1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOE9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //��ʼ��PE9	 
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOE11����Ϊ��ʱ��1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           //GPIOE11
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //��ʼ��PE11 		
}


static void TIM1_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    
	
	TIM_DeInit(TIM1);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //���ϼ���ģʽ,ʵ���Ͼ����ҵĲ��Ա�����ģʽ����ûɶ��
	TIM_TimeBaseStructure.TIM_Period = 65535;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
	
	/*����������*/
	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);

	TIM_SetCounter(TIM1,0);
	 
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1							  
}  
/*******************************************************************************************************************/

/*******************************************************���5������*************************************************/

static void TIM3_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOA6����Ϊ��ʱ��3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA6	 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOA7����Ϊ��ʱ��3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOA7
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA7 		
}


static void TIM3_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
	
	TIM_DeInit(TIM3);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //���ϼ���ģʽ,ʵ���Ͼ����ҵĲ��Ա�����ģʽ����ûɶ��
	TIM_TimeBaseStructure.TIM_Period = 65535;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��3
	
	/*����������*/
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Falling);

	TIM_SetCounter(TIM3,0);
	 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3							  
}  
/*******************************************************************************************************************/

/*******************************************************���6������*************************************************/

static void TIM5_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOA0����Ϊ��ʱ��5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA0	 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //GPIOA1����Ϊ��ʱ��5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA1 		
}


static void TIM5_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	
//	TIM_DeInit(TIM5);

	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //���ϼ���ģʽ,ʵ���Ͼ����ҵĲ��Ա�����ģʽ����ûɶ��
	TIM_TimeBaseStructure.TIM_Period = 65535;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//��ʼ����ʱ��5
	
	/*����������*/
	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM5,0);

	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM3							  
}  
/*******************************************************************************************************************/



//���1������
static void ENCODER1_Init(void)	
{
	TIM8_GPIO_Init();
	TIM8_IC_Mode_Init();
}

//���2������
static void ENCODER2_Init(void)	
{
	TIM4_GPIO_Init();
	TIM4_IC_Mode_Init();
}

//���3������
static void ENCODER3_Init(void)	
{
	TIM2_GPIO_Init();
	TIM2_IC_Mode_Init();
}

//���4������
static void ENCODER4_Init(void)	
{
	TIM1_GPIO_Init();
	TIM1_IC_Mode_Init();
}

//���5������
static void ENCODER5_Init(void)	
{
	TIM3_GPIO_Init();
	TIM3_IC_Mode_Init();
}
 
//���6������
static void ENCODER6_Init(void)	
{
	TIM5_GPIO_Init();
	TIM5_IC_Mode_Init();
}

void ENCODER_TIM_Init(void)	
{
	ENCODER1_Init();
	ENCODER2_Init();
	ENCODER3_Init();
	ENCODER4_Init();
	ENCODER5_Init();
	ENCODER6_Init();

}





short ENCODER_READ(u8 motor_num)	       
{
	short enc_val;
	switch(motor_num)
	{
		case 1: enc_val = TIM_GetCounter(MOTOR1_TIM);
			    TIM_SetCounter(MOTOR1_TIM,0);
				break;
		case 2: enc_val = TIM_GetCounter(MOTOR2_TIM);
			    TIM_SetCounter(MOTOR2_TIM,0);
			    break;
		case 3: enc_val = TIM_GetCounter(MOTOR3_TIM);
			    TIM_SetCounter(MOTOR3_TIM,0);
			    break;
		case 4: enc_val = TIM_GetCounter(MOTOR4_TIM);
			    TIM_SetCounter(MOTOR4_TIM,0);
			    break;
		case 5: enc_val = TIM_GetCounter(MOTOR5_TIM);
			    TIM_SetCounter(MOTOR5_TIM,0);
			    break;
		case 6: enc_val = TIM_GetCounter(MOTOR6_TIM);
			    TIM_SetCounter(MOTOR6_TIM,0);
			    break;
		default:break;
	}
	
	return enc_val;
}



