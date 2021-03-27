#ifndef _COPYCAT_H
#define _COPYCAT_H

#define Distance		15				//����
#define Copycat_Speed1	4500			//�����ٶ�
#define Copycat_Speed2	800				//�����ٶ�
#define Copycat_PID_P	1700			//��������
#define Copycat_PID_I	0.7				//���ֳ���
#define Copycat_PID_D	0				//΢�ֳ���
#define Copycat_PID_K	1				//K����

#include "ultrasonic.h"
#include "pwm.h"
#include "changemode.h"

struct Copycat_Pid{						//��¼Copycat��PID���ƽṹ�壬Ŀ�����Ϊultrasonic.h�е�Limit_Distance
	int last_error;						//error[-1]
	int prev_error;						//error[-2]
	int sum_error;						//sum of errors
};

void Copycat(void);						//��ƨ�湦�ܺ���
int Copycat_PID(void);					//PID�����㷨
void Copycat_PID_Reset(void);			//PID��������

#endif
