#ifndef PWM_H
#define PWM_H

#include "sys.h"

#define BALANCE		0									//两轮电机基础平衡
#define PWM_RIGHT_A TIM4->CCR1							//右轮(A)pwm输出占空比(电平)，PB6
#define PWM_LIFT_B  TIM4->CCR2							//左轮(B)pwm输出占空比(电平)，PB7
#define AIN1 PBout(0)									//右轮(A)轮子转向控制1，正极
#define AIN2 PBout(1)									//右轮(A)轮子转向控制2，负极
#define BIN1 PBout(2)									//左轮(B)轮子转向控制1，正极
#define BIN2 PBout(3)									//左轮(B)轮子转向控制2，负极
#define RIGHT_WHEEL 1									//右轮(A)，对应PWM1
#define LIFT_WHEEL  2									//左轮(B)，对应PWM2
#define STOP		0									//轮子停止
#define STRAIGHT	1									//轮子正转
#define BACK		2									//轮子反转
#define TURN_RIGHT	3									//车子右转
#define TURN_LIFT	4									//车子左转
#define BACK_RIGHT	5									//车子向右倒
#define BACK_LIFT	6									//车子向左倒
#define MAX_SPEED	8000								//最大车速

void PWM_Init(u16 arr, u16 psc);						//TIM4_CH1(PB6)与TIM4_CH2(PB7)的pwm初始化
void Wheel_Turn_Pwm(u8 wheel, u8 direction, u16 pwm);	//wheel轮子，向direction方向转，以及相应pwm电压
void Car_Turn_Speed_pwm(u8 turn, u16 speed, u16 pwm);	//车向turn转，两个轮子的基础速度speed，转向时两轮的速度差

#endif
