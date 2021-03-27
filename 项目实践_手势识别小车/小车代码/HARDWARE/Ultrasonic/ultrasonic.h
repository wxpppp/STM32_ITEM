/********************************************************************************************************

HC-SR04超声波测距模块的基本使用方法：
	(1)采用IO口TRIG触发测距，给至少10us的高电平信号
	(2)模块自动发送8个40KHz的方波，自动检测是否有信号返回
	(3)有信号返回，通过IO口ECHO输出一个高电平，高电平持续的时间就是超声波从发射到返回的时间
	(4)测试距离=(高电平时间*声速(340m/s))/2
	
********************************************************************************************************/

#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#define Trig PAout(12)						//超声波模块控制端，配置为推挽输出
#define Echo 								//超声波模块的反馈端，配置为输入捕获，使用TIM1_CH4
#define Sound_Velocity 0.034				//声速，0.034 cm/us
#define Limit_Distance 20					//限制距离，单位厘米(cm)

#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

struct Capture_Register{					//记录捕获状态的类
	unsigned char flag1;					//标志是否有完整的一次输入捕获，0的时候无，1的时候有一次完整的捕获
	unsigned char flag2;					//标志是否有一次上升沿的捕获，0没有，1有
	u8 times;								//记录计数器溢出的次数，总时间大约为times*65536
	u16 value;								//记录未达到上限的时间
};

void Ultrasonic(void);						//超声波避障功能函数
void Ultrasonic_Init(void);					//超声波模块的初始化
void Capture_Register_Reset(void);			//Capture_Register清零
u32 Capture_Register_Display(void);			//向串口打印距离数据并返回距离数据
unsigned char Capture_Register_Empyt(void);			//判断是否完成一次完整的输入捕获，即返回cap.flag1的值
void TIM1_CH4_Init(u16 arr, u16 psc);		//TIM1_CH4输入捕获初始化
void Trig_Init(void);						//PA12初始化，推挽输出，最大50MHz
void Echo_Init(void);						//PA11初始化，上拉下拉输入模式
void Ultrasonic_Work(void);					//给超声波模块一个激励信号，使其发射超声波

#endif
