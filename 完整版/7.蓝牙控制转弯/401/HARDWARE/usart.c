#include "usart.h"

/*F407����Э��:'P'(PWM)��ͷ,����12��short��������
(ÿ2�����ݷֱ�Ϊһ��PMֵ�ĵڰ�λ�͸߰�λ),'E'(END)��β����14������*/

char rec_buff[14];                          //������
u8 pwm_process_flag = RESET;                     //���������ı�־λ

void USART1_IRQHandler(void)                	
{
	short res;                                
	static u8 res_length = 0;                 //���յ����ݳ���
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		res = USART_ReceiveData(USART1);
		
		if(res_length == 0)
		{
			if(res == 'P')                     //���յ�֡ͷ'P'
				rec_buff[res_length++] = res;  //���յ�һ������
		}
		else
			rec_buff[res_length++] = res;
		
		if(res_length == 14)                   //������������
		{
			res_length = 0;                   //�������ݳ�������
			pwm_process_flag = SET;               //�����������־λ��λ,��ѭ���ô���������
		}
		
	}		
} 

void Pwm_Data_Process(short* pwm_to_set)
{
	u8 i;
	
	if(rec_buff[13] == 'E')
		for(i = 0; i < 6; i++)
			pwm_to_set[i] = (rec_buff[2*i+2] << 8) | rec_buff[2*i+1];	   //rec_buff[1]-rec_buff[13]ΪPWM����ֵ,��8λ��ǰ
			
	for(i = 0; i < 14; i++)         //����������
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
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
}



