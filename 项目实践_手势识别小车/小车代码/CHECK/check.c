#include "check.h"
#include "pwm.h"
#include "changemode.h"
#include "delay.h"
#include "buzzer.h"

void Check(void){									//自检模式
	extern u8 ex_mode;
	u16 speed = 0;
	
	//delay_ms(2000);									//延时2秒后开始自检
	
	while(ex_mode == MODE_OFF){						//只要不发生外部中断，ex_mode始终为M0DE_OFF，当发生外部中断，将退出该循环，返回到主函数的模式选择中
		while(speed++ < MAX_SPEED && ex_mode == MODE_OFF){
			delay_ms(1);
			Car_Turn_Speed_pwm(STRAIGHT, speed, 0);
		}
		Car_Turn_Speed_pwm(TURN_RIGHT, speed - 4000, 4000);
		delay_ms(2000);
		Car_Turn_Speed_pwm(TURN_LIFT, speed - 4000, 4000);
		delay_ms(2000);
		//delay_ms(5000);								//轮子反转留时间
		speed = STOP;
		while(speed++ < MAX_SPEED && ex_mode == MODE_OFF){
			delay_ms(1);
			Car_Turn_Speed_pwm(BACK, speed, 0);
		}
		speed = STOP;
		delay_ms(5000);								//轮子反转留时间			
	}
	Car_Turn_Speed_pwm(STOP, STOP, 0);				//小车复位，全部停下来
	
	return ;
}

void Check1(void){									//自检模式2，原地打转
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
	Car_Turn_Speed_pwm(STOP, STOP, 0);				//小车复位，全部停下来
	return ;
}
