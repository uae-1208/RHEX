#include "usart.h"

//3-48行为匿名上位机协议
#define Byte0(dwTemp)      (*(char*)(&dwTemp))
#define Byte1(dwTemp)      (*((char*)(&dwTemp)+1))

u8 DataToSend[20];


static void Usart_Send(u8 length)
{
	u8 i;
	for(i=0;i<length;i++)
    {
		USART_SendData(USART1,DataToSend[i]);
		while((USART1->SR&0X40)==0);
	}
}

void ANODT_Send_F1(short target, short measure)
{
	u8 count = 0;
	u8 sc = 0;
	u8 ac = 0; 
	u8 i;
	
	DataToSend[count++] = 0xAA;
	DataToSend[count++] = 0xFF;
	DataToSend[count++] = 0xF1;
	DataToSend[count++] = 4;

	DataToSend[count++] = Byte0(target);
	DataToSend[count++] = Byte1(target);

	DataToSend[count++] = Byte0(measure);
	DataToSend[count++] = Byte1(measure);
	
	for(i=0;i<DataToSend[3]+4;i++)
	{
		sc += DataToSend[i];
		ac += sc;
	}

	DataToSend[count++] = sc;
	DataToSend[count++] = ac;

	Usart_Send(count);
}


int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

	return (ch);
}

void USART_NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void USART_GPIO_Configuration(void)
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
	
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断     匿名上位机看到数据传输错误会发送，然后就卡死在接收中断中
}



