#include "remote.h"

//ң��������״̬
//RmtSta[7]:�յ����������־
//RmtSta[6]:�õ���һ��������������Ϣ
//RmtSta[5]:����	
//RmtSta[4]:����������Ƿ��Ѿ�������								   
//RmtSta[3:0]:�����ʱ��
static u8 RmtSta = 0;	  	  
static u16 Dval;									//�½���ʱ��������ֵ
static u32 RmtRec = 0;								//������յ�������

void Remote_Init(void){								//����ң�س�ʼ��
	RCC->APB2ENR |= 1<<2;    						//ʹ��PAʱ��
	GPIOA->CRL &= 0xFFFFFF0F;						//PA1����λ����
	GPIOA->CRL |= 0x00000080;						//PA1����Ϊ��/��������
	GPIOA->ODR |= 1<<1;								//PA1����
	
	RCC->APB1ENR |= 1<<3;							//TIM5ʱ��ʹ��
 	TIM5->ARR = 10000;								//�趨�������Զ���װֵ�����10ms���
	TIM5->PSC = 71;									//Ԥ��Ƶ����1M�ļ���Ƶ�ʣ�1us��1
	
	TIM5->CCMR1 |= 1<<8;							//CC2S=01��ѡ������ˣ�IC2ӳ�䵽TI2��
 	TIM5->CCMR1 |= 3<<12;							//IC2F=0011�����������˲�����8����ʱ��ʱ�������˲�
 	TIM5->CCMR1 |= 0<<10;							//IC2PS=00�����������Ƶ,����Ƶ
	
	TIM5->CCER |= 0<<5;								//CC2P=0�������ز���
	TIM5->CCER |= 1<<4;								//CC2E=1���������������ֵ������Ĵ�����
	TIM5->DIER |= 1<<2;								//����CC2IE�����ж�
	TIM5->DIER |= 1<<0;								//��������ж�
	TIM5->CR1 |= 0x01;								//ʹ�ܶ�ʱ��5
  	MY_NVIC_Init(1, 3, TIM5_IRQn, 2);				//��ռ1�������ȼ�3����2
	return;
}

void TIM5_IRQHandler(void){							//TIM5�жϷ�����
	u16 tsr = TIM5->SR;								//��ȡTIM5��SR
	if(tsr & 0x01){									//���
		if(RmtSta & 0x80){							//�ϴ������ݱ����յ���
			RmtSta &= ~0x10;						//ȡ���������Ѿ���������
			if((RmtSta & 0x0F) == 0x00)				//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
				RmtSta |= 1<<6;
			if((RmtSta & 0x0F) < 14)
				++RmtSta;
			else{
				RmtSta&=~(1<<7);					//���������ʶ
				RmtSta&=0xF0;						//��ռ�����	
			}
		}
	}
	if(tsr & (1<<2)){								//CC2IE�ж�
		if(RDATA){									//�����ز���
  			TIM5->CCER |= 1<<5;						//CC2P=1������Ϊ�½��ز���
			TIM5->CNT = 0;							//��ն�ʱ��ֵ
			RmtSta |= 0x10;							//����������Ѿ�������
		}
		else{										//�½��ز���
			Dval = TIM5->CCR2;						//��ȡCCR1Ҳ������CC2IF��־λ
  			TIM5->CCER &= ~(1<<5);					//CC2P=0������Ϊ�����ز���
			if(RmtSta & 0x10){						//���һ�θߵ�ƽ����
 				if(RmtSta & 0x80){					//���յ���������
					if(Dval > 300 && Dval < 800){	//560Ϊ��׼ֵ��560us
						RmtRec <<= 1;				//����һλ
						RmtRec |= 0;				//���յ�0
					}
					else if(Dval > 1400 && Dval < 1800){//1680Ϊ��׼ֵ��1680us
						RmtRec <<= 1;				//����һλ
						RmtRec |= 1;				//���յ�1
					}
					else if(Dval > 2200 && Dval < 2600)//�õ�������ֵ���ӵ���Ϣ��2500Ϊ��׼ֵ2.5ms
						RmtSta &= 0xF0;				//��ռ�ʱ��
 				}
				else if(Dval > 4200 && Dval < 4700)	//4500Ϊ��׼ֵ4.5ms
					RmtSta |= 1<<7;					//��ǳɹ����յ���������
			}
			RmtSta &= ~(1<<4);
		}
	}
	TIM5->SR = 0;									//����жϱ�־λ
	return;
}

u8 Remote_Scan(void){								//ɨ�������̣�û�κΰ������·���0�����а������·�����Ӧ�ļ�ֵ
	u8 sta = 0;										//��¼��ֵ
    u8 t1 = 0, t2 = 0;								//t1��¼��ַ�룬t2��¼��ַ����
	if(RmtSta & (1<<6)){							//�õ�һ��������������Ϣ��
	    t1 = RmtRec >> 24;							//�õ���ַ��
	    t2 = (RmtRec >> 16) & 0xFF;					//�õ���ַ���� 
 	    if((t1 == (u8)~t2) && t1 == REMOTE_ID){		//����ң��ʶ����(ID)����ַ
	        t1 = RmtRec>>8;
	        t2 = RmtRec;
	        if(t1 == (u8)~t2)
				sta = t1;							//��ֵ��ȷ
		}
		if((sta == 0) || ((RmtSta & 0x80) == 0))	//�������ݴ���/ң���Ѿ�û�а�����
		 	RmtSta &= ~(1<<6);						//������յ���Ч������ʶ
	}
    return sta;
}

void Remote(void){									//����ң��ģ�鹦�ܺ���
	extern u8 ex_mode;
	u8 key;
	Remote_Init();												//������ճ�ʼ��
	while(ex_mode == MODE_OFF){
		key = Remote_Scan();									//��ȡ����ң�صļ�ֵ
		switch(key){
			case 0:												//û�а���
				Car_Turn_Speed_pwm(STOP, Remote_Speed0, Remote_Speed0);
				break;
			case 104:											//1����ת��
				Car_Turn_Speed_pwm(TURN_LIFT, Remote_Speed1, Remote_Speed1);
				break;
			case 152:											//2��ֱ��
				Car_Turn_Speed_pwm(STRAIGHT, Remote_Speed2, Remote_Speed0);
				break;
			case 176:											//3����ת��
				Car_Turn_Speed_pwm(TURN_RIGHT, Remote_Speed1, Remote_Speed1);
				break;
			case 48:											//4��ԭ����ת
				Car_Turn_Speed_pwm(TURN_LIFT, Remote_Speed0, Remote_Speed2);
				break;
			case 24:											//5��������
				Buzzer(1, 10);
				break;
			case 122:											//6��ԭ����ת
				Car_Turn_Speed_pwm(TURN_RIGHT, Remote_Speed0, Remote_Speed2);
				break;
			case 16:											//7�����ת��
				Car_Turn_Speed_pwm(BACK_LIFT, Remote_Speed1, Remote_Speed1);
				break;
			case 56:											//8������
				Car_Turn_Speed_pwm(BACK, Remote_Speed2, Remote_Speed0);
				break;
			case 90:											//9���Һ�ת��
				Car_Turn_Speed_pwm(BACK_RIGHT, Remote_Speed1, Remote_Speed1);
				break;
			case 162:
				ex_mode = MODE_ON;
		}
		delay_ms(10);
	}
	Car_Turn_Speed_pwm(STOP, Remote_Speed0, Remote_Speed0);
	return;
}
