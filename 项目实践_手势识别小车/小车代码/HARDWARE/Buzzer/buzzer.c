#include "buzzer.h"
#include "delay.h"
#include "nixietube.h"

void Buzzer_Init(void){						//BUZZER(PC7)初始化
	RCC->APB2ENR |= 1<<4;					//PC时钟使能
	GPIOC->CRL &= 0x0FFFFFFF;				//BUZZER(PC7)设置位刷零
	GPIOC->CRL |= 0x30000000;				//BUZZER(PC7)设置为通用推挽输出，最大50MHz
	Buzzer(2, 250);							//快响两下提示初始化完成
	
	return ;
}

void Buzzer(u8 number, u16 time){				//蜂鸣器响number次，每次间隔为time毫秒
	u8 i;									//计数器
	
	for(i = 0; i < number; i++){
		BUZZER = Buzzer_On;
		delay_ms(250);
		BUZZER = Buzzer_Off;
		delay_ms(time);
	}
	
	return ;
}
