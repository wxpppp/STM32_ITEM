#include "led.h"

void LED_Init(void){
	RCC->APB2ENR |= 1<<4;			//PC时钟使能
	GPIOC->CRH &= 0xFF0FFFFF;		//PC13设置初始化
	GPIOC->CRH |= 0x00300000;		//PC13设置为:通用推挽输出，最大速度为50MHz
	LED = 1;						//PA13默认电平为高电平
	return;
}
