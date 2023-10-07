#include "motor.h"
#include "usart.h"
#include "encoder.h"
#include "math.h"

#define TIM_period 4200
//����PWMռ�ձȡ�100%	
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
//�����ų�ʼ��	
#define MOTOR_num_Init()	      \
{                                 \
	motor[0].motor_num = 1;        \
	motor[1].motor_num = 2;        \
	motor[2].motor_num = 3;        \
	motor[3].motor_num = 4;        \
	motor[4].motor_num = 5;        \
	motor[5].motor_num = 6;        \
}	


static void TIM3_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTA��PORTBʱ��	
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOA6����Ϊ��ʱ��3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA5	 
	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); //GPIOA7����Ϊ��ʱ��3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOA7
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA7	 
		
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); //GPIOB0����Ϊ��ʱ��3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOB0
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB0	 

		
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); //GPIOB1����Ϊ��ʱ��3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOB1
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB1	 

}



static void TIM3_OC_Mode_Init(void)
{		 					 
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
	 
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //��ʱ����Ƶ                42M/1/4200=10kHZ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=TIM_period-1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��3
	
	//��ʼ��TIM3 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
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
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3										  
}

void MOTOR_TIM_Init(void)	
{
	TIM3_GPIO_Init();
	TIM3_OC_Mode_Init();
}





void MOTOR_Init(Motor_t* motor)
{
	
	//�����ų�ʼ��
	MOTOR_num_Init();
	
	//���pid������ʼ��
	//motor1
	Pid_Init(&(motor[0].pid_speed),motor1_speed_kp,motor1_speed_ki,motor1_speed_kd,
			motor1_speed_dead_band,motor1_speed_outmax,motor1_speed_integral_max);     
	Pid_Init(&(motor[0].pid_angle),motor1_angle_kp,motor1_angle_ki,motor1_angle_kd,
			motor1_angle_dead_band,motor1_angle_outmax,motor1_angle_integral_max);     

	
	//motor2
	Pid_Init(&(motor[1].pid_speed),motor2_speed_kp,motor2_speed_ki,motor2_speed_kd,
			motor2_speed_dead_band,motor2_speed_outmax,motor2_speed_integral_max);     
	Pid_Init(&(motor[1].pid_angle),motor2_angle_kp,motor2_angle_ki,motor2_angle_kd,
			motor2_angle_dead_band,motor2_angle_outmax,motor2_angle_integral_max);     
	
	
	//motor3
	Pid_Init(&(motor[2].pid_speed),motor3_speed_kp,motor3_speed_ki,motor3_speed_kd,
			motor3_speed_dead_band,motor3_speed_outmax,motor3_speed_integral_max);     
	Pid_Init(&(motor[2].pid_angle),motor3_angle_kp,motor3_angle_ki,motor3_angle_kd,
			motor3_angle_dead_band,motor3_angle_outmax,motor3_angle_integral_max);     
	
	
	//motor4
	Pid_Init(&(motor[3].pid_speed),motor4_speed_kp,motor4_speed_ki,motor4_speed_kd,
			motor4_speed_dead_band,motor4_speed_outmax,motor4_speed_integral_max);     
	Pid_Init(&(motor[3].pid_angle),motor4_angle_kp,motor4_angle_ki,motor4_angle_kd,
			motor4_angle_dead_band,motor4_angle_outmax,motor4_angle_integral_max);     
	
	
	//motor5
	Pid_Init(&(motor[4].pid_speed),motor5_speed_kp,motor5_speed_ki,motor5_speed_kd,
			motor5_speed_dead_band,motor5_speed_outmax,motor5_speed_integral_max);     
	Pid_Init(&(motor[4].pid_angle),motor5_angle_kp,motor5_angle_ki,motor5_angle_kd,
			motor5_angle_dead_band,motor5_angle_outmax,motor5_angle_integral_max);     
	
	
	//motor6
	Pid_Init(&(motor[5].pid_speed),motor6_speed_kp,motor6_speed_ki,motor6_speed_kd,
			motor6_speed_dead_band,motor6_speed_outmax,motor6_speed_integral_max);     
	Pid_Init(&(motor[5].pid_angle),motor6_angle_kp,motor6_angle_ki,motor6_angle_kd,
			motor6_angle_dead_band,motor6_angle_outmax,motor6_angle_integral_max);     				
}


#define angle_To_encoder(angle) ((short)((angle)*Motor_Prescaler/360))

void MOTOR_Angle_Init(Motor_t* motor)
{
	//��������ʱ�ı�����ֵ
	motor[0].angle_accmul = 1503;
	motor[1].angle_accmul = -1643;
	motor[2].angle_accmul = -1662;
	motor[3].angle_accmul = 1500 ;
	motor[4].angle_accmul = -1653;
	motor[5].angle_accmul = -1666;
	
//	motor[0].pid_angle.target = angle_To_encoder(30);
//	motor[2].pid_angle.target = angle_To_encoder(30);
//	motor[4].pid_angle.target = angle_To_encoder(30);

//	motor[1].pid_angle.target = angle_To_encoder(-10);
//	motor[3].pid_angle.target = angle_To_encoder(-10);
//	motor[5].pid_angle.target = angle_To_encoder(-10);
	motor[0].pid_angle.target = angle_To_encoder(0);
	motor[2].pid_angle.target = angle_To_encoder(0);
	motor[4].pid_angle.target = angle_To_encoder(0);

	motor[1].pid_angle.target = angle_To_encoder(-0);
	motor[3].pid_angle.target = angle_To_encoder(-0);
	motor[5].pid_angle.target = angle_To_encoder(-0);
	
}


#define kf  0.75
static void MOTOR_UPDATE(Motor_t* motor)	
{   
	short enc_val;
	enc_val = ENCODER_READ(motor->motor_num);
	
	motor->angle_accmul += enc_val;
	motor->speed = kf*enc_val + (1 - kf)*motor->last_speed;   //һ�׵�ͨ�˲�
	motor->pid_angle.measure = motor->angle_accmul;
	motor->pid_speed.measure = motor->speed;
	
	motor->last_speed = motor->speed;

}


//�ٶȻ�����:�ٶȻ�Ŀ��Ϊ�������Ϊ��;�ٶȻ������������PWM����,PWM��������ת������
static void MOTOR_Speed_Loop(Motor_t* motor)
{
	Pid_Calulate(&(motor->pid_speed));	
	motor->pwm_out = motor->pid_speed.out;    
	LimitPwm(motor->pwm_out);
}



//�ǶȻ�����:�ܽǶ����Ϊ��ʱ������ٶȻ���ֵΪ��
static void MOTOR_Angle_Loop(Motor_t* motor)
{
	Pid_Calulate(&(motor->pid_angle));			       //�ǶȻ�����
	motor->pid_speed.target = motor->pid_angle.out;
	MOTOR_Speed_Loop(motor);
}


void Single_MOTOR_Speed_Task(Motor_t* motor)
{
	MOTOR_UPDATE(motor);                          //������ݸ���	
	
	MOTOR_Speed_Loop(motor);
}

void Single_MOTOR_Angle_Task(Motor_t* motor)
{
	MOTOR_UPDATE(motor);                          //������ݸ���
	
	MOTOR_Angle_Loop(motor);
}