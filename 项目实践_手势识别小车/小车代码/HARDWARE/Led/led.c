#include "led.h"
#include "sys.h"
#include "delay.h"
#include "nixietube.h"
#include "changemode.h"

void LED_Init(void){						//LED0(PC8)��LED1(PC9)��ʼ����ͨ����ʱ��(TIM3)�жϿ��������Ƶ����𣬷���ϵͳ״̬
	RCC->APB2ENR |= 1<<4;					//PCʱ��ʹ��
	GPIOC->CRH &= 0xFFFFFF00;				//LED0(PC8)LED1(PC9)����λˢ��
	GPIOC->CRH |= 0x00000033;				//LED0(PC8)LED1(PC9)����Ϊͨ��������������50MHz
	LED0 = LED1 = Led_On;
	
	return ;
}

void TIM3_Int_Init(u16 arr, u16 psc){		//ͨ�ö�ʱ��3��ʼ����ʱ��ѡ��ΪAPB1(36MHz)��2��72MHz��arr�Զ���װ��ֵ��pscʱ��Ԥ��Ƶ��
	RCC->APB1ENR |= 1<<1;					//TIM3ʱ��ʹ��
	TIM3->ARR = arr;						//�趨�������Զ���װ��ֵ���պ�1ms
	TIM3->PSC = psc;						//Ԥ��Ƶ��7200���õ�10KHz�ļ���ʱ��
	TIM3->DIER |= 1<<0;						//��������ж�
	MY_NVIC_Init(2, 3, TIM3_IRQn, 2);		//���ö�ʱ���жϣ���ռ2����Ӧ3����2
	TIM3->CR1 |= 0x0001;					//ʹ�ܼ�����3	
	
	return ;
}

void TIM3_IRQHandler(void){								//TIM3ʱ���жϷ������LED0(PC8)LED1(PC9)��˸����
	extern u8 ex_mode;									//ģʽѡ����ƣ�Ϊ1ʱ������ѡ��ģʽ��Ϊ0ʱ���˳�ѡ��ģʽ
	extern u8 mode;										//��ǰģʽ��־
	
	if(TIM3->SR & 0x0001){								//��ȡ�жϱ�־λ���Ƿ����ж�
		if(ex_mode == MODE_OFF && mode != MODE_STANDBY){//ִ�г���ģʽ��LED0(PC8)LED1(PC9)������
			switch(LED1 * 2 + LED0 * 1){				//���ü�Ȩ���룬����LED���룬���ĸ����룬ͨ������ʵ��ת��
				case 0:									//ƫ��̬
					LED0 = Led_On;
					break;
				case 1:
					LED0 = !LED0;
					LED1 = !LED1;
					break;
				case 2:
					LED0 = !LED0;
					LED1 = !LED1;
					break;			
				case 3:									//ƫ��̬
					LED0 = Led_Off;
					break;
			}
		}
		else if(ex_mode == MODE_ON)						//ѡ��ģʽ��LED0(PC8)LED1(PC9)����
			LED0 = LED1 = Led_On;
		else{											//����ģʽ��LED0(PC8)LED1(PC9)˫�������ǡ��Ϊ�����˳�ģʽ��ʲô������ִ��
			switch(LED1 * 2 + LED0 * 1){				//���ü�Ȩ���룬����LED���룬���ĸ����룬ͨ������ʵ��ת��
				case 0:
					LED0 = LED1 = Led_On;
					break;
				case 1:									//ƫ��̬
					LED0 = Led_Off;
					break;
				case 2:									//ƫ��̬
					LED0 = Led_On;
					break;
				case 3:
					LED0 = LED1 = Led_Off;
					break;
			}
		}
	}
	TIM3->SR &= ~(1<<0);								//����жϱ�־λ
	
	return ;
}
