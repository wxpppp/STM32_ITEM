#ifndef CHANGEMODE_H
#define CHANGEMODE_H

#include "sys.h"

//小车模式
#define MODE_NUMBER		7	//模式总数
#define MODE_STANDBY	0	//待机
#define MODE_CHECK		1	//自检
#define MODE_COPYCAT	2	//跟屁虫
#define MODE_ULTRASONIC	3	//超声波
#define MODE_INFRARED	4	//红外循迹
#define MODE_REMOTE		5	//红外遥控
#define MODE_BLUETOOTH	6	//蓝牙手势遥控
//选择模式标志
#define MODE_OFF		0	//选择关闭
#define MODE_ON			1	//选择开启，开启后，所有动作应该停止

void ChangeMode_Init(void);	//模式改变，使WKUP(PA0)为中断输入，进入模式选择，初始化函数
void ChangeMode(void);		//选择模式，改变全局变量mode的值
u8 KEY1_Scan(u8 set);		//扫描KEY1是否按下，按下返回1，否则返回0，set为1时开启连按，为0时关闭连按

#endif
