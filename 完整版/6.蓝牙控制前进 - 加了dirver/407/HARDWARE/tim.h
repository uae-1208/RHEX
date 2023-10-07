#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f4xx.h"

/*���������*/
#define TASK_ENABLE 0
#define OFF 0
#define ON  1
#define SET_CLK(clk)     (clk) = ON
#define RESET_CLK(clk)   (clk) = OFF

void TIMER_Init(void);

#endif

