#include "motor.h"
#include "usart.h"
#include "encoder.h"
#include "math.h"

#define TIM_period 4200
//限制PWM占空比≤100%	
#define LimitPwm(input)        			\
    {                          			\
        if (input > TIM_period)         \
        {                      			\
            input = TIM_period;         \
        }                       	    \
        else if (input < -TIM_period)   \
        {                      			\
            input = -TIM_period;        \
        }                      			\
    }
//电机编号初始化	
#define MOTOR_num_Init()	      \
{                                 \
	motor1->motor_num = 1;        \
	motor2->motor_num = 2;        \
	motor3->motor_num = 3;        \
	motor4->motor_num = 4;        \
	motor5->motor_num = 5;        \
	motor6->motor_num = 6;        \
}	


static void TIM3_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTA、PORTB时钟	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOA6复用为定时器3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA5	 
	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOA7复用为定时器3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOA7
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA7	 
		
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); //GPIOB0复用为定时器3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOB0
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB0	 

		
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); //GPIOB1复用为定时器3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOB1
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB1	 

}



static void TIM3_OC_Mode_Init(void)
{		 					 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
	 
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //定时器分频                42M/1/4200=10kHZ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=TIM_period-1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器3
	
	//初始化TIM3 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);	

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3										  
}

void MOTOR_TIM_Init(void)	
{
	TIM3_GPIO_Init();
	TIM3_OC_Mode_Init();
}





