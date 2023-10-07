#include "encoder.h"
#include "usart.h"
#include "motor.h"


/*******************************************************µç»ú1±àÂëÆ÷*************************************************/
static void TIM8_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//Ê¹ÄÜPORTCÊ±ÖÓ
		
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8); //GPIOC6¸´ÓÃÎª¶¨Ê±Æ÷8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //³õÊ¼»¯PC6	 
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); //GPIOC7¸´ÓÃÎª¶¨Ê±Æ÷8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOC7
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //³õÊ¼»¯PC7		
}


static void TIM8_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8Ê±ÖÓÊ¹ÄÜ    
	
	TIM_DeInit(TIM8);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //ÏòÉÏ¼ÆÊýÄ£Ê½,Êµ¼ÊÉÏ¾­¹ýÎÒµÄ²âÊÔ±àÂëÆ÷Ä£Ê½ÏÂËüÃ»É¶ÓÃ
	TIM_TimeBaseStructure.TIM_Period = 65535;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//³õÊ¼»¯¶¨Ê±Æ÷8
	
	/*±àÂëÆ÷ÅäÖÃ*/
	TIM_EncoderInterfaceConfig(TIM8,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM8,0);
	 
	TIM_Cmd(TIM8, ENABLE);  //Ê¹ÄÜTIM8							  
}  
/*******************************************************************************************************************/

/*******************************************************µç»ú2±àÂëÆ÷*************************************************/
static void TIM4_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//Ê¹ÄÜPORTDÊ±ÖÓ	
		
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOD12¸´ÓÃÎª¶¨Ê±Æ÷4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;           //GPIOD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //³õÊ¼»¯PD12	 
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); //GPIOD13¸´ÓÃÎª¶¨Ê±Æ÷4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;           //GPIOD13
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //³õÊ¼»¯PD13 		
}


static void TIM4_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4Ê±ÖÓÊ¹ÄÜ    
	
	TIM_DeInit(TIM4);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //ÏòÉÏ¼ÆÊýÄ£Ê½,Êµ¼ÊÉÏ¾­¹ýÎÒµÄ²âÊÔ±àÂëÆ÷Ä£Ê½ÏÂËüÃ»É¶ÓÃ
	TIM_TimeBaseStructure.TIM_Period = 65535;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//³õÊ¼»¯¶¨Ê±Æ÷4
	
	/*±àÂëÆ÷ÅäÖÃ*/
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM4,0);
	 
	TIM_Cmd(TIM4, ENABLE);  //Ê¹ÄÜTIM4							  
}  
/*******************************************************************************************************************/

/*******************************************************µç»ú3±àÂëÆ÷*************************************************/
static void TIM2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//Ê¹ÄÜPORTBÊ±ÖÓ	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//Ê¹ÄÜPORTAÊ±ÖÓ	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2); //GPIOA15¸´ÓÃÎª¶¨Ê±Æ÷2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //GPIOA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //³õÊ¼»¯PA15	 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2); //GPIOB3¸´ÓÃÎª¶¨Ê±Æ÷2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;           //GPIOB3
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //³õÊ¼»¯PB3 		
}


static void TIM2_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM2Ê±ÖÓÊ¹ÄÜ    
	
	TIM_DeInit(TIM2);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //ÏòÉÏ¼ÆÊýÄ£Ê½,Êµ¼ÊÉÏ¾­¹ýÎÒµÄ²âÊÔ±àÂëÆ÷Ä£Ê½ÏÂËüÃ»É¶ÓÃ
	TIM_TimeBaseStructure.TIM_Period = 65535;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//³õÊ¼»¯¶¨Ê±Æ2
	
	/*±àÂëÆ÷ÅäÖÃ*/
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM2,0);
	 
	TIM_Cmd(TIM2, ENABLE);  //Ê¹ÄÜTIM2							  
}  
/*******************************************************************************************************************/

/*******************************************************µç»ú4±àÂëÆ÷*************************************************/

static void TIM1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//Ê¹ÄÜPORTEÊ±ÖÓ	
		
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOE1¸´ÓÃÎª¶¨Ê±Æ÷1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOE9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //³õÊ¼»¯PE9	 
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOE11¸´ÓÃÎª¶¨Ê±Æ÷1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           //GPIOE11
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //³õÊ¼»¯PE11 		
}


