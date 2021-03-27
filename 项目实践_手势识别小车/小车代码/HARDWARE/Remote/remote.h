#ifndef _REMOTE_H
#define _REMOTE_H

#define RDATA PAin(1)							//红外输入脚
#define REMOTE_ID 0								//设定遥控器识别码
#define Remote_Speed0 0
#define Remote_Speed1 2500
#define Remote_Speed2 5000

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "changemode.h"
#include "pwm.h"
#include "buzzer.h"

void Remote(void);								//红外遥控模块功能函数
void Remote_Init(void);							//红外遥控初始化
u8 Remote_Scan(void);							//扫描红外键盘，没任何按键按下返回0，若有按键按下返回相应的键值

#endif
