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

bool PID_TASK_CLK;            //PID_TASK����ʱ��,����ʱPID_TASK�Ż���������ʱ
u16 Pid_cal_Task = 5;         //pid��������5ms
bool STATION_CHANGE_CLK;
u16 Station_change_Task = 200;      

#define FORWARD 1
#define STOP    0


#define get_white_buffer(station)    (((station)==station1) ? 70 : 200)
#define get_blue_buffer(station)    (((station)==station1) ? 200 : 70)
#define angle_To_encoder(angle) ((short)((angle)*Motor_Prescaler/360))
#define Limit_Init_Pwm(input)        	\
    {                          			\
        if (input > 3600)         		\
        {                      			\
            input = 3600;         		\
        }                       	    \
        else if (input < -3600)   		\
        {                      			\
            input = -3600;        		\
        }                      			\
    }
enum Task_flag_t
{
	speed_pid_Task,     //�ٶȻ�����
	angle_pid_Task	    //�ǶȻ�����
};

enum Move_station_t
{
	station1,     //���2��4��6ת320��
	station2	  //���2��4��6ת40��
};

u8 order;

enum Task_flag_t White_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station);    
enum Task_flag_t Blue_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station);    
void System_Init(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6);




//�����ԣ�������ת��Ϊ35������ֵ/5ms
int main(void)
{
	Motor_t motor1,motor2,motor3,motor4,motor5,motor6;
	u8 blue_flag = 0;
	u8 white_flag = 0;
	short Send_F401[8]={'P',0,0,0,0,0,0,'E'};
	enum Task_flag_t Blue_Task_flag;         //�����ǩ,�ж���Ӧ�ý����ٶȻ����ǽǶȻ�
	enum Task_flag_t White_Task_flag;        //�����ǩ,�ж���Ӧ�ý����ٶȻ����ǽǶȻ�
	enum Move_station_t Move_station = station2;

	System_Init(&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);

	
	//��ʼ������
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
	
		
	motor1.pid_angle.target += angle_To_encoder(320);
	motor3.pid_angle.target += angle_To_encoder(320);
	motor5.pid_angle.target += angle_To_encoder(320);

	motor2.pid_angle.target += angle_To_encoder(40);
	motor4.pid_angle.target += angle_To_encoder(40);
	motor6.pid_angle.target += angle_To_encoder(40);
	    
	
	while(1)
    {
		if(Pid_cal_Task == TASK_ENABLE)
		{
			SET_CLK(PID_TASK_CLK);           //ÿ�μ�����0ʱ��ر�ʱ��,����ʹ��Ҫ��
			Pid_cal_Task = 5;    
			
			White_Task_flag = White_Judge_task(&motor1,white_flag,Move_station);          //����ڰ�����ѡһ���������
			Blue_Task_flag = Blue_Judge_task(&motor2,blue_flag,Move_station);             //�����������ѡһ���������
			
			/***********************************************************�׵����********************************************************/
			if(White_Task_flag == speed_pid_Task)               //������δת��������,����ִ���ٶȻ�
			{
				if(Move_station == station1)                    //״̬1��,����תС�Ƕ�
				{
					motor1.pid_speed.target = 4.8;              //zhp
					motor3.pid_speed.target = 4.8;
					motor5.pid_speed.target = 4.8;
				}
				else                                            //״̬2��,����ת��Ƕ�
				{
					motor1.pid_speed.target = 28;
					motor3.pid_speed.target = 28;
					motor5.pid_speed.target = 28;
				}
				MOTOR_Speed_Task(&motor1,&motor3,&motor5);
			}
			else                                              //����ת��������,ִ�нǶȻ�
			    MOTOR_Angle_Task(&motor1,&motor3,&motor5);
			/***************************************************************************************************************************/
	
			/***********************************************************�������********************************************************/
			if(Blue_Task_flag == speed_pid_Task)              //������δת��������,����ִ���ٶȻ�
			{
				if(Move_station == station1)                  //״̬1��,����ת��Ƕ�
				{
					motor2.pid_speed.target = 28;
					motor4.pid_speed.target = 28;
					motor6.pid_speed.target = 28;
				}
				else                                          //״̬2��,����תС�Ƕ�
				{
					motor2.pid_speed.target = 4.8;
					motor4.pid_speed.target = 4.8;
					motor6.pid_speed.target = 4.8;
				}
				MOTOR_Speed_Task(&motor2,&motor4,&motor6);
			}
			else                                              //����ת��������,ִ�нǶȻ�
			    MOTOR_Angle_Task(&motor2,&motor4,&motor6);
			/***************************************************************************************************************************/
			
			
			if(Blue_Task_flag == angle_pid_Task && White_Task_flag == angle_pid_Task)       //���������鶼��ʼִ�нǶȻ�����ʱ
			{
				if(white_flag == 0 && blue_flag == 0)                                       //�˴�2����־λ��������ֻ��һ��ʱ��
					SET_CLK(STATION_CHANGE_CLK);     //��״̬�л�ʱ��
				white_flag = 1;
				blue_flag = 1;
            }
			
			if(Station_change_Task == TASK_ENABLE)
            {    
				Station_change_Task = 200;       //��ʱ0.2s
				
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
				
//			ANODT_Send_F1(motor1.pid_speed.target, motor1.pid_speed.measure);
//			ANODT_Send_F1(motor1.pid_angle.measure, motor1.pid_angle.target);
//			ANODT_Send_F2(motor1.pid_speed.target, motor1.pid_speed.measure,motor2.pid_speed.measure, motor2.pid_speed.target);

		}	
	}
}



void System_Init(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
{
	SysTick_Init();
	USART_Config();
	BLUETOOTH_Config();
	TIMER_Init();
	MOTOR_TIM_Init();
	ENCODER_TIM_Init();	
	
	
	//�����ʼ��
	MOTOR_Init(motor1,motor2,motor3,motor4,motor5,motor6);	   
    MOTOR_Angle_Init(motor1,motor2,motor3,motor4,motor5,motor6);
	
	SET_CLK(PID_TASK_CLK);            //��pid�������ڼ�ʱclk
	RESET_CLK(STATION_CHANGE_CLK);    //�ر�״̬�л���ʱclk
	
}




enum Task_flag_t White_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station)
{
	enum Task_flag_t Task_flag;
	//�ھ���Ŀ��ֵ��Զʱ,�����ٶȻ�����,ʹ�������ת��;�ӽ�Ŀ��ֵʱ,ת��Ϊ�ǶȻ��ȶ���Ŀ��ֵ;����ӽ�ֵ���ǻ�����buffer
	if(abs((int)(motor->pid_angle.target) - motor->angle_accmul) > get_white_buffer(station))       
	{
		if(flag == 0)
			Task_flag = speed_pid_Task;
	}
	else 
		//�ǶȻ��п��ܳ���,���������Ŀ��ֵbuffer��ͻ����ٶȻ�,Ȼ��һֱ�޷�����Ŀ��ֵ;��������flag,ʹ���ת��Ϊ�ǶȻ�֮�󲻻��ٱ���ٶȻ�
		Task_flag = angle_pid_Task;
	
	return Task_flag;
}


enum Task_flag_t Blue_Judge_task(Motor_t* motor, u8 flag, enum Move_station_t station)
{
	enum Task_flag_t Task_flag;
	//�ھ���Ŀ��ֵ��Զʱ,�����ٶȻ�����,ʹ�������ת��;�ӽ�Ŀ��ֵʱ,ת��Ϊ�ǶȻ��ȶ���Ŀ��ֵ;����ӽ�ֵ���ǻ�����buffer
	if(abs((int)(motor->pid_angle.target) - motor->angle_accmul) > get_blue_buffer(station))       
	{
		if(flag == 0)
			Task_flag = speed_pid_Task;
	}
	else 
		//�ǶȻ��п��ܳ���,���������Ŀ��ֵbuffer��ͻ����ٶȻ�,Ȼ��һֱ�޷�����Ŀ��ֵ;��������flag,ʹ���ת��Ϊ�ǶȻ�֮�󲻻��ٱ���ٶȻ�
		Task_flag = angle_pid_Task;
	
	return Task_flag;
}

		
