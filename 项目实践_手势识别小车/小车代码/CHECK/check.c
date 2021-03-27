#include "check.h"
#include "pwm.h"
#include "changemode.h"
#include "delay.h"
#include "buzzer.h"

void Check(void){									//�Լ�ģʽ
	extern u8 ex_mode;
	u16 speed = 0;
	
	//delay_ms(2000);									//��ʱ2���ʼ�Լ�
	
	while(ex_mode == MODE_OFF){						//ֻҪ�������ⲿ�жϣ�ex_modeʼ��ΪM0DE_OFF���������ⲿ�жϣ����˳���ѭ�������ص���������ģʽѡ����
		while(speed++ < MAX_SPEED && ex_mode == MODE_OFF){
			delay_ms(1);
			Car_Turn_Speed_pwm(STRAIGHT, speed, 0);
		}
		Car_Turn_Speed_pwm(TURN_RIGHT, speed - 4000, 4000);
		delay_ms(2000);
		Car_Turn_Speed_pwm(TURN_LIFT, speed - 4000, 4000);
		delay_ms(2000);
		//delay_ms(5000);								//���ӷ�ת��ʱ��
		speed = STOP;
		while(speed++ < MAX_SPEED && ex_mode == MODE_OFF){
			delay_ms(1);
			Car_Turn_Speed_pwm(BACK, speed, 0);
		}
		speed = STOP;
		delay_ms(5000);								//���ӷ�ת��ʱ��			
	}
	Car_Turn_Speed_pwm(STOP, STOP, 0);				//С����λ��ȫ��ͣ����
	
	return ;
}

void Check1(void){									//�Լ�ģʽ2��ԭ�ش�ת
	extern u8 ex_mode;
	u16 speed = 0;
	
	while(ex_mode == MODE_OFF){
		while(speed++ < MAX_SPEED && ex_mode == MODE_OFF){
			Wheel_Turn_Pwm(RIGHT_WHEEL, STRAIGHT, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, BACK, speed);
			delay_ms(1);
		}
		delay_ms(2000);
		while(speed-- > 500 && ex_mode == MODE_OFF){
			Wheel_Turn_Pwm(RIGHT_WHEEL, STRAIGHT, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, BACK, speed);
			delay_ms(1);
		}
		Buzzer(2, 100);
		while(speed++ < MAX_SPEED && ex_mode == MODE_OFF){
			Wheel_Turn_Pwm(RIGHT_WHEEL, BACK, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, STRAIGHT, speed);
			delay_ms(1);
		}
		delay_ms(2000);
		while(speed-- > 500 && ex_mode == MODE_OFF){
			Wheel_Turn_Pwm(RIGHT_WHEEL, BACK, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, STRAIGHT, speed);
			delay_ms(1);
		}
		Buzzer(2, 100);
	}
	Car_Turn_Speed_pwm(STOP, STOP, 0);				//С����λ��ȫ��ͣ����
	return ;
}
