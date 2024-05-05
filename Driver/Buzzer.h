#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "config.h"

void Buzzer_init();

// ����ָ��Ƶ����
void Buzzer_open();

// �ر���
void Buzzer_close();

// ��һС��ʱ��
void Buzzer_beep();

// ��count��, ����ָ�����ʱ��gap ms
void Buzzer_play(u8 count, u16 gap);

#endif