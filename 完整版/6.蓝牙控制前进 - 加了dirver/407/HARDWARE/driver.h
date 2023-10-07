#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "stm32f4xx.h"
#include "motor.h"
#include "stdbool.h"   

#define FORWARD 1
#define STOP    0


#define get_white_buffer(station)    (((station)==station1) ? 70 : 200)
#define get_blue_buffer(station)    (((station)==station1) ? 200 : 70)
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
	
	
enum Task_flag_t
{
	speed_pid_Task,     //速度环任务
	angle_pid_Task	    //角度环任务
};

enum Move_station_t
{
	station1,     //电机2、4、6转320°
	station2	  //电机2、4、6转40°
};


enum Task_flag_t White_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station);    
enum Task_flag_t Blue_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station);    
void System_Init(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6);



#endif
