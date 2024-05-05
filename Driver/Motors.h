#ifndef __MOTORS_H__
#define __MOTORS_H__

#include "config.h"

void Motors_init();


// ǰ��
void Motors_forward(int speed); // [0, 100]

// ����
void Motors_backward(int speed); // [0, 100]

// ��ƽ��
void Motors_left(int speed, int dir);

// ��ƽ��
void Motors_right(int speed, int dir);

// ԭ�ش�ת direction
void Motors_around(int speed, int dir);

void Motors_stop();
#endif