#include "Ultrasonic.h"
#include "GPIO.h"


void Ultrasonic_init(){
	GPIO_INIT();
	
	// Ĭ������
	TRIG = 0;
	// ����
	ECHO = 0;
	
}
void Delay10us(void)	//@24.000MHz
{
	unsigned char data i;

	i = 78;
	while (--i);
}

// ������࣬���������
// ����ֵ���Ƿ�ɹ���0�ɹ�SUCCESS�� -1ʧ��FAIL
// distance�ᱻ���Ϊ���룬��λ��cm
char Ultrasonic_get_distance(float* distance){
	u16 cnt_10us = 0;
	float calc_dis = 0;
	
	// 1. ��TRIG����10us����
	TRIG = 1;
	Delay10us();
	Delay10us();
	TRIG = 0;
	
	// Delayһ��ʱ�䣬ȷ��ECHO��������
	Delay10us();
	
	// 2. �ȴ�ECHO������,��һ��ʱ���� (194us) < 300us
	while(ECHO == 0 && cnt_10us <= 30){
		Delay10us();
		cnt_10us++; // 30 -> 300us
	}
	
	if(cnt_10us * 10 >= 300){
		printf("ECHO�����źų�ʱ: %d us, �˳�\n", (int)(cnt_10us * 10));
		return FAIL; // -1
	}
	
//	printf("ECHO: %d us\n", (int)(cnt_10us * 10));
	
	// 3. ��¼ECHO�������ص��½��صļ��ʱ��
	cnt_10us = 0;
	while(ECHO == 1){
		Delay10us();
		cnt_10us++;
	}
	// 3000us -> 3ms
	// us -> ms
	printf("echo -> %.2f ms\n", (cnt_10us * 0.01));
	
	// dis = (11ms * 340m/s) / 2
	// dis = (11ms * 34000cm/1000ms) / 2
	// dis = (11ms * 34cm/ms) / 2
	// dis = 187cm
	calc_dis = ((cnt_10us * 0.01) * 34) * 0.5;
	
	*distance = calc_dis;
	
	if(calc_dis < 2.0){ // ����̫�̣���ֵ����֤
		return 1;
	}else if(calc_dis > 400.0){ // ����̫Զ����ֵҲ����֤
		return 2;
	}
	
	return SUCCESS; // 0
}
