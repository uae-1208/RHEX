#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx.h"

#define MOTOR1_TIM  TIM8
#define MOTOR2_TIM  TIM4
#define MOTOR3_TIM  TIM2
#define MOTOR4_TIM  TIM1
#define MOTOR5_TIM  TIM3
#define MOTOR6_TIM  TIM5

void ENCODER_TIM_Init(void);
short ENCODER_READ(u8 motor_num);	
#endif
