#include "pwm.h"

void PWM_Init(u16 arr, u16 psc){					//TIM4_CH1(PB6)与TIM4_CH2(PB7)的pwm初始化
	RCC->APB1ENR |= 1<<2;							//TIM4时钟使能
	RCC->APB2ENR |= 1<<3;							//PB时钟使能
	GPIOB->CRL &= 0x00FF0000;						//TIM4_CH1(PB6)与TIM4_CH2(PB7)设置位刷零
	GPIOB->CRL |= 0xBB003333;						//TIM4_CH1(PB6)与TIM4_CH2(PB7)设置为复用推挽输出，最大速度为50MHz
	
	TIM4->ARR = arr;								//设定计数器重装载值
	TIM4->PSC = psc;								//预分频器设置
	
	TIM4->CCMR1 |= 6<<4;							//OC1M：设置为pwm1模式
	TIM4->CCMR1 |= 6<<12;							//OC2M：设置为pwm1模式
	TIM4->CCMR1 |= 1<<3;							//OC1PE：输出比较预装载使能
	TIM4->CCMR1 |= 1<<11;							//OC2PE：输出比较预装载使能
	TIM4->CCER |= 1<<0;								//CC1E：开启输出(CH1)到对应引脚(PB6)
	TIM4->CCER |= 1<<4;								//CC2E：开启输出(CH2)到对应引脚(PB7)
	TIM4->CR1 |= 1<<7;								//ARPE：自动重装载预装载允许
	TIM4->CR1 |= 1<<0;								//CEN：使能计数器
	
	return ;
}

void Wheel_Turn_Pwm(u8 wheel,u8 direction,u16 pwm){	//wheel轮子，向direction方向转，以及相应pwm电压
	if(wheel == RIGHT_WHEEL){						//右轮A
		switch(direction){
			case STOP:								//停止
				AIN1 = AIN2 = 0;
				PWM_RIGHT_A = pwm;
				break;
			case STRAIGHT:							//正转
				AIN1 = 1;
				AIN2 = 0;
				PWM_RIGHT_A = pwm;
				break;
			case BACK:								//反转
				AIN1 = 0;
				AIN2 = 1;
				PWM_RIGHT_A = pwm;
				break;
		}
	}
	else if(wheel == LIFT_WHEEL){					//左轮B
		switch(direction){
			case STOP:								//停止
				BIN1 = BIN2 = 0;
				PWM_LIFT_B = pwm;
				break;
			case STRAIGHT:							//正转
				BIN1 = 1;
				BIN2 = 0;
				PWM_LIFT_B = pwm;
				break;
			case BACK:								//反转
				BIN1 = 0;
				BIN2 = 1;
				PWM_LIFT_B = pwm;
				break;
		}
	}
	
	return ;
}

void Car_Turn_Speed_pwm(u8 turn, u16 speed, u16 pwm){				//车向turn转，两个轮子的基础速度speed，转向时两轮的速度差
	switch(turn){
		case STOP:													//停止，两个轮子挂空，pwm保持基础速度
			Wheel_Turn_Pwm(RIGHT_WHEEL, STOP, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, STOP, speed);
			break;
		case STRAIGHT:												//直走，两个轮子正转，pwm为基础速度
			Wheel_Turn_Pwm(RIGHT_WHEEL, STRAIGHT, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, STRAIGHT, speed);
			break;
		case BACK:													//后退，两个轮子反转，pwm为基础速度
			Wheel_Turn_Pwm(RIGHT_WHEEL, BACK, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, BACK, speed);
			break;
		case TURN_RIGHT:											//右转，两个轮子正转，左轮pwm为基础速度加pwm
			Wheel_Turn_Pwm(RIGHT_WHEEL, STRAIGHT, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL , STRAIGHT, speed + pwm);
			break;
		case TURN_LIFT:												//左转，两个轮子正传，右轮pwm为基础速度加pwm
			Wheel_Turn_Pwm(RIGHT_WHEEL, STRAIGHT, speed + pwm);
			Wheel_Turn_Pwm(LIFT_WHEEL, STRAIGHT, speed);
			break;
		case BACK_RIGHT:											//右退，两个轮子反转，左轮pwm为基础速度加pwm
			Wheel_Turn_Pwm(RIGHT_WHEEL, BACK, speed);
			Wheel_Turn_Pwm(LIFT_WHEEL, BACK, speed + pwm);
			break;
		case BACK_LIFT:												//左退，两个轮子反转，右轮pwm为基础速度加pwm
			Wheel_Turn_Pwm(RIGHT_WHEEL, BACK, speed + pwm);
			Wheel_Turn_Pwm(LIFT_WHEEL, BACK, speed);
			break;
	}
	
	return ;
}
