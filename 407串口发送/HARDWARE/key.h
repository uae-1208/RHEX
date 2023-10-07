#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f4xx.h"
	 
#define KEY_READ() 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) //PB2，读取高电平时按下

enum{
	UP,        //未按下
	DOWN       //按下
};


void KEY_Init(void);	//IO初始化
u8 Key_Scan(void);


#endif
