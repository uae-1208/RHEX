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
bool STATION_CHANGE_CLK;
u16 Station_change_Task = 200;   

u8 order;        //蓝牙命令


//经测试，电机最大转速为35编码器值/5ms
int main(void)
{
	Motor_t motor1,motor2,motor3,motor4,motor5,motor6;
	u8 blue_flag = 0;
	u8 white_flag = 0;
	short Send_F401[8]={'P',0,0,0,0,0,0,'E'};
	enum Task_flag_t Blue_Task_flag = angle_pid_Task;        //任务标签,判断是应该进行速度环还是角度环
	enum Task_flag_t White_Task_flag= angle_pid_Task;        //任务标签,判断是应该进行速度环还是角度环
	enum Move_station_t Move_station = station1;

	System_Init(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);

	
	//初始化任务
	while(1)
	{
		static u16 delay_time = 0;
		static u8 delay_flag = 0;
		
		if(Pid_cal_Task == TASK_ENABLE)
        {
			SET_CLK(PID_TASK_CLK);           
			Pid_cal_Task = 5; 

			if(delay_flag)
				delay_time ++;
			if(delay_time == 400)
				break;
			
			MOTOR_Angle_Task(&motor1,&motor3,&motor5);
			MOTOR_Angle_Task(&motor2,&motor4,&motor6);

			Limit_Init_Pwm(motor1.pwm_out);
			Limit_Init_Pwm(motor2.pwm_out);
			Limit_Init_Pwm(motor3.pwm_out);
			Limit_Init_Pwm(motor4.pwm_out);
			Limit_Init_Pwm(motor5.pwm_out);
			Limit_Init_Pwm(motor6.pwm_out);
			
			
			Set_Pwm(Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);

			if(	   abs((int)(motor1.pid_angle.target) - motor1.angle_accmul) < 50
				&& abs((int)(motor2.pid_angle.target) - motor2.angle_accmul) < 50
				&& abs((int)(motor3.pid_angle.target) - motor3.angle_accmul) < 50
				&& abs((int)(motor4.pid_angle.target) - motor4.angle_accmul) < 50
				&& abs((int)(motor5.pid_angle.target) - motor5.angle_accmul) < 50
				&& abs((int)(motor6.pid_angle.target) - motor6.angle_accmul) < 50 ) 
			{
				delay_flag = 1;
			}    
		}
//		ANODT_Send_F1((int)motor1.pid_speed.target, motor1.pid_speed.measure);
//		ANODT_Send_F1((int)motor1.pid_angle.measure, motor1.pid_angle.target);
//		ANODT_Send_F2((int)motor1.pid_speed.target, motor1.pid_speed.measure,motor2.pid_speed.measure,(int) motor2.pid_speed.target);
	}
	
		
 
	while(1)
    {
		if(Pid_cal_Task == TASK_ENABLE)
		{
			SET_CLK(PID_TASK_CLK);           //每次计数至0时会关闭时钟,继续使用要打开
			Pid_cal_Task = 5;    
			
			White_Task_flag = White_Judge_task(&motor1,white_flag,Move_station);          //随便在白组中选一个电机即可
			Blue_Task_flag = Blue_Judge_task(&motor2,blue_flag,Move_station);             //随便在蓝组中选一个电机即可
			
			/***********************************************************白电机组********************************************************/
			if(White_Task_flag == speed_pid_Task)               //白组尚未转到缓冲区,所以执行速度环
			{
				if(Move_station == station1)                    //状态1下,白组转小角度
				{
					motor1.pid_speed.target = 4.8;              //zhp
					motor3.pid_speed.target = 4.8;
					motor5.pid_speed.target = 4.8;
				}
				else                                            //状态2下,白组转大角度
				{
					motor1.pid_speed.target = 28;
					motor3.pid_speed.target = 28;
					motor5.pid_speed.target = 28;
				}
				MOTOR_Speed_Task(&motor1,&motor3,&motor5);
			}
			else                                              //白组转到缓冲区,执行角度环
			    MOTOR_Angle_Task(&motor1,&motor3,&motor5);
			/***************************************************************************************************************************/
	
			/***********************************************************蓝电机组********************************************************/
			if(Blue_Task_flag == speed_pid_Task)              //蓝组尚未转到缓冲区,所以执行速度环
			{
				if(Move_station == station1)                  //状态1下,蓝组转大角度
				{
					motor2.pid_speed.target = 28;
					motor4.pid_speed.target = 28;
					motor6.pid_speed.target = 28;
				}
				else                                          //状态2下,蓝组转小角度
				{
					motor2.pid_speed.target = 4.8;
					motor4.pid_speed.target = 4.8;
					motor6.pid_speed.target = 4.8;
				}
				MOTOR_Speed_Task(&motor2,&motor4,&motor6);
			}
			else                                              //蓝组转到缓冲区,执行角度环
			    MOTOR_Angle_Task(&motor2,&motor4,&motor6);
			/***************************************************************************************************************************/
			
			
			if(Blue_Task_flag == angle_pid_Task && White_Task_flag == angle_pid_Task)       //当白蓝两组都开始执行角度环任务时
			{
				if(white_flag == 0 && blue_flag == 0)                                       //此处2个标志位的作用是只开一次时钟
					SET_CLK(STATION_CHANGE_CLK);     //打开状态切换时钟
				white_flag = 1;
				blue_flag = 1;
            }
			
			printf("%d\r\n",Station_change_Task);
			if(Station_change_Task == TASK_ENABLE)
            {    
				if(order == FORWARD)
				{
					Station_change_Task = 200;       //计时0.2s

					if(Move_station == station1)
					{
						motor1.pid_angle.target += angle_To_encoder(320);
						motor3.pid_angle.target += angle_To_encoder(320);
						motor5.pid_angle.target += angle_To_encoder(320);

						motor2.pid_angle.target += angle_To_encoder(40.5);		//1:40.5 2:40.5 3:40.5 5:38 6:40.5		
						motor4.pid_angle.target += angle_To_encoder(40.5);				
						motor6.pid_angle.target += angle_To_encoder(40);
						
						Move_station = station2;
					}
					else
					{
						motor1.pid_angle.target += angle_To_encoder(40.5);		//1:40.5 2:40.5 3:40.5 5:38 6:40.5		
						motor3.pid_angle.target += angle_To_encoder(40.5);				
						motor5.pid_angle.target += angle_To_encoder(40);
						
						motor2.pid_angle.target += angle_To_encoder(320);
						motor4.pid_angle.target += angle_To_encoder(320);
						motor6.pid_angle.target += angle_To_encoder(320);
						
						Move_station = station1;
					}		
					white_flag = 0;
					blue_flag = 0;
				}				
			}
			
			Set_Pwm(Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
				
//			ANODT_Send_F1(motor1.pid_speed.target, motor1.pid_speed.measure);
//			ANODT_Send_F1(motor1.pid_angle.measure, motor1.pid_angle.target);
//			ANODT_Send_F2(motor1.pid_speed.target, motor1.pid_speed.measure,motor2.pid_speed.measure, motor2.pid_speed.target);

		}	
	}
}




		
