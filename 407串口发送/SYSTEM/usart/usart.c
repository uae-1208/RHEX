#include "sys.h"
#include "usart.h"	

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
 

void USART1_SEND(u8 length, short* DataToSend)
{
	u8 i = 0;
	USART_SendData(USART1,DataToSend[i]);   //发送帧头'P'
	while((USART1->SR&0X40)==0);
	
	for(i=1;i<length-1;i++)
    {
		USART_SendData(USART1,(DataToSend[i] & 0xFF));        //发送6个数据的低八位
		while((USART1->SR&0X40)==0);
		USART_SendData(USART1,((DataToSend[i] >> 8) & 0xFF)); //发送6个数据的高八位
		while((USART1->SR&0X40)==0);		
	}

	USART_SendData(USART1,DataToSend[i]);   //发送帧尾'E'
	while((USART1->SR&0X40)==0);
}


void Set_Pwm(short* Set_PWM_Value,short* Send_F401)
{
	int i;
	for(i=1;i<7;i++)
		Send_F401[i] = Set_PWM_Value[i-1];
	USART1_SEND(8,Send_F401);	
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		USART_ReceiveData(USART1);

	}   		 
 } 

 


static void USART_NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


static void USART_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
}


void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_GPIO_Configuration();
	USART_NVIC_Configuration();
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
}