static void TIM1_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1Ê±ÖÓÊ¹ÄÜ    
	
	TIM_DeInit(TIM1);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //ÏòÉÏ¼ÆÊýÄ£Ê½,Êµ¼ÊÉÏ¾­¹ýÎÒµÄ²âÊÔ±àÂëÆ÷Ä£Ê½ÏÂËüÃ»É¶ÓÃ
	TIM_TimeBaseStructure.TIM_Period = 65535;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//³õÊ¼»¯¶¨Ê±Æ÷1
	
	/*±àÂëÆ÷ÅäÖÃ*/
	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);

	TIM_SetCounter(TIM1,0);
	 
	TIM_Cmd(TIM1, ENABLE);  //Ê¹ÄÜTIM1							  
}  
/*******************************************************************************************************************/

/*******************************************************µç»ú5±àÂëÆ÷*************************************************/

static void TIM3_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//Ê¹ÄÜPORTAÊ±ÖÓ	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOA6¸´ÓÃÎª¶¨Ê±Æ÷3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //³õÊ¼»¯PA6	 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOA7¸´ÓÃÎª¶¨Ê±Æ÷3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOA7
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //³õÊ¼»¯PA7 		
}


static void TIM3_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3Ê±ÖÓÊ¹ÄÜ    
	
	TIM_DeInit(TIM3);
	 
	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //ÏòÉÏ¼ÆÊýÄ£Ê½,Êµ¼ÊÉÏ¾­¹ýÎÒµÄ²âÊÔ±àÂëÆ÷Ä£Ê½ÏÂËüÃ»É¶ÓÃ
	TIM_TimeBaseStructure.TIM_Period = 65535;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//³õÊ¼»¯¶¨Ê±Æ÷3
	
	/*±àÂëÆ÷ÅäÖÃ*/
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Falling);

	TIM_SetCounter(TIM3,0);
	 
	TIM_Cmd(TIM3, ENABLE);  //Ê¹ÄÜTIM3							  
}  
/*******************************************************************************************************************/

/*******************************************************µç»ú6±àÂëÆ÷*************************************************/

static void TIM5_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//Ê¹ÄÜPORTAÊ±ÖÓ	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOA0¸´ÓÃÎª¶¨Ê±Æ÷5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //³õÊ¼»¯PA0	 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //GPIOA1¸´ÓÃÎª¶¨Ê±Æ÷5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOA1
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //³õÊ¼»¯PA1 		
}


static void TIM5_IC_Mode_Init(void)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5Ê±ÖÓÊ¹ÄÜ    
	
//	TIM_DeInit(TIM5);

	TIM_TimeBaseStructure.TIM_Prescaler= 0;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; //ÏòÉÏ¼ÆÊýÄ£Ê½,Êµ¼ÊÉÏ¾­¹ýÎÒµÄ²âÊÔ±àÂëÆ÷Ä£Ê½ÏÂËüÃ»É¶ÓÃ
	TIM_TimeBaseStructure.TIM_Period = 65535;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//³õÊ¼»¯¶¨Ê±Æ÷5
	
	/*±àÂëÆ÷ÅäÖÃ*/
	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Falling);
	
	TIM_SetCounter(TIM5,0);

	TIM_Cmd(TIM5, ENABLE);  //Ê¹ÄÜTIM3							  
}  
/*******************************************************************************************************************/



//µç»ú1±àÂëÆ÷
static void ENCODER1_Init(void)	
{
	TIM8_GPIO_Init();
	TIM8_IC_Mode_Init();
}

//µç»ú2±àÂëÆ÷
static void ENCODER2_Init(void)	
{
	TIM4_GPIO_Init();
	TIM4_IC_Mode_Init();
}

//µç»ú3±àÂëÆ÷
static void ENCODER3_Init(void)	
{
	TIM2_GPIO_Init();
	TIM2_IC_Mode_Init();
}

//µç»ú4±àÂëÆ÷
static void ENCODER4_Init(void)	
{
	TIM1_GPIO_Init();
	TIM1_IC_Mode_Init();
}

//µç»ú5±àÂëÆ÷
static void ENCODER5_Init(void)	
{
	TIM3_GPIO_Init();
	TIM3_IC_Mode_Init();
}
 
//µç»ú6±àÂëÆ÷
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



