#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx.h"

typedef struct __pid_t
{
    float kp;
    float ki;
    float kd;
    
	short dead_band;
	short outmax;
	short integral_max;
	
	int target;			    	//目标值
	int measure;				//测量值
    short error;                //误差值
    short last_error;           //上次误差值
	short integral;           

    float pout;							
    float iout;							
    float dout;		
	
    float out;				//本次位置式输出
}Pid_t;

void Pid_Calulate(Pid_t* Pid);
void Pid_Init(Pid_t* Pid,float kp, float ki, float kd, short dead_band, short outmax, short integral_max);

#endif
