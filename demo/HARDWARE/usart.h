#ifndef __USART_H__
#define	__USART_H__

#include "stm32f4xx.h"
#include <stdio.h>

void USART_NVIC_Configuration(void);
void USART_GPIO_Configuration(void);
void USART_Config(void);
int fputc(int ch, FILE *f);


#endif
