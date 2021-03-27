#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

#define BLUE_FIX 800

#include "usart2.h"
#include "pwm.h"
#include "changemode.h"
#include "usart.h"

void Bluetooth_Init(void);								//蓝牙手势遥控模块初始化
void Bluetooth(void);									//蓝牙手势遥控功能函数

#endif
