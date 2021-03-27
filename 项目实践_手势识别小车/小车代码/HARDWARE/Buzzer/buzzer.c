#include "buzzer.h"
#include "delay.h"
#include "nixietube.h"

void Buzzer_Init(void){						//BUZZER(PC7)��ʼ��
	RCC->APB2ENR |= 1<<4;					//PCʱ��ʹ��
	GPIOC->CRL &= 0x0FFFFFFF;				//BUZZER(PC7)����λˢ��
	GPIOC->CRL |= 0x30000000;				//BUZZER(PC7)����Ϊͨ��������������50MHz
	Buzzer(2, 250);							//����������ʾ��ʼ�����
	
	return ;
}

void Buzzer(u8 number, u16 time){				//��������number�Σ�ÿ�μ��Ϊtime����
	u8 i;									//������
	
	for(i = 0; i < number; i++){
		BUZZER = Buzzer_On;
		delay_ms(250);
		BUZZER = Buzzer_Off;
		delay_ms(time);
	}
	
	return ;
}
