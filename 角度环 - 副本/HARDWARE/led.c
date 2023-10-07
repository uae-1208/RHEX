#include "led.h"   

void LED_Init(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����LED��ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOC, ENABLE); 

	/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	 

	/*��������ģʽΪ���ģʽ*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   

	/*�������ŵ��������Ϊ�������*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	/*��������Ϊ����ģʽ��Ĭ��LED��*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
