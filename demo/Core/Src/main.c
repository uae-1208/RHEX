#include "stm32f4xx.h"
#include "tim.h"
#include "usart.h"
#include "key.h"
#include "systick.h"


int main(void)
{
	u8 flag = 0;
		u16 count;
		u32 i=1000000;

	SysTick_Init();
	USART_Config();
	KEY_Init();	
	TIM3_Init();
	TIM4_Init();
  	
	;
	while(1)
	{
//		if(Key_Scan())
//			flag++;
//		if(flag % 2)
//		{
//			TIM_SetCompare1(TIM3,500);
//			TIM_SetCompare3(TIM3,800);
//		}
//		else
//		{
//			TIM_SetCompare1(TIM3,0);
//			TIM_SetCompare3(TIM3,0);	
//		}
		
		TIM_SetCompare3(TIM3,475);            

		count=TIM_GetCounter(TIM4);
        printf("%d\r\n",count);
		while(i)
			i--;
			i=100000;
	}
}

