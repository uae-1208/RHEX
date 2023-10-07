#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "motor.h" 



void USART_Config(void);
void BLUETOOTH_Config(void);
void Set_Pwm(short* Send_F401,Motor_t* motor);
void ANODT_Send_F1(short target, short measure);
void ANODT_Send_F2(short target1, short measure1, short target2, short measure2);

#endif


