#include "key.h"
#include "sys.h"

void KEY_Init(void){
	RCC->APB2ENR |= 1<<2;		//PA时钟使能
	RCC->APB2ENR |= 1<<4;		//PC时钟使能
	JTAG_Set(JTAG_SWD_DISABLE);	//关闭JTAG-DP，启用WD-DP，让PA15，PB3，PB4可用
	
	GPIOC->CRL &= 0xFF0FFFFF;	//KEY1(PC5)设置位刷零
	GPIOC->CRL |= 0x00800000;	//KEY1(PC5)设置为上下拉输入
	GPIOC->ODR |= 1<<5;			//KEY1(PC5)上拉
	
	GPIOA->CRH &= 0x0FFFFFFF;	//KEY2(PA15)设置位刷零
	GPIOA->CRH |= 0x80000000;	//KEY2(PA15)设置为上下拉输入
	GPIOA->ODR |= 1<<15;		//KEY2(PA15)上拉
	
	GPIOA->CRL &= 0xFFFFFFF0;	//WKUP(PA0)设置位刷零
	GPIOA->CRL |= 0X00000008;	//WKUP(PA0)设置为上下拉输入
	GPIOA->ODR |= ~(1<<0);		//WKUP(PA0)下拉
	
	return ;
}
