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

#define get_white_buffer(station)    (((station)==station1) ? 50 : 200)
#define get_blue_buffer(station)    (((station)==station1) ? 200 : 50)
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


enum Task_flag_t White_Judge_task(Motor_t* motor, u8* flag, enum Move_station_t station);    
enum Task_flag_t Blue_Judge_task(Motor_t* motor, u8* flag, enum Move_station_t station);    



//经测试，电机最大转速为35编码器值/5ms
int main(void)
{
	Motor_t motor1,motor2,motor3,motor4,motor5,motor6;
	u8 blue_flag = 0;
	u8 white_flag = 0;
	short Send_F401[8]={'P',0,0,0,0,0,0,'E'};
	enum Task_flag_t Blue_Task_flag;        //任务标签,判断是应该进行速度环还是角度环
	enum Task_flag_t White_Task_flag;        //任务标签,判断是应该进行速度环还是角度环
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
	
	motor1.pid_angle.target = angle_To_encoder(320);
	motor3.pid_angle.target = angle_To_encoder(320);
	motor5.pid_angle.target = angle_To_encoder(320);

	motor2.pid_angle.target = angle_To_encoder(40.5);
	motor4.pid_angle.target = angle_To_encoder(40);
	motor6.pid_angle.target = angle_To_encoder(40.5);
	
	while(1)
    {
		if(Pid_cal_Task == TASK_ENABLE)
		{
			SET_CLK(PID_TASK_CLK);           //每次计数至0时会关闭时钟,使用要打开
			Pid_cal_Task = 5;    
			
			
			White_Task_flag = White_Judge_task(&motor1,&white_flag,Move_station);          //随便在白组中选一个电机即可
			Blue_Task_flag = Blue_Judge_task(&motor2,&blue_flag,Move_station);             //随便在蓝组中选一个电机即可
			
			if(White_Task_flag == speed_pid_Task)               //白组尚未转到缓冲区,所以执行速度环
			{
				if(Move_station == station1)                    //状态1下,白组转小角度
				{
					motor1.pid_speed.target = 3;
					motor3.pid_speed.target = 3;
					motor5.pid_speed.target = 3;
				}
				else                                            //状态2下,白组转大角度
				{
					motor1.pid_speed.target = 28;
					motor3.pid_speed.target = 28;
					motor5.pid_speed.target = 28;
				}
				MOTOR_Speed_Task(&motor1,&motor3,&motor5,&motor4,&motor5,&motor6);
			}
			else                                              //白组转到缓冲区,执行角度环
			    MOTOR_Angle_Task(&motor1,&motor3,&motor5,&motor4,&motor5,&motor6);
	
			
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
					motor2.pid_speed.target = 3;
					motor4.pid_speed.target = 3;
					motor6.pid_speed.target = 3;
				}
				MOTOR_Speed_Task(&motor2,&motor4,&motor6,&motor4,&motor5,&motor6);
			}
			else                                              //蓝组转到缓冲区,执行角度环
			    MOTOR_Angle_Task(&motor2,&motor4,&motor6,&motor4,&motor5,&motor6);
			
			
			if(Blue_Task_flag == angle_pid_Task && White_Task_flag == angle_pid_Task)       //当白蓝两组都开始执行角度环任务时
			{
				if(white_flag == 0 && blue_flag == 0)                                       //此处2个标志位的作用是只开一次时钟
					SET_CLK(STATION_CHANGE_CLK);    //打开状态切换时钟
				white_flag = 1;
				blue_flag = 1;
            }
			
			if(Station_change_Task == TASK_ENABLE)
            {    
				Station_change_Task = 500;       //计时0.5s
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
			
			
			Set_Pwm(Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
				
//			printf("%d  %d  %d \r\n",motor5.pid_angle.measure,motor3.pid_angle.measure,motor1.pid_angle.measure);
//			ANODT_Send_F1(motor4.pid_speed.target, motor4.pid_speed.measure);
//			ANODT_Send_F1(motor2.pid_angle.measure, motor2.pid_angle.target);
		}	
	}
}


enum Task_flag_t White_Judge_task(Motor_t* motor, u8* flag, enum Move_station_t station)
{
	enum Task_flag_t Task_flag;
	//在距离目标值较远时,进行速度环任务,使电机恒速转动;接近目标值时,转变为角度环稳定在目标值;这个接近值就是缓冲区buffer
	if(abs(motor->pid_angle.target - motor->angle_accmul) > get_white_buffer(station))       
	{
		if(*flag == 0)
			Task_flag = speed_pid_Task;
	}
	else 
		//角度环有可能超调,当电机超过目标值buffer后就会变回速度环,然后一直无法到达目标值;所以设置flag,使电机转变为角度环之后不会再变回速度环
		Task_flag = angle_pid_Task;
	
	return Task_flag;
}


enum Task_flag_t Blue_Judge_task(Motor_t* motor, u8* flag, enum Move_station_t station)
{
	enum Task_flag_t Task_flag;
	//在距离目标值较远时,进行速度环任务,使电机恒速转动;接近目标值时,转变为角度环稳定在目标值;这个接近值就是缓冲区buffer
	if(abs(motor->pid_angle.target - motor->angle_accmul) > get_blue_buffer(station))       
	{
		if(*flag == 0)
			Task_flag = speed_pid_Task;
	}
	else 
		//角度环有可能超调,当电机超过目标值buffer后就会变回速度环,然后一直无法到达目标值;所以设置flag,使电机转变为角度环之后不会再变回速度环
		Task_flag = angle_pid_Task;
	
	return Task_flag;
}

		
