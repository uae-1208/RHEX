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
	
	int target;			    	//Ŀ��ֵ
	int measure;				//����ֵ
    short error;                //���ֵ
    short last_error;           //�ϴ����ֵ
	short integral;           

    float pout;							
    float iout;							
    float dout;		
	
    float out;				//����λ��ʽ���
}Pid_t;

void Pid_Calulate(Pid_t* Pid);
void Pid_Init(Pid_t* Pid,float kp, float ki, float kd, short dead_band, short outmax, short integral_max);

#endif
