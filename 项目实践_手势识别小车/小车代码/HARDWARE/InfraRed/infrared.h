#ifndef _INFRARED_H
#define _INFRARED_H

#define A_L	PBin(9)						//ǰ����PB9
#define A_R	PBin(10)					//ǰ���ң�PB10
#define B_L	PBin(8)						//������PB8
#define B_R	PBin(11)					//�����ң�PB11
#define SPEED_1	-500
#define SPEED0	0
#define SPEED1	1000+SPEED_1
#define SPEED2	2000+SPEED_1
#define SPEED3	3000+SPEED_1
#define SPEED4	4000+SPEED_1
#define SPEED5	5000+SPEED_1
#define SPEED6	6000+SPEED_1
#define SPEED7	7000+SPEED_1
#define DELAY_TIME 200

#include "stm32f10x.h"
#include "delay.h"
#include "pwm.h"
#include "changemode.h"

void Infrared_Init(void);				//����ѭ��ģ���ʼ��
void Infrared(void);					//����ѭ��ģ�麯��

#endif
