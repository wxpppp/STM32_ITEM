#include "pwm.h"

void PWM_Init(u16 arr, u16 psc){					//TIM4_CH1(PB6)��TIM4_CH2(PB7)��pwm��ʼ��
	RCC->APB1ENR |= 1<<2;							//TIM4ʱ��ʹ��
	RCC->APB2ENR |= 1<<3;							//PBʱ��ʹ��
	GPIOB->CRL &= 0x00FF0000;						//TIM4_CH1(PB6)��TIM4_CH2(PB7)����λˢ��
	GPIOB->CRL |= 0xBB003333;						//TIM4_CH1(PB6)��TIM4_CH2(PB7)����Ϊ�����������������ٶ�Ϊ50MHz
	
	TIM4->ARR = arr;								//�趨��������װ��ֵ
	TIM4->PSC = psc;								//Ԥ��Ƶ������
	
	TIM4->CCMR1 |= 6<<4;							//OC1M������Ϊpwm1ģʽ
	TIM4->CCMR1 |= 6<<12;							//OC2M������Ϊpwm1ģʽ
	TIM4->CCMR1 |= 1<<3;							//OC1PE������Ƚ�Ԥװ��ʹ��
	TIM4->CCMR1 |= 1<<11;							//OC2PE������Ƚ�Ԥװ��ʹ��
	TIM4->CCER |= 1<<0;								//CC1E���������(CH1)����Ӧ����(PB6)
	TIM4->CCER |= 1<<4;								//CC2E���������(CH2)����Ӧ����(PB7)
	TIM4->CR1 |= 1<<7;								//ARPE���Զ���װ��Ԥװ������
	TIM4->CR1 |= 1<<0;								//CEN��ʹ�ܼ�����
	
	return ;
}

void Wheel_Turn_Pwm(u8 wheel,u8 direction,u16 pwm){	//wheel���ӣ���direction����ת���Լ���Ӧpwm��ѹ
	if(wheel == RIGHT_WHEEL){						//����A
		switch(direction){
			case STOP:								//ֹͣ
				AIN1 = AIN2 = 0;
				PWM_RIGHT_A = pwm;
				break;
			case STRAIGHT:							//��ת
				AIN1 = 1;
				AIN2 = 0;
				PWM_RIGHT_A = pwm;
				break;
			case BACK:								//��ת
				AIN1 = 0;
				AIN2 = 1;
				PWM_RIGHT_A = pwm;
				break;
		}
	}
	else if(wheel == LIFT_WHEEL){					//����B
		switch(direction){
			case STOP:								//ֹͣ
				BIN1 = BIN2 = 0;
				PWM_LIFT_B = pwm;
				break;
			case STRAIGHT:							//��ת
				BIN1 = 1;
				BIN2 = 0;
				PWM_LIFT_B = pwm;
				break;
			case BACK:								//��ת
				BIN1 = 0;
				BIN2 = 1;
				PWM_LIFT_B = pwm;
				break;
		}
	}
	
	return ;
}

void Car_Turn_Speed_pwm(u8 turn, u16 speed, u16 pwm){				//����turnת���������ӵĻ����ٶ�speed��ת��ʱ���ֵ��ٶȲ�
	switch(turn){
		case STOP:													//ֹͣ���������ӹҿգ�pwm���ֻ����ٶ�
			Wheel_Turn_Pwm(RIGHT_WHEEL, STOP, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, STOP, speed);
			break;
		case STRAIGHT:												//ֱ�ߣ�����������ת��pwmΪ�����ٶ�
			Wheel_Turn_Pwm(RIGHT_WHEEL, STRAIGHT, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, STRAIGHT, speed);
			break;
		case BACK:													//���ˣ��������ӷ�ת��pwmΪ�����ٶ�
			Wheel_Turn_Pwm(RIGHT_WHEEL, BACK, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, BACK, speed);
			break;
		case TURN_RIGHT:											//��ת������������ת������pwmΪ�����ٶȼ�pwm
			Wheel_Turn_Pwm(RIGHT_WHEEL, STRAIGHT, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL , STRAIGHT, speed + pwm);
			break;
		case TURN_LIFT:												//��ת��������������������pwmΪ�����ٶȼ�pwm
			Wheel_Turn_Pwm(RIGHT_WHEEL, STRAIGHT, speed + pwm);
			Wheel_Turn_Pwm(LIFT_WHEEL, STRAIGHT, speed);
			break;
		case BACK_RIGHT:											//���ˣ��������ӷ�ת������pwmΪ�����ٶȼ�pwm
			Wheel_Turn_Pwm(RIGHT_WHEEL, BACK, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, BACK, speed + pwm);
			break;
		case BACK_LIFT:												//���ˣ��������ӷ�ת������pwmΪ�����ٶȼ�pwm
			Wheel_Turn_Pwm(RIGHT_WHEEL, BACK, speed + pwm);
			Wheel_Turn_Pwm(LIFT_WHEEL, BACK, speed);
			break;
	}
	
	return ;
}
