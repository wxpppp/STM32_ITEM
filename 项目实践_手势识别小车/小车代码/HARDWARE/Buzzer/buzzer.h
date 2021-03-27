#ifndef BUZZER_H
#define BUZZER_H

#include "sys.h"

#define BUZZER PCout(7)					//��BUZZER����ΪPC7
#define Buzzer_On	1
#define Buzzer_Off	0

void Buzzer_Init(void);					//BUZZER(PC7)��ʼ��
void Buzzer(u8 number, u16 time);		//��������number�Σ�ÿ�μ��Ϊtime����

#endif
