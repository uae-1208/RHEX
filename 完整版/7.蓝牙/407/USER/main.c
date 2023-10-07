#include "sys.h"
#include "stm32f4xx.h"
#include "motor.h"
#include "encoder.h"
#include "usart.h"
#include "systick.h"
#include "stdlib.h"
#include "tim.h"
#include "led.h"   
#include "driver.h"   

bool PID_TASK_CLK;            //PID_TASK虚拟时钟,开启时PID_TASK才会正常倒计时
u16 Pid_cal_Task = 5;         //pid任务周期5ms

u8 order;           //蓝牙命令

#define sen 0.08
//经测试，电机最大转速为35编码器值/5ms
int main(void)
{
	Motor_t MOTOR[6];
	short Send_F401[8]={'P',0,0,0,0,0,0,'E'};

	System_Init(MOTOR);
	int loop = 0;
	
	//初始化任务
	while(1)
	{
		static u16 delay_time = 0;
		static u8 delay_flag = 0;
		
		if(Pid_cal_Task == TASK_ENABLE)
        {
			u8 i;
			SET_CLK(PID_TASK_CLK);           
			Pid_cal_Task = 5; 
			
			for(i=0;i<6;i++)
				Single_MOTOR_Angle_Task(MOTOR+i);

			for(i=0;i<6;i++)          
				Limit_Init_Pwm(MOTOR[i].pwm_out);

			Set_Pwm(Send_F401,MOTOR);

			if(Get_Close_To_Target(MOTOR)) 
				delay_flag = 1;
			
			if(delay_flag)              //距离目标位置50以内开始计时,计时2s退出初始循环
				delay_time ++;
			if(delay_time == 400)
				break;
		}
		
//		ANODT_Send_F1((int)motor1.pid_speed.target, motor1.pid_speed.measure);
//		ANODT_Send_F1((int)motor1.pid_angle.measure, motor1.pid_angle.target);
//		ANODT_Send_F2((int)motor1.pid_speed.target, motor1.pid_speed.measure,motor2.pid_speed.measure,(int) motor2.pid_speed.target);
	}
	


	while(1)
    {
		loop ++;
		if(loop == 600)
		{
			loop = 0;
			switch(order)
			{
				case GROUP1_INC : (MOTOR+0)->pid_angle.target += sen;
								  (MOTOR+3)->pid_angle.target += sen;
								  break;
				case GROUP1_DEC : (MOTOR+0)->pid_angle.target -= sen;
								  (MOTOR+3)->pid_angle.target -= sen;
								  break;
				case GROUP2_INC : (MOTOR+1)->pid_angle.target += sen;
								  (MOTOR+4)->pid_angle.target += sen;
								  break;
				case GROUP2_DEC : (MOTOR+1)->pid_angle.target -= sen;
								  (MOTOR+4)->pid_angle.target -= sen;
								  break;
				case GROUP3_INC : (MOTOR+2)->pid_angle.target += sen;
								  (MOTOR+5)->pid_angle.target += sen;
								  break;
				case GROUP3_DEC : (MOTOR+2)->pid_angle.target -= sen;
								  (MOTOR+5)->pid_angle.target -= sen;
								  break;
				default:          break;
			}
		}
		
		if(Pid_cal_Task == TASK_ENABLE)
		{
			SET_CLK(PID_TASK_CLK);           //每次计数至0时会关闭时钟,继续使用要打开
			Pid_cal_Task = 5;    
			
			Single_MOTOR_Angle_Task(MOTOR);
			Single_MOTOR_Angle_Task(MOTOR+1);
			Single_MOTOR_Angle_Task(MOTOR+2);
			Single_MOTOR_Angle_Task(MOTOR+3);
			Single_MOTOR_Angle_Task(MOTOR+4);
			Single_MOTOR_Angle_Task(MOTOR+5);
								
			Set_Pwm(Send_F401,MOTOR);

//			printf("%.0f  %.0f  %.0f  %.0f  %.0f  %.0f  %d\r\n",MOTOR->pid_angle.target,(MOTOR+1)->pid_angle.target,
//			(MOTOR+2)->pid_angle.target,(MOTOR+3)->pid_angle.target,(MOTOR+4)->pid_angle.target,(MOTOR+5)->pid_angle.target,order);
//			ANODT_Send_F1((int)MOTOR[num].pid_angle.target, MOTOR[num].pid_angle.measure);

		}	
	}
}




		
