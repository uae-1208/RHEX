#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

void USART_Config(void);
void USART1_SEND(u8 length, short* DataToSend);
void Set_Pwm(short* Set_PWM_Value,short* Send_F401);

#endif


