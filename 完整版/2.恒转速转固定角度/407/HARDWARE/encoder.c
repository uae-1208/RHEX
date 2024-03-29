#include "encoder.h"
#include "usart.h"
#include "motor.h"


/*******************************************************电机1编码器*************************************************/
static void TIM8_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTC时钟
		
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8); //GPIOC6复用为定时器8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //初始化PC6	 
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); //GPIOC7复用为定时器8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOC7
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //初始化PC7		
}


static void TIM8_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8时钟使能    
	
	TIM_DeInit(TIM8);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //向上计数模式,实际上经过我的测试编码器模式下它没啥用
	TIM_TimeBaseStructure.TIM_Period = 65535;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//初始化定时器8
	
	/*编码器配置*/
	TIM_EncoderInterfaceConfig(TIM8,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM8,0);
	 
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8							  
}  
/*******************************************************************************************************************/

/*******************************************************电机2编码器*************************************************/
static void TIM4_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//使能PORTD时钟	
		
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOD12复用为定时器4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;           //GPIOD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //初始化PD12	 
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); //GPIOD13复用为定时器4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;           //GPIOD13
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //初始化PD13 		
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
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM4,0);
	 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4							  
}  
/*******************************************************************************************************************/

/*******************************************************电机3编码器*************************************************/
static void TIM2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2); //GPIOA15复用为定时器2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //GPIOA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA15	 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2); //GPIOB3复用为定时器2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;           //GPIOB3
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB3 		
}


static void TIM2_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM2时钟使能    
	
	TIM_DeInit(TIM2);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //向上计数模式,实际上经过我的测试编码器模式下它没啥用
	TIM_TimeBaseStructure.TIM_Period = 65535;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化定时�2
	
	/*编码器配置*/
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM2,0);
	 
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2							  
}  
/*******************************************************************************************************************/

/*******************************************************电机4编码器*************************************************/

static void TIM1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTE时钟	
		
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOE1复用为定时器1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOE9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE9	 
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOE11复用为定时器1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           //GPIOE11
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE11 		
}


static void TIM1_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	
	TIM_DeInit(TIM1);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //向上计数模式,实际上经过我的测试编码器模式下它没啥用
	TIM_TimeBaseStructure.TIM_Period = 65535;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	/*编码器配置*/
	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);

	TIM_SetCounter(TIM1,0);
	 
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1							  
}  
/*******************************************************************************************************************/

/*******************************************************电机5编码器*************************************************/

static void TIM3_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOA6复用为定时器3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA6	 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOA7复用为定时器3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOA7
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA7 		
}


static void TIM3_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
	
	TIM_DeInit(TIM3);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //向上计数模式,实际上经过我的测试编码器模式下它没啥用
	TIM_TimeBaseStructure.TIM_Period = 65535;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器3
	
	/*编码器配置*/
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Falling);

	TIM_SetCounter(TIM3,0);
	 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3							  
}  
/*******************************************************************************************************************/

/*******************************************************电机6编码器*************************************************/

static void TIM5_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOA0复用为定时器5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA0	 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //GPIOA1复用为定时器5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA1 		
}


static void TIM5_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	
//	TIM_DeInit(TIM5);

	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //向上计数模式,实际上经过我的测试编码器模式下它没啥用
	TIM_TimeBaseStructure.TIM_Period = 65535;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器5
	
	/*编码器配置*/
	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM5,0);

	TIM_Cmd(TIM5, ENABLE);  //使能TIM3							  
}  
/*******************************************************************************************************************/



//电机1编码器
static void ENCODER1_Init(void)	
{
	TIM8_GPIO_Init();
	TIM8_IC_Mode_Init();
}

//电机2编码器
static void ENCODER2_Init(void)	
{
	TIM4_GPIO_Init();
	TIM4_IC_Mode_Init();
}

//电机3编码器
static void ENCODER3_Init(void)	
{
	TIM2_GPIO_Init();
	TIM2_IC_Mode_Init();
}

//电机4编码器
static void ENCODER4_Init(void)	
{
	TIM1_GPIO_Init();
	TIM1_IC_Mode_Init();
}

//电机5编码器
static void ENCODER5_Init(void)	
{
	TIM3_GPIO_Init();
	TIM3_IC_Mode_Init();
}
 
//电机6编码器
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



