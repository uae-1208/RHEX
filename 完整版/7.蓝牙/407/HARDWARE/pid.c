#include "pid.h"
#include "stdlib.h"
#include "usart.h"

#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }
	
void Pid_Calulate(Pid_t* Pid)
{	
	Pid->error = Pid->target - Pid ->measure ;   
	if(abs((int)(Pid->error)) < Pid->dead_band)                             
	{
		Pid->error = 0;
		Pid->integral = 0;
	}
	Pid->pout = Pid->kp * Pid->error;
	Pid->iout = Pid->ki * Pid->integral;
	Pid->dout = Pid->kd * (Pid->error - Pid->last_error);
	
	Pid->out = Pid->pout + Pid->iout + Pid->dout;
	LimitMax(Pid->out,Pid->outmax);
	
	Pid->integral += Pid->error;
	LimitMax(Pid->integral,Pid->integral_max);                   
	Pid->last_error = Pid->error;
}

void Pid_Init(Pid_t* Pid,float kp, float ki, float kd, short dead_band, short outmax, short integral_max)
{
	Pid->kp = kp;
    Pid->ki = ki;
    Pid->kd = kd;
    
	Pid->dead_band = dead_band;
	Pid->outmax = outmax;
	Pid->integral_max = integral_max;
	
	Pid->target = 0;				
	Pid->measure = 0;				
    Pid->error = 0;                
    Pid->last_error = 0;           
	Pid->integral = 0;           

    Pid->pout = 0;							
    Pid->iout = 0;							
    Pid->dout = 0;		
	
    Pid->out = 0;			
}


