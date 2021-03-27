#include "infrared.h"

void Infrared_Init(void){						//����ѭ��ģ���ʼ��
	RCC->APB2ENR |= 1<<3;						//PBʱ��ʹ��
	GPIOB->CRH &= 0xFF00FFFF;					//PB8-PB11����λ����
	GPIOB->CRH |= 0x00880000;					//PB8-PB11����Ϊ��/��������
	GPIOB->IDR &= 0xFF00FFFF;					//PB8-PB11����
	return;
}

void Infrared(void){									//����ѭ��ģ�麯��
	extern u8 ex_mode;
	while(ex_mode == MODE_OFF){
		switch(8*B_R + 4*A_R + 2*A_L + 1*B_L){				//ʹ��8421��Ȩֵ����
			case 0:
				Car_Turn_Speed_pwm(STRAIGHT, SPEED4, SPEED0);
				delay_ms(DELAY_TIME);
				break;
			case 1:
				Car_Turn_Speed_pwm(TURN_LIFT, SPEED0, SPEED6);
				break;
			case 2:
				Car_Turn_Speed_pwm(TURN_LIFT, SPEED2, SPEED5);
				break;
			//case 3:break;
			case 4:
				Car_Turn_Speed_pwm(TURN_RIGHT, SPEED2, SPEED5);
				break;
			//case 5:break;
			case 6:
				Car_Turn_Speed_pwm(STRAIGHT, SPEED2, SPEED0);
				break;
			//case 7:break;
			case 8:
				Car_Turn_Speed_pwm(TURN_RIGHT, SPEED0, SPEED6);
				delay_ms(DELAY_TIME);
				break;
			//case 9:break;
			//case 10:break;
			//case 11:break;
			//case 12:break;
			//case 13:break;
			//case 14:break;
			//case 15:break;
			default:
				Car_Turn_Speed_pwm(STRAIGHT, SPEED2, SPEED0);
		}
	}
	Car_Turn_Speed_pwm(STOP, 0, 0);						//����С��ֹͣ
	return;
}
