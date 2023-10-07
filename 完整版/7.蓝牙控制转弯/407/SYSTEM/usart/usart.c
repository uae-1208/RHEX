#include "sys.h"
#include "usart.h"	

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
	USART3->DR = (u8) ch;      
	return ch;
}


/*****************������λ��Э��**********************/
#define Byte0(dwTemp)      (*(char*)(&dwTemp))
#define Byte1(dwTemp)      (*((char*)(&dwTemp)+1))


static void USART3_SEND(u8 length, char* DataToSend)
{
	u8 i;
	
	for(i=0;i<length;i++)
    {
		USART_SendData(USART3,DataToSend[i]);
		while((USART3->SR&0X40)==0);
	}
}

void ANODT_Send_F1(short target, short measure)
{
	char DataToSend[20];
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

	USART3_SEND(count,DataToSend);
}
/**************************************************/


void ANODT_Send_F2(short target1, short measure1, short target2, short measure2)
{
	char DataToSend[24];
	u8 count = 0;
	u8 sc = 0;
	u8 ac = 0; 
	u8 i;
	
	DataToSend[count++] = 0xAA;
	DataToSend[count++] = 0xFF;
	DataToSend[count++] = 0xF2;
	DataToSend[count++] = 8;

	DataToSend[count++] = Byte0(target1);
	DataToSend[count++] = Byte1(target1);

	DataToSend[count++] = Byte0(measure1);
	DataToSend[count++] = Byte1(measure1);
	
	DataToSend[count++] = Byte0(target2);
	DataToSend[count++] = Byte1(target2);

	DataToSend[count++] = Byte0(measure2);
	DataToSend[count++] = Byte1(measure2);
	
	for(i=0;i<DataToSend[3]+4;i++)
	{
		sc += DataToSend[i];
		ac += sc;
	}

	DataToSend[count++] = sc;
	DataToSend[count++] = ac;

	USART3_SEND(count,DataToSend);
}








static void USART1_SEND(u8 length, short* DataToSend)
{
	u8 i = 0;
	USART_SendData(USART1,DataToSend[i]);   //����֡ͷ'P'
	while((USART1->SR&0X40)==0);
	
	for(i=1;i<length-1;i++)
    {
		USART_SendData(USART1,(DataToSend[i] & 0xFF));        //����6�����ݵĵͰ�λ
		while((USART1->SR&0X40)==0);
		USART_SendData(USART1,((DataToSend[i] >> 8) & 0xFF)); //����6�����ݵĸ߰�λ
		while((USART1->SR&0X40)==0);		
	}

	USART_SendData(USART1,DataToSend[i]);   //����֡β'E'
	while((USART1->SR&0X40)==0);
}


void Set_Pwm(short* Send_F401,Motor_t* motor1,Motor_t* motor2,
			 Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
{	
	Send_F401[1] = motor1->pwm_out;
	Send_F401[2] = motor2->pwm_out;
	Send_F401[3] = motor3->pwm_out;
	Send_F401[4] = motor4->pwm_out;
	Send_F401[5] = motor5->pwm_out;
	Send_F401[6] = motor6->pwm_out;

	USART1_SEND(8,Send_F401);	
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
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
}


void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_GPIO_Configuration();
	USART_NVIC_Configuration();
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			
	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
}




static void BLUETOOTH_NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


static void BLUETOOTH_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB10,PB11
}


void BLUETOOTH_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	BLUETOOTH_NVIC_Configuration();
	BLUETOOTH_GPIO_Configuration();
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
			
	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�
}

extern u8 order;
void USART3_IRQHandler(void)                	
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
	{
		order = USART_ReceiveData(USART3);
	}		
} 