void MOTOR_Init(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
{
	//电机编号初始化
	MOTOR_num_Init();
	
	//电机pid参数初始化
	//motor1
	Pid_Init(&(motor1->pid_speed),motor1_speed_kp,motor1_speed_ki,motor1_speed_kd,
			motor1_speed_dead_band,motor1_speed_outmax,motor1_speed_integral_max);     
	Pid_Init(&(motor1->pid_angle),motor1_angle_kp,motor1_angle_ki,motor1_angle_kd,
			motor1_angle_dead_band,motor1_angle_outmax,motor1_angle_integral_max);     

	
	//motor2
	Pid_Init(&(motor2->pid_speed),motor2_speed_kp,motor2_speed_ki,motor2_speed_kd,
			motor2_speed_dead_band,motor2_speed_outmax,motor2_speed_integral_max);     
	Pid_Init(&(motor2->pid_angle),motor2_angle_kp,motor2_angle_ki,motor2_angle_kd,
			motor2_angle_dead_band,motor2_angle_outmax,motor2_angle_integral_max);     
	
	
	//motor3
	Pid_Init(&(motor3->pid_speed),motor3_speed_kp,motor3_speed_ki,motor3_speed_kd,
			motor3_speed_dead_band,motor3_speed_outmax,motor3_speed_integral_max);     
	Pid_Init(&(motor3->pid_angle),motor3_angle_kp,motor3_angle_ki,motor3_angle_kd,
			motor3_angle_dead_band,motor3_angle_outmax,motor3_angle_integral_max);     
	
	
	//motor4
	Pid_Init(&(motor4->pid_speed),motor4_speed_kp,motor4_speed_ki,motor4_speed_kd,
			motor4_speed_dead_band,motor4_speed_outmax,motor4_speed_integral_max);     
	Pid_Init(&(motor4->pid_angle),motor4_angle_kp,motor4_angle_ki,motor4_angle_kd,
			motor4_angle_dead_band,motor4_angle_outmax,motor4_angle_integral_max);     
	
	
	//motor5
	Pid_Init(&(motor5->pid_speed),motor5_speed_kp,motor5_speed_ki,motor5_speed_kd,
			motor5_speed_dead_band,motor5_speed_outmax,motor5_speed_integral_max);     
	Pid_Init(&(motor5->pid_angle),motor5_angle_kp,motor5_angle_ki,motor5_angle_kd,
			motor5_angle_dead_band,motor5_angle_outmax,motor5_angle_integral_max);     
	
	
	//motor6
	Pid_Init(&(motor6->pid_speed),motor6_speed_kp,motor6_speed_ki,motor6_speed_kd,
			motor6_speed_dead_band,motor6_speed_outmax,motor6_speed_integral_max);     
	Pid_Init(&(motor6->pid_angle),motor6_angle_kp,motor6_angle_ki,motor6_angle_kd,
			motor6_angle_dead_band,motor6_angle_outmax,motor6_angle_integral_max);     				
}


static void MOTOR_UPDATE(Motor_t* motor)	
{   
	short enc_val;
	enc_val = ENCODER_READ(motor->motor_num);
	
	motor->angle_accmul += enc_val;
	motor->angle = motor->angle_accmul % Motor_Prescaler;
	motor->pid_angle.measure = motor->angle_accmul;
	motor->pid_speed.measure = enc_val;
}


////速度环控制:速度环目标为正则输出为正;速度环输出正负决定PWM正负,PWM正负决定转动方向。
//static void MOTOR_Speed_Loop(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
//{
//	Pid_Calulate(&(motor1->pid_speed));
//	Pid_Calulate(&(motor2->pid_speed));
//	Pid_Calulate(&(motor3->pid_speed));
//	Pid_Calulate(&(motor4->pid_speed));
//	Pid_Calulate(&(motor5->pid_speed));
//	Pid_Calulate(&(motor6->pid_speed));

//	
//	motor1->pwm_out = motor1->pid_speed.out;    
//	motor2->pwm_out = motor2->pid_speed.out;       
//	motor3->pwm_out = motor3->pid_speed.out;      
//	motor4->pwm_out = motor4->pid_speed.out;       
//	motor5->pwm_out = motor5->pid_speed.out;       
//	motor6->pwm_out = motor6->pid_speed.out;       
//	

//	LimitPwm(motor1->pwm_out);
//	LimitPwm(motor2->pwm_out);
//	LimitPwm(motor3->pwm_out);
//	LimitPwm(motor4->pwm_out);
//	LimitPwm(motor5->pwm_out);
//	LimitPwm(motor6->pwm_out);
//}



////角度环控制:总角度误差为正时输出给速度环的值为正
//static void MOTOR_Angle_Loop(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
//{
//	Pid_Calulate(&(motor1->pid_angle));			       //角度环计算
//	Pid_Calulate(&(motor2->pid_angle));			      
//	Pid_Calulate(&(motor3->pid_angle));			      
//	Pid_Calulate(&(motor4->pid_angle));			       
//	Pid_Calulate(&(motor5->pid_angle));			       
//	Pid_Calulate(&(motor6->pid_angle));			       

//	motor1->pid_speed.target = motor1->pid_angle.out;
//	motor2->pid_speed.target = motor2->pid_angle.out;
//	motor3->pid_speed.target = motor3->pid_angle.out;
//	motor4->pid_speed.target = motor4->pid_angle.out;
//	motor5->pid_speed.target = motor5->pid_angle.out;
//	motor6->pid_speed.target = motor6->pid_angle.out;
//	
//	MOTOR_Speed_Loop(motor1,motor2,motor3,motor4,motor5,motor6);
//}


//void MOTOR_Speed_Task(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
//{
//	MOTOR_UPDATE(motor1);                          //电机数据更新
//	MOTOR_UPDATE(motor2);                           
//	MOTOR_UPDATE(motor3);                         
//	MOTOR_UPDATE(motor4);                        
//	MOTOR_UPDATE(motor5);                         
//	MOTOR_UPDATE(motor6); 	
//	
//	MOTOR_Speed_Loop(motor1,motor2,motor3,motor4,motor5,motor6);
//}

//void MOTOR_Angle_Task(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
//{
//	MOTOR_UPDATE(motor1);                          //电机数据更新
//	MOTOR_UPDATE(motor2);                           
//	MOTOR_UPDATE(motor3);                         
//	MOTOR_UPDATE(motor4);                        
//	MOTOR_UPDATE(motor5);                         
//	MOTOR_UPDATE(motor6);	
//	
//	MOTOR_Angle_Loop(motor1,motor2,motor3,motor4,motor5,motor6);
//}




//速度环控制:速度环目标为正则输出为正;速度环输出正负决定PWM正负,PWM正负决定转动方向。
static void MOTOR_Speed_Loop(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
{
	Pid_Calulate(&(motor1->pid_speed));
	Pid_Calulate(&(motor2->pid_speed));
	Pid_Calulate(&(motor3->pid_speed));


	
	motor1->pwm_out = motor1->pid_speed.out;    
	motor2->pwm_out = motor2->pid_speed.out;       
	motor3->pwm_out = motor3->pid_speed.out;      
      
	

	LimitPwm(motor1->pwm_out);
	LimitPwm(motor2->pwm_out);
	LimitPwm(motor3->pwm_out);

}



//角度环控制:总角度误差为正时输出给速度环的值为正
static void MOTOR_Angle_Loop(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
{
	Pid_Calulate(&(motor1->pid_angle));			       //角度环计算
	Pid_Calulate(&(motor2->pid_angle));			      
	Pid_Calulate(&(motor3->pid_angle));			      
		       

	motor1->pid_speed.target = motor1->pid_angle.out;
	motor2->pid_speed.target = motor2->pid_angle.out;
	motor3->pid_speed.target = motor3->pid_angle.out;

	
	MOTOR_Speed_Loop(motor1,motor2,motor3,motor4,motor5,motor6);
}


void MOTOR_Speed_Task(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
{
	MOTOR_UPDATE(motor1);                          //电机数据更新
	MOTOR_UPDATE(motor2);                           
	MOTOR_UPDATE(motor3);                         
	
	
	MOTOR_Speed_Loop(motor1,motor2,motor3,motor4,motor5,motor6);
}

void MOTOR_Angle_Task(Motor_t* motor1,Motor_t* motor2,Motor_t* motor3,Motor_t* motor4,Motor_t* motor5,Motor_t* motor6)
{
	MOTOR_UPDATE(motor1);                          //电机数据更新
	MOTOR_UPDATE(motor2);                           
	MOTOR_UPDATE(motor3);                         

	
	MOTOR_Angle_Loop(motor1,motor2,motor3,motor4,motor5,motor6);
}