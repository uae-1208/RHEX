#include "sys.h"
#include "stm32f4xx.h"
#include "motor.h"
#include "encoder.h"
#include "usart.h"
#include "systick.h"
#include "stdlib.h"
#include "stdbool.h"
#include "tim.h"
#include "led.h"   

bool PID_TASK_CLK;            //PID_TASK虚拟时钟,开启时PID_TASK才会正常倒计时
u16 Pid_cal_Task = 5;         //pid计算周期5ms
bool STATION_CHANGE_CLK;
u16 Station_change_Task = 500;      

#define get_buffer(station)    (((station)==station1) ? 50 : 200)
#define angle_To_encoder(angle) ((short)((angle)*Motor_Prescaler/360))

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


enum Task_flag_t Judge_task(Motor_t* motor, u8* flag, enum Move_station_t station);    


#define MOTOR motor1
//经测试，电机最大转速为35编码器值/5ms
int main(void)
{
	Motor_t motor1,motor2,motor3,motor4,motor5,motor6;
	u8 flag = 0;
	short Send_F401[8]={'P',0,0,0,0,0,0,'E'};
	enum Task_flag_t Task_flag;        //任务标签,判断是应该进行速度环还是角度环
	enum Move_station_t Move_station = station2;

	SysTick_Init();
	USART_Config();
	BLUETOOTH_Config();
	TIMER_Init();
	MOTOR_TIM_Init();
	ENCODER_TIM_Init();
	
	//电机初始化
	MOTOR_Init(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);	   
    
	SET_CLK(PID_TASK_CLK);
	RESET_CLK(STATION_CHANGE_CLK);
	
	MOTOR.pid_angle.target = angle_To_encoder(320);


	while(1)
    {
		if(Pid_cal_Task == TASK_ENABLE)
		{
			SET_CLK(PID_TASK_CLK);           //每次计数至0时会关闭时钟,使用要打开
			Pid_cal_Task = 5;    
			
			
			Task_flag = Judge_task(&MOTOR,&flag,Move_station);          //这里随便选一个电机即可
			
			if(Task_flag == speed_pid_Task)
			{
				if(Move_station == station1)
					MOTOR.pid_speed.target = 3;
				else
					MOTOR.pid_speed.target = 28;
					
				MOTOR_Speed_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
			}
			else
			    MOTOR_Angle_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
	
			
			if(Station_change_Task == TASK_ENABLE)
            {    
				Station_change_Task = 500;      //计时0.5s
				if(Move_station == station1)
				{
					MOTOR.pid_angle.target += angle_To_encoder(320);
					Move_station = station2;
                }
				else
				{
					MOTOR.pid_angle.target += angle_To_encoder(40.5);		//1:40.5 2:40.5 3:40.5 5:38 6:40.5		
					Move_station = station1;
				}		
				flag = 0;
			}
			
			
			Set_Pwm(Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
					
//			ANODT_Send_F1(motor4.pid_speed.target, motor4.pid_speed.measure);
			ANODT_Send_F1(motor1.pid_angle.target, motor1.pid_angle.measure);
		}	
	}
}


enum Task_flag_t Judge_task(Motor_t* motor, u8* flag, enum Move_station_t station)
{
	enum Task_flag_t Task_flag;
	//在距离目标值较远时,进行速度环任务,使电机恒速转动;接近目标值时,转变为角度环稳定在目标值;这个接近值就是缓冲区buffer
	if(abs(motor->pid_angle.target - motor->angle_accmul) > get_buffer(station))       
	{
		if(*flag == 0)
			Task_flag = speed_pid_Task;
	}
	else 
	{
		//角度环有可能超调,当电机超过目标值buffer后就会变回速度环,然后一直无法到达目标值;所以设置flag,使电机转变为角度环之后不会再变回速度环
		Task_flag = angle_pid_Task;
		
		if(*flag == 0)
			SET_CLK(STATION_CHANGE_CLK);    //打开状态切换时钟
		*flag = 1;
	}	
	
	return Task_flag;
}



		
//		if(delay_task ==500)      //非阻塞延时3s
//		{
//			motor4.pid_speed.target = -motor4.pid_speed.target;            //速度反相
//			delay_task = 0;
//		}
//		
//		if(Speed_Pid_cal_Task == TASK_ENABLE)
//		{
//			Speed_Pid_cal_Task = 5;              
//			delay_task++;
//			MOTOR_Speed_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//								
//			Set_Pwm(Set_PWM_Value,Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//			
//			
//			ANODT_Send_F1(motor4.pid_speed.target, motor4.pid_speed.measure);
//			/*记得修改fputc*/
////			printf(" %d  %d  %.2f  %.2f \r\n",
////			motor2.pid_speed.target,motor2.pid_speed.measure,motor2.pid_speed.out, motor2.pwm_out);
//		}
		
		
		
//		if(delay_task ==500)      //非阻塞延时3s
//		{
//			motor4.pid_angle.target += 5600;            
//			delay_task = 0;
//		}
//		
//		
//		if(Speed_Pid_cal_Task == TASK_ENABLE)
//		{
//			Speed_Pid_cal_Task = 5;              
//			delay_task++;
//						
//			MOTOR_Angle_Task(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//			
//			Set_Pwm(Set_PWM_Value,Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
//			
//			ANODT_Send_F1(motor4.pid_angle.target, motor4.pid_angle.measure);
//			/*记得修改fputc*/
////			printf("%d  %.2f %d  %d  %.2f  %.2f \r\n",motor4.pid_angle.measure,motor4.pid_angle.out,
////			motor4.pid_speed.target,motor4.pid_speed.measure,motor4.pid_speed.out, motor4.pwm_out);
//		}



