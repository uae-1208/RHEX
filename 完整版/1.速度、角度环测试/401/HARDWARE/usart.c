#include "usart.h"

/*F407发送协议:'P'(PWM)开头,紧跟12个short类型数据
(每2个数据分别为一个PM值的第八位和高八位),'E'(END)结尾。共14个数据*/

char rec_buff[14];                          //缓冲区
u8 pwm_process_flag = RESET;                     //处理缓存区的标志位

void USART1_IRQHandler(void)                	
{
	short res;                                
	static u8 res_length = 0;                 //接收的数据长度
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		res = USART_ReceiveData(USART1);
		
		if(res_length == 0)
		{
			if(res == 'P')                     //接收到帧头'P'
				rec_buff[res_length++] = res;  //接收第一个数据
		}
		else
			rec_buff[res_length++] = res;
		
		if(res_length == 14)                   //接收数据已满
		{
			res_length = 0;                   //接收数据长度清零
			pwm_process_flag = SET;               //缓存区处理标志位置位,主循环该处理缓存区了
		}
		
	}		
} 

void Pwm_Data_Process(short* pwm_to_set)
{
	u8 i;
	
	if(rec_buff[13] == 'E')
		for(i = 0; i < 6; i++)
			pwm_to_set[i] = (rec_buff[2*i+2] << 8) | rec_buff[2*i+1];	   //rec_buff[1]-rec_buff[13]为PWM数据值,低8位在前
			
	for(i = 0; i < 14; i++)         //缓冲区清零
		rec_buff[i] = 0;		
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
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
}



