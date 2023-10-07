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

bool PID_TASK_CLK;            //PID_TASK����ʱ��,����ʱPID_TASK�Ż���������ʱ
u16 Pid_cal_Task = 5;         //pid��������5ms
bool STATION_CHANGE_CLK;
u16 Station_change_Task = 200;   

u8 order;        //��������


//�����ԣ�������ת��Ϊ35������ֵ/5ms
int main(void)
{
	Motor_t motor1,motor2,motor3,motor4,motor5,motor6;
	u8 blue_flag = 0;
	u8 white_flag = 0;
	short Send_F401[8]={'P',0,0,0,0,0,0,'E'};
	enum Task_flag_t Blue_Task_flag = angle_pid_Task;        //�����ǩ,�ж���Ӧ�ý����ٶȻ����ǽǶȻ�
	enum Task_flag_t White_Task_flag= angle_pid_Task;        //�����ǩ,�ж���Ӧ�ý����ٶȻ����ǽǶȻ�
	enum Move_station_t Move_station = station1;

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
			
			printf("%d\r\n",Station_change_Task);
			if(Station_change_Task == TASK_ENABLE)
            {    
				if(order == FORWARD)
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
			}
			
			Set_Pwm(Send_F401,&motor1,&motor2,&motor3,&motor4,&motor5,&motor6);
				
//			ANODT_Send_F1(motor1.pid_speed.target, motor1.pid_speed.measure);
//			ANODT_Send_F1(motor1.pid_angle.measure, motor1.pid_angle.target);
//			ANODT_Send_F2(motor1.pid_speed.target, motor1.pid_speed.measure,motor2.pid_speed.measure, motor2.pid_speed.target);

		}	
	}
}




		
