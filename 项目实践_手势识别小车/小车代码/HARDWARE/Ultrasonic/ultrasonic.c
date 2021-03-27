#include "ultrasonic.h"
#include "changemode.h"
#include "pwm.h"

static struct Capture_Register cap;					//ultrasonic.c�ļ���ȫ�ֲ�����������

void Ultrasonic(void){								//���������Ϲ��ܺ���
	extern u8 ex_mode;
	while(ex_mode == MODE_OFF){
		Ultrasonic_Work();
		if(Capture_Register_Empyt()){				//����һ�θߵ�ƽ
			if(Capture_Register_Display() <= Limit_Distance){//�жϾ�ǰ���ϰ���ľ����Ƿ�С�����ƾ���
				if(cap.times & 1)
					Car_Turn_Speed_pwm(BACK_LIFT, MAX_SPEED / 2 - 1, MAX_SPEED / 2);
				else
					Car_Turn_Speed_pwm(BACK_RIGHT, MAX_SPEED / 2 - 1, MAX_SPEED / 2);
				delay_ms(100);
			}
			Capture_Register_Reset();				//��������
		}
		Car_Turn_Speed_pwm(STRAIGHT, MAX_SPEED - 1000, 0);	//ȫ��ֱ��
	}
	Car_Turn_Speed_pwm(STOP, 0, 0);					//�˳�ʱС����λ
	Capture_Register_Reset();
	return;
}

void Trig_Init(void){								//PA12��ʼ����������������50MHz
	RCC->APB2ENR |= 1<<2;							//ʹ��PAʱ��
	GPIOA->CRH &= 0xFFF0FFFF;						//PA12��������
	GPIOA->CRH |= 0x00030000;						//PA12����Ϊͨ���������
	Trig = 0;
	return;
}

void Echo_Init(void){								//PA11��ʼ����������������ģʽ
	RCC->APB2ENR |= 1<<2;							//ʹ��PAʱ��
	GPIOA->CRH &= 0xFFFF0FFF;						//PA11��������
	GPIOA->CRH |= 0x00008000;						//PA11����Ϊ������������ģʽ
	PAin(11) = 0;									//����
	return;
}

void TIM1_CH4_Init(u16 arr, u16 psc){				//TIM1_CH4���벶���ʼ��
	RCC->APB2ENR |= 1<<11;							//TIM1ʱ��ʹ��
	
	TIM1->ARR = arr;								//�趨���������Զ���װ��ֵ
	TIM1->PSC = psc;								//Ԥ��Ƶ
	
	TIM1->CCMR2 &= 0x00FF;							//TIM1->CCMR2ͨ��4��������
	TIM1->CCMR2 |= 1<<8;							//CC4S����Ϊ01��IC4ӳ�䵽TI4��
	TIM1->CCMR2 |= 0<<10;							//IC4PSC����Ϊ00����Ԥ��Ƶ
	TIM1->CCMR2 |= 0<<12;							//IC4F����Ϊ0000�����˲���
	
	TIM1->CCER &= ~(1<<13);							//CC4P����Ϊ0�������ز���
	TIM1->CCER |= 1<<12;							//CC4E����Ϊ1������ʹ��
	
	TIM1->DIER |= 1<<4;								//������4�ж�
	TIM1->DIER |= 1<<0;								//��������ж�
	
	TIM1->CR1 |= 1<<0;								//ʹ��TIM1��ʱ��
	MY_NVIC_Init(2, 0, TIM1_CC_IRQn, 2);			//��ռ2�������ȼ�0����2
	return;
}

void Ultrasonic_Init(void){							//������ģ��ĳ�ʼ��
	Trig_Init();									//Trig(PA12)��ʼ����������������50MHz
	Echo_Init();									//Echo(PA11)��ʼ����������������
	TIM1_CH4_Init(0xFFFF, 72 - 1);					//TIM1_CH4��ʼ��
	Capture_Register_Reset();						//����������������
	uart_init(72, 9600);							//���ڳ�ʼ��
	return;
}

void Ultrasonic_Work(void){							//��������ģ��һ�������źţ�ʹ�䷢�䳬����
	Trig = 0;
	Trig = 1;
	delay_us(16);									//ʹ�������ʱ���Ƽ���ʱ��
	Trig = 0;
	return;
}

void Capture_Register_Reset(void){					//Capture_Register����
	cap.flag1 = cap.flag2 = cap.times = cap.value = 0;
	return;
}

unsigned char Capture_Register_Empyt(void){			//�ж��Ƿ����һ�����������벶�񣬼�����cap.flag1��ֵ
	return cap.flag1;
}

u32 Capture_Register_Display(void){					//�򴮿ڴ�ӡ�������ݲ����ؾ�������
	u32 temp = ((cap.times * 65536 + cap.value) * Sound_Velocity) / 2;
	//printf("%d cm\r\n", temp);						//�򴮿ڴ�ӡ����
	return temp;
}

void TIM1_CC_IRQHandler(void){						//TIM1_CH4�����жϷ�����
	u16 tsr = TIM1->SR;
	if(cap.flag1 == 0){								//δ���һ���жϲ���
		if(tsr & 0x01 && cap.flag2){				//��������жϣ������Ѿ����������ش��ڸߵ�ƽ״̬
			if(cap.times == 0xFF){					//�ߵ�ƽʱ��̫��
				cap.flag1 = 1;
				cap.value = 0xFFFF;
			}
			else									//���������һ
				cap.times++;
		}
		if(tsr & 0x10){								//����4���������¼�
			if(cap.flag2 == 0){						//����������
				Capture_Register_Reset();
				cap.flag2 = 1;						//��ʼһ�����벶���Ѿ����������أ��ȴ��½���
				TIM1->CNT = 0;						//����������
				TIM1->CCER |= 1<<13;				//CC4P����Ϊ�½��ز���
			}
			else{									//�����½���
				cap.flag1 = 1;						//���һ�����벶��
				cap.value = TIM1->CCR4;				//��ȡ��ǰ����ֵ
				TIM1->CCER &= ~(1<<13);				//CC4P��������Ϊ�����ز���
			}
		}
	}
	TIM1->SR = 0;									//����жϱ�־λ
	return;
}
