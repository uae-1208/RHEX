#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "stm32f4xx.h"
#include "motor.h"
#include "stdbool.h"   

#define GROUP1_INC 1      //motor1、4 奐
#define GROUP1_DEC 2      //motor1、4 受
#define GROUP2_INC 3      //motor2、5 奐
#define GROUP2_DEC 4      //motor2、5 受
#define GROUP3_INC 5      //motor3、6 奐
#define GROUP3_DEC 6      //motor3、6 受
#define STOP       0

#define angle_To_encoder(angle) ((short)((angle)*Motor_Prescaler/360))
#define Limit_Init_Pwm(input)        	\
    {                          			\
        if (input > 3600)         		\
        {                      			\
            input = 3600;         		\
        }                       	    \
        else if (input < -3600)   		\
        {                      			\
            input = -3600;        		\
        }                      			\
    }
	
	
void System_Init(Motor_t* motor);
u8 Get_Close_To_Target(Motor_t* motor);


#endif
