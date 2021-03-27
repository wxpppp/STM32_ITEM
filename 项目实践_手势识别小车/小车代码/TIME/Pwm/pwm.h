#ifndef PWM_H
#define PWM_H

#include "sys.h"

#define BALANCE		0									//���ֵ������ƽ��
#define PWM_RIGHT_A TIM4->CCR1							//����(A)pwm���ռ�ձ�(��ƽ)��PB6
#define PWM_LIFT_B  TIM4->CCR2							//����(B)pwm���ռ�ձ�(��ƽ)��PB7
#define AIN1 PBout(0)									//����(A)����ת�����1������
#define AIN2 PBout(1)									//����(A)����ת�����2������
#define BIN1 PBout(2)									//����(B)����ת�����1������
#define BIN2 PBout(3)									//����(B)����ת�����2������
#define RIGHT_WHEEL 1									//����(A)����ӦPWM1
#define LIFT_WHEEL  2									//����(B)����ӦPWM2
#define STOP		0									//����ֹͣ
#define STRAIGHT	1									//������ת
#define BACK		2									//���ӷ�ת
#define TURN_RIGHT	3									//������ת
#define TURN_LIFT	4									//������ת
#define BACK_RIGHT	5									//�������ҵ�
#define BACK_LIFT	6									//��������
#define MAX_SPEED	8000								//�����

void PWM_Init(u16 arr, u16 psc);						//TIM4_CH1(PB6)��TIM4_CH2(PB7)��pwm��ʼ��
void Wheel_Turn_Pwm(u8 wheel, u8 direction, u16 pwm);	//wheel���ӣ���direction����ת���Լ���Ӧpwm��ѹ
void Car_Turn_Speed_pwm(u8 turn, u16 speed, u16 pwm);	//����turnת���������ӵĻ����ٶ�speed��ת��ʱ���ֵ��ٶȲ�

#endif
