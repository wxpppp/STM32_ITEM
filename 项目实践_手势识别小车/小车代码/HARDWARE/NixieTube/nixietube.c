#include "nixietube.h"
#include "delay.h"
#include "sys.h"
#include "changemode.h"

void NixieTube_Init(void){					//数码管初始化
	RCC->APB2ENR |= 1<<2;					//PA时钟使能
	RCC->APB2ENR |= 1<<3;					//PB时钟使能
	GPIOA->CRL &= 0x0000FFFF;				//PA4到PA7设置位刷零
	GPIOA->CRL |= 0x33330000;				//PA4到PA7设置为推挽输出，最大50Mhz
	GPIOB->CRL &= 0xFF00FFFF;				//PB4和PB5设置位刷零
	GPIOB->CRL |= 0x00330000;				//PB4和PB5设置为推挽输出，最大50Mhz
	GPIOA->CRH &= 0xFFFFFFF0;				//PA8设置位刷零
	GPIOA->CRH |= 0x00000003;				//PA8设置为推挽输出，最大50Mhz
	A0 = B1 = C2 = D3 = E4 = F5 = G6 = ON;	//闪烁两次表示初始化完成
	delay_ms(250);
	A0 = B1 = C2 = D3 = E4 = F5 = G6 = OFF;
	delay_ms(250);
	A0 = B1 = C2 = D3 = E4 = F5 = G6 = ON;
	delay_ms(250);
	A0 = B1 = C2 = D3 = E4 = F5 = G6 = OFF;
	
	return ;
}

void NixieTube(u8 number){		//数码管显示number，反馈选择模式
	switch(number){
		case MODE_STANDBY:		//0待机
			Number0();
			break;
		case MODE_CHECK:		//1自检
			Number1();
			break;
		case MODE_COPYCAT:		//2跟屁虫
			Number2();
			break;
		case MODE_ULTRASONIC:	//3超声波
			Number3();
			break;
		case MODE_INFRARED:		//4红外循迹
			Number4();
			break;
		case MODE_REMOTE:		//4红外遥控
			Number5();
			break;
		case MODE_BLUETOOTH:	//5蓝牙手势遥控
			Number6();
			break;
	}
	
	return ;
}

void NixieTube_Shine(void){
	return ;
}

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

void Number0(void){
	A0 = ON;
	B1 = ON;
	C2 = ON;
	D3 = ON;
	E4 = ON;
	F5 = ON;
	G6 = OFF;

	return ;
}

void Number1(void){
	A0 = OFF;
	B1 = ON;
	C2 = ON;
	D3 = OFF;
	E4 = OFF;
	F5 = OFF;
	G6 = OFF;

	return ;
}

void Number2(void){
	A0 = ON;
	B1 = ON;
	C2 = OFF;
	D3 = ON;
	E4 = ON;
	F5 = OFF;
	G6 = ON;

	return ;
}

void Number3(void){
	A0 = ON;
	B1 = ON;
	C2 = ON;
	D3 = ON;
	E4 = OFF;
	F5 = OFF;
	G6 = ON;

	return ;
}

void Number4(void){
	A0 = OFF;
	B1 = ON;
	C2 = ON;
	D3 = OFF;
	E4 = OFF;
	F5 = ON;
	G6 = ON;

	return ;
}

void Number5(void){
	A0 = ON;
	B1 = OFF;
	C2 = ON;
	D3 = ON;
	E4 = OFF;
	F5 = ON;
	G6 = ON;

	return ;
}

void Number6(void){
	A0 = ON;
	B1 = OFF;
	C2 = ON;
	D3 = ON;
	E4 = ON;
	F5 = ON;
	G6 = ON;
	
	return;
}
