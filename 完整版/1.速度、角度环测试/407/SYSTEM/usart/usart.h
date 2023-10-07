#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "motor.h" 



void USART_Config(void);
void Set_Pwm(short* Set_PWM_Value,short* Send_F401,Motor_t* motor1,Motor_t* motor2,
			 Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6);

void BLUETOOTH_Config(void);
void ANODT_Send_F1(short target, short measure);

#endif


