#include "led.h"

void LED_Init(void){
	RCC->APB2ENR |= 1<<4;			//PCʱ��ʹ��
	GPIOC->CRH &= 0xFF0FFFFF;		//PC13���ó�ʼ��
	GPIOC->CRH |= 0x00300000;		//PC13����Ϊ:ͨ���������������ٶ�Ϊ50MHz
	LED = 1;						//PA13Ĭ�ϵ�ƽΪ�ߵ�ƽ
	return;
}
