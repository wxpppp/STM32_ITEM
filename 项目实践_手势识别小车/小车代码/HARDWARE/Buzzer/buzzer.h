#ifndef BUZZER_H
#define BUZZER_H

#include "sys.h"

#define BUZZER PCout(7)					//将BUZZER定义为PC7
#define Buzzer_On	1
#define Buzzer_Off	0

void Buzzer_Init(void);					//BUZZER(PC7)初始化
void Buzzer(u8 number, u16 time);		//蜂鸣器响number次，每次间隔为time毫秒

#endif
