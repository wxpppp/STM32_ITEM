#ifndef _COPYCAT_H
#define _COPYCAT_H

#define Distance		15				//距离
#define Copycat_Speed1	4500			//上限速度
#define Copycat_Speed2	800				//下限速度
#define Copycat_PID_P	1700			//比例常数
#define Copycat_PID_I	0.7				//积分常数
#define Copycat_PID_D	0				//微分常数
#define Copycat_PID_K	1				//K常数

#include "ultrasonic.h"
#include "pwm.h"
#include "changemode.h"

struct Copycat_Pid{						//记录Copycat的PID控制结构体，目标距离为ultrasonic.h中的Limit_Distance
	int last_error;						//error[-1]
	int prev_error;						//error[-2]
	int sum_error;						//sum of errors
};

void Copycat(void);						//跟屁虫功能函数
int Copycat_PID(void);					//PID控制算法
void Copycat_PID_Reset(void);			//PID控制清零

#endif
