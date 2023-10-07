#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx.h"

void ENCODER_TIM_Init(void);
short ENCODER_READ(u8 motor_num);	
#endif
