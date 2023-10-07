#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f4xx.h"
#include "pid.h"

#define Motor_Prescaler      1560//平衡小车使用的电机  //5764  //RHEX使用的电机编码器分辨率 11*131*4

/*******************************************************/
#define motor1_speed_kp 		       7     //zhp
#define motor1_speed_ki			       0
#define motor1_speed_kd 			   0.1
#define motor1_speed_dead_band 	       0
#define motor1_speed_outmax 		   4200
#define motor1_speed_integral_max      1000

#define motor1_angle_kp 			   100    //zhp
#define motor1_angle_ki 			   0
#define motor1_angle_kd 			   0
#define motor1_angle_dead_band 	       10
#define motor1_angle_outmax 		   1000
#define motor1_angle_integral_max      1000

/*******************************************************/
#define motor2_speed_kp 		       50     //zhp
#define motor2_speed_ki			       0
#define motor2_speed_kd 			   0
#define motor2_speed_dead_band 	       0
#define motor2_speed_outmax 		   4200
#define motor2_speed_integral_max      1000

#define motor2_angle_kp 			   100    //zhp
#define motor2_angle_ki 			   0
#define motor2_angle_kd 			   0
#define motor2_angle_dead_band 	       10
#define motor2_angle_outmax 		   1000
#define motor2_angle_integral_max      1000

/*******************************************************/
#define motor3_speed_kp 		       50     //zhp
#define motor3_speed_ki			       0
#define motor3_speed_kd 			   0
#define motor3_speed_dead_band 	       0
#define motor3_speed_outmax 		   4200
#define motor3_speed_integral_max      1000

#define motor3_angle_kp 			   100    //zhp
#define motor3_angle_ki 			   0
#define motor3_angle_kd 			   0
#define motor3_angle_dead_band 	       10
#define motor3_angle_outmax 		   1000
#define motor3_angle_integral_max      1000

/*******************************************************/
#define motor4_speed_kp 		       50     //zhp
#define motor4_speed_ki			       0
#define motor4_speed_kd 			   0
#define motor4_speed_dead_band 	       0
#define motor4_speed_outmax 		   4200
#define motor4_speed_integral_max      1000

#define motor4_angle_kp 			   100    //zhp
#define motor4_angle_ki 			   0
#define motor4_angle_kd 			   0
#define motor4_angle_dead_band 	       10
#define motor4_angle_outmax 		   1000
#define motor4_angle_integral_max      1000

/*******************************************************/
#define motor5_speed_kp 		       50     //zhp
#define motor5_speed_ki			       0
#define motor5_speed_kd 			   0
#define motor5_speed_dead_band 	       0
#define motor5_speed_outmax 		   4200
#define motor5_speed_integral_max      1000

#define motor5_angle_kp 			   100    //zhp
#define motor5_angle_ki 			   0
#define motor5_angle_kd 			   0
#define motor5_angle_dead_band 	       10
#define motor5_angle_outmax 		   1000
#define motor5_angle_integral_max      1000

/*******************************************************/
#define motor6_speed_kp 		       50     //zhp
#define motor6_speed_ki			       0
#define motor6_speed_kd 			   0
#define motor6_speed_dead_band 	       0
#define motor6_speed_outmax 		   4200
#define motor6_speed_integral_max      1000

#define motor6_angle_kp 			   100    //zhp
#define motor6_angle_ki 			   0
#define motor6_angle_kd 			   0
#define motor6_angle_dead_band 	       10
#define motor6_angle_outmax 		   1000
#define motor6_angle_integral_max      1000


typedef struct __motor_t
{
	short encoder_overflow_count;    //编码器溢出计数
	int angle_accmul;  			    //累计转过的总角度
	short angle;   		 			 //0-Motor_Prescaler的角度
	float pwm_out;
	u8 motor_num;  		 			 //电机编号,1-6,用来判断读取哪个编码器TIM
	
	Pid_t pid_speed;
	Pid_t pid_angle;
}Motor_t;


void MOTOR_TIM_Init(void);
void MOTOR_Init(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6);
void MOTOR_Speed_Task(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6);
void MOTOR_Angle_Task(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6);

#endif
