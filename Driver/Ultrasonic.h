#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include "config.h"

#define TRIG	P47
#define ECHO	P33

#define GPIO_INIT()	            \
	/* TRIG ����*/                \
	P4_MODE_OUT_PP(GPIO_Pin_7);   \
	/* ECHO ����*/              \
	P3_MODE_IN_HIZ(GPIO_Pin_3);	  \
	
void Ultrasonic_init();

// ������࣬���������
// ����ֵ���Ƿ�ɹ���0�ɹ�SUCCESS�� -1ʧ��FAIL
// distance�ᱻ���Ϊ���룬��λ��cm
char Ultrasonic_get_distance(float* distance);

#endif