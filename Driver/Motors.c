#include "Motors.h"
#include	"STC8H_PWM.h"
#include	"NVIC.h"
#include	"Switch.h"
#include "GPIO.h"

typedef struct {

	// -100 --------------- 0 -------------- 100
	int LQ_speed;
	int RQ_speed;
	int LH_speed;
	int RH_speed;
	
}MotorsCfg;

/************
[-100 --------------- 0 -------------- 100]
����Χ��[-100,100]�ٶ�ֵת�ɶ�Ӧռ�ձȣ���ֵ
 100  ---------------50 --------------- 0  duty

*************/
int speed2duty(int speed){
	// speed < 0 ���� eg. -60
	// speed > 0 ǰ�� eg. 50
	// speed ==0 ֹͣ
	
	// B_Max               stop             F_Max
	// -100 --------------- 0 -------------- 100 speed
	
	// [-100, 100] / 2 ==> [-50, 50]
	// [-50, 50] * -1  ==> [50, -50]
	// [50, -50] + 50  ==> [100, 0]
	
	// 100  ---------------50 --------------- 0  duty
	
	return -(speed / 2) + 50;
}

#define PERIOD ((MAIN_Fosc / 1000) - 1)
PWMx_Duty dutyA;
void	PWM_config(MotorsCfg cfg)
{
  PWMx_InitDefine		PWMx_InitStructure;
	
	// �����������ĸ����ӵ��ٶ�ת��dutyռ�ձ�
	int LQ_duty = speed2duty(cfg.LQ_speed);
	int RQ_duty = speed2duty(cfg.RQ_speed);
	int LH_duty = speed2duty(cfg.LH_speed);
	int RH_duty = speed2duty(cfg.RH_speed);
	
	u8 is_LQ_enable = (cfg.LQ_speed != 0); // �ٶȲ���0������Ҫѡ���Ӧ������
	u8 is_RQ_enable = (cfg.RQ_speed != 0); // �ٶȲ���0������Ҫѡ���Ӧ������
	u8 is_LH_enable = (cfg.LH_speed != 0); // �ٶȲ���0������Ҫѡ���Ӧ������
	u8 is_RH_enable = (cfg.RH_speed != 0); // �ٶȲ���0������Ҫѡ���Ӧ������
	// ����һ��PWM��enable��PWMA����
	u8 is_main_enable = is_LQ_enable || is_RQ_enable || is_LH_enable || is_RH_enable;

  // ����PWM1 M3 ����� LH
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * LH_duty / 100;	//PWMռ�ձ�ʱ��, 0~Period
  PWMx_InitStructure.PWM_EnoSelect	= is_LH_enable ? (ENO1P | ENO1N) : 0;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM1, &PWMx_InitStructure);			//��ʼ��PWM
  // ����PWM2 M4 �Һ��� RH
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * RH_duty / 100;	//PWMռ�ձ�ʱ��, 0~Period
  PWMx_InitStructure.PWM_EnoSelect  = is_RH_enable ? (ENO2P | ENO2N) : 0;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM2, &PWMx_InitStructure);			//��ʼ��PWM
  // ����PWM3 M1 ��ǰ�� LQ
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * LQ_duty / 100;	//PWMռ�ձ�ʱ��, 0~Period
  PWMx_InitStructure.PWM_EnoSelect  = is_LQ_enable ? (ENO3P | ENO3N) : 0;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM3, &PWMx_InitStructure);
  // ����PWM4 M2 ��ǰ�� RQ
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty    		= PERIOD * RQ_duty / 100;	//PWMռ�ձ�ʱ��, 0~Period
  PWMx_InitStructure.PWM_EnoSelect  = is_RQ_enable ? (ENO4P | ENO4N) : 0;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM4, &PWMx_InitStructure);

  // ����PWMA
  PWMx_InitStructure.PWM_Period   = PERIOD;			//����ʱ��,   0~65535
  PWMx_InitStructure.PWM_DeadTime = 0;					//��������������, 0~255
  PWMx_InitStructure.PWM_MainOutEnable= is_main_enable;			//�����ʹ��, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable   = is_main_enable;			//ʹ�ܼ�����, ENABLE,DISABLE
  PWM_Configuration(PWMA, &PWMx_InitStructure);			//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

  // �л�PWMͨ��
  PWM1_SW(PWM1_SW_P20_P21);			//PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
  PWM2_SW(PWM2_SW_P22_P23);			//PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
  PWM3_SW(PWM3_SW_P14_P15);			//PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
  PWM4_SW(PWM4_SW_P16_P17);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

  // ��ʼ��PWMA���ж�
  NVIC_PWM_Init(PWMA,DISABLE,Priority_0);
}
void Motors_init() {
	
	// ����
	P1_MODE_OUT_PP(GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	P2_MODE_OUT_PP(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	
}

// ǰ��
void Motors_forward(int speed) {
	// 0 |--------------| 50 |--------------| 100
	MotorsCfg cfg;
	
	printf("forward\r\n");
  cfg.LQ_speed = speed;
  cfg.RQ_speed = speed;
  cfg.LH_speed = speed;
  cfg.RH_speed = speed;
	PWM_config(cfg);
}

void Motors_backward(int speed){ 
	// 0 |--------------| 50 |--------------| 100
	MotorsCfg cfg = {0, 0, 0, 0};
	
	printf("backward\r\n");
  cfg.LQ_speed = -speed;
  cfg.RQ_speed = -speed;
  cfg.LH_speed = -speed;
  cfg.RH_speed = -speed;
	PWM_config(cfg);
	
}
// ��ƽ��
// dir: 0��1��ǰ��-1���
void Motors_left(int speed, int dir){
	MotorsCfg cfg = {0, 0, 0, 0};
	
	printf("left\r\n");
	if(dir == 1){
		cfg.RQ_speed = speed;
		cfg.LH_speed = speed;		
	} else if(dir == -1){
		cfg.LQ_speed = -speed;
		cfg.RH_speed = -speed;		
	} else { // 0
		cfg.RQ_speed = speed;
		cfg.LH_speed = speed;	
		cfg.LQ_speed = -speed;
		cfg.RH_speed = -speed;			
	}
  
	PWM_config(cfg);
}

// ��ƽ��
// dir: 0�ң�1��ǰ��-1�Һ�
void Motors_right(int speed, int dir){
	MotorsCfg cfg = {0, 0, 0, 0};
	
	printf("right\r\n");
	if(dir == 1 || dir == 0){
		cfg.LQ_speed = speed;
		cfg.RH_speed = speed;		
	}
  
	if(dir == -1 || dir == 0){
		cfg.RQ_speed = -speed;
		cfg.LH_speed = -speed;	
	}

 	PWM_config(cfg);
}

// ԭ�ش�ת direction
// dir: 0���ͷ����ʱ�룩 1�ҵ�ͷ��˳ʱ�룩
void Motors_around(int speed, int dir){
	MotorsCfg cfg = {0, 0, 0, 0};
	if(dir == 1){
		cfg.LQ_speed = speed;
		cfg.LH_speed = speed;	
		cfg.RQ_speed = -speed;
		cfg.RH_speed = -speed;	
	}else {
		cfg.LQ_speed = -speed;
		cfg.LH_speed = -speed;	
		cfg.RQ_speed = speed;
		cfg.RH_speed = speed;	
	}
	
 	PWM_config(cfg);
}

void Motors_stop(){

	MotorsCfg cfg;
	printf("stop\r\n");
  cfg.LQ_speed = 0;
  cfg.RQ_speed = 0;
  cfg.LH_speed = 0;
  cfg.RH_speed = 0;
	PWM_config(cfg);
}