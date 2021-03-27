#ifndef LED_H
#define LED_H

#include "sys.h"

#define LED0 PCout(8)					//定义LED0为PC8，蓝
#define LED1 PCout(9)					//定义LED1为PC9，黄
#define Led_On	1
#define Led_Off	0

void TIM3_Int_Init(u16 arr, u16 psc);	//通用定时器3初始化，时钟选择为APB1(36MHz)的2倍72MHz，arr自动重装载值，psc时钟预分频数
void LED_Init(void);					//LED0(PC8)，LED1(PC9)初始化，通过定时器(TIM3)中断控制两个灯的亮灭，反馈系统状态
										//待机时“双闪”；选择模式时“常亮”；执行模式时“交替闪烁”
#endif
