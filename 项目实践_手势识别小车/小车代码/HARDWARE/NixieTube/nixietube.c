#include "nixietube.h"
#include "delay.h"
#include "sys.h"
#include "changemode.h"

void NixieTube_Init(void){					//����ܳ�ʼ��
	RCC->APB2ENR |= 1<<2;					//PAʱ��ʹ��
	RCC->APB2ENR |= 1<<3;					//PBʱ��ʹ��
	GPIOA->CRL &= 0x0000FFFF;				//PA4��PA7����λˢ��
	GPIOA->CRL |= 0x33330000;				//PA4��PA7����Ϊ������������50Mhz
	GPIOB->CRL &= 0xFF00FFFF;				//PB4��PB5����λˢ��
	GPIOB->CRL |= 0x00330000;				//PB4��PB5����Ϊ������������50Mhz
	GPIOA->CRH &= 0xFFFFFFF0;				//PA8����λˢ��
	GPIOA->CRH |= 0x00000003;				//PA8����Ϊ������������50Mhz
	A0 = B1 = C2 = D3 = E4 = F5 = G6 = ON;	//��˸���α�ʾ��ʼ�����
	delay_ms(250);
	A0 = B1 = C2 = D3 = E4 = F5 = G6 = OFF;
	delay_ms(250);
	A0 = B1 = C2 = D3 = E4 = F5 = G6 = ON;
	delay_ms(250);
	A0 = B1 = C2 = D3 = E4 = F5 = G6 = OFF;
	
	return ;
}

void NixieTube(u8 number){		//�������ʾnumber������ѡ��ģʽ
	switch(number){
		case MODE_STANDBY:		//0����
			Number0();
			break;
		case MODE_CHECK:		//1�Լ�
			Number1();
			break;
		case MODE_COPYCAT:		//2��ƨ��
			Number2();
			break;
		case MODE_ULTRASONIC:	//3������
			Number3();
			break;
		case MODE_INFRARED:		//4����ѭ��
			Number4();
			break;
		case MODE_REMOTE:		//4����ң��
			Number5();
			break;
		case MODE_BLUETOOTH:	//5��������ң��
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
