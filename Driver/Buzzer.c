#include "Buzzer.h"
#include "GPIO.h"
#include "STC8H_PWM.h"
#include "Switch.h"
#include "NVIC.h"
#include "delay.h"


#define BUZZER								P34
#define BUZZER_GPIO_INIT()		P3_MODE_OUT_PP(GPIO_Pin_4)

#define BUZZER_HZ			1000

#define PERIOD 				((MAIN_Fosc / BUZZER_HZ) - 1)
PWMx_Duty dutyB;
void	PWM_config(u8 enable)
{
	PWMx_InitDefine		PWMx_InitStructure;

	// ����PWM8
	PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    		= PERIOD * 0.5f;	//PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect  = enable ? ENO8P : 0;			//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM8, &PWMx_InitStructure);			//��ʼ��PWM,  PWMA,PWMB

	// ����PWMB
	PWMx_InitStructure.PWM_Period   = PERIOD;			//����ʱ��,   0~65535
	PWMx_InitStructure.PWM_DeadTime = 0;					//��������������, 0~255
	PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//�����ʹ��, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//ʹ�ܼ�����, ENABLE,DISABLE
	PWM_Configuration(PWMB, &PWMx_InitStructure);			//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

	// �л�PWMͨ��
	PWM8_SW(PWM8_SW_P34);					//PWM8_SW_P23,PWM8_SW_P34,PWM8_SW_P03,PWM8_SW_P77

	// ��ʼ��PWMB���ж�
	NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}

void Buzzer_init(){
	BUZZER_GPIO_INIT();
	
	PWM_config(DISABLE);
}

// ����ָ��Ƶ����
void Buzzer_open(){
//	BUZZER = 1;
	PWM_config(ENABLE);
}

// �ر���
void Buzzer_close(){
//	BUZZER = 0;
	PWM_config(DISABLE);
}

// ��count��, ����ָ�����ʱ��gap ms . gap < 1275
void Buzzer_play(u8 count, u16 gap){
	u8 i;
	for (i = 0; i < count; i++){
		Buzzer_open();
//		delay_ms(100);
		os_wait2(K_TMO, 20);// 20 * 5ms = 100ms
		
		Buzzer_close();
//		delay_ms(gap);
		os_wait2(K_TMO, gap / 5);// 100ms / 5ms = N
	}
}

void Delay100us(void)	//@24.000MHz
{
	unsigned char data i, j;

	i = 4;
	j = 27;
	do
	{
		while (--j);
	} while (--i);
}

// ��һС��ʱ�� 100ms
void Buzzer_beep(){
	int i;
	for(i = 0; i < 1000; i++){
		BUZZER = !BUZZER;
		//  1ms -> 500Hz
		//0.1ms -> 5000Hz
		Delay100us();
	}
	// 1000 * 0.1ms = 100ms
}