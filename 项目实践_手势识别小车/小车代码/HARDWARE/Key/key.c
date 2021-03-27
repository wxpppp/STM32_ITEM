#include "key.h"
#include "sys.h"

void KEY_Init(void){
	RCC->APB2ENR |= 1<<2;		//PAʱ��ʹ��
	RCC->APB2ENR |= 1<<4;		//PCʱ��ʹ��
	JTAG_Set(JTAG_SWD_DISABLE);	//�ر�JTAG-DP������WD-DP����PA15��PB3��PB4����
	
	GPIOC->CRL &= 0xFF0FFFFF;	//KEY1(PC5)����λˢ��
	GPIOC->CRL |= 0x00800000;	//KEY1(PC5)����Ϊ����������
	GPIOC->ODR |= 1<<5;			//KEY1(PC5)����
	
	GPIOA->CRH &= 0x0FFFFFFF;	//KEY2(PA15)����λˢ��
	GPIOA->CRH |= 0x80000000;	//KEY2(PA15)����Ϊ����������
	GPIOA->ODR |= 1<<15;		//KEY2(PA15)����
	
	GPIOA->CRL &= 0xFFFFFFF0;	//WKUP(PA0)����λˢ��
	GPIOA->CRL |= 0X00000008;	//WKUP(PA0)����Ϊ����������
	GPIOA->ODR |= ~(1<<0);		//WKUP(PA0)����
	
	return ;
}
