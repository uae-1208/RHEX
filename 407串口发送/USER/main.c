#include "stm32f4xx.h"
#include "tim.h"
#include "usart.h"
#include "key.h"
#include "systick.h"


int main(void)
{
	short Set_PWM_Value[6]={0};
	short Send_F401[8]={'P',0,0,0,0,0,0,'E'};
	u32 i=100000;
    u32 k=0;
	u8 j = 0;
	
	SysTick_Init();
	USART_Config();
	KEY_Init();	

  	while(1)
	{
		for(j=0;j<6;j++)
			Set_PWM_Value[j] = k++;
	
		Set_Pwm(Set_PWM_Value,Send_F401);

		while(i)
			i--;
			i=10000000;
	}
}

