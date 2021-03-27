#ifndef NIXIETUBE_H
#define NIXIETUBE_H

#include "sys.h"

#define ON  0	//开
#define OFF 1	//关
//定义各个引脚的接线
#define A0 PBout(4)
#define B1 PBout(5)
#define C2 PAout(4)
#define D3 PAout(5)
#define E4 PAout(6)
#define F5 PAout(7)
#define G6 PAout(8)

void NixieTube_Shine(void);			//数码管闪烁
void NixieTube(u8 number);			//显示接收到的参数
void NixieTube_Init(void);			//数码管初始化
//显示0-9数字的10个函数
void Number0(void);//0
void Number1(void);//1
void Number2(void);//2
void Number3(void);//3
void Number4(void);//4
void Number5(void);//5
void Number6(void);//6

#endif

/****************************************************************************//*
Number		A(PA2)	B(PA3)	C(PA4)	D(PA5)	E(PA6)	F(PA7)	G(PA8)		
0			1		1		1		1		1		1		0
1			0		1		1		0		0		0		0
2			1		1		0		1		1		0		1
3			1		1		1		1		0		0		1
4			0		1		1		0		0		1		1
5			1		0		1		1		0		1		1
6			1		0		1		1		1		1		1
7			1		1		1		0		0		0		0
8			1		1		1		1		1		1		1
9			1		1		1		1		0		1		1
******************************************************************************/
