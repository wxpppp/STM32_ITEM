#include "ultrasonic.h"
#include "changemode.h"
#include "pwm.h"

static struct Capture_Register cap;					//ultrasonic.c文件的全局捕获描述对象

void Ultrasonic(void){								//超声波避障功能函数
	extern u8 ex_mode;
	while(ex_mode == MODE_OFF){
		Ultrasonic_Work();
		if(Capture_Register_Empyt()){				//捕获到一次高电平
			if(Capture_Register_Display() <= Limit_Distance){//判断距前方障碍物的距离是否小于限制距离
				if(cap.times & 1)
					Car_Turn_Speed_pwm(BACK_LIFT, MAX_SPEED / 2 - 1, MAX_SPEED / 2);
				else
					Car_Turn_Speed_pwm(BACK_RIGHT, MAX_SPEED / 2 - 1, MAX_SPEED / 2);
				delay_ms(100);
			}
			Capture_Register_Reset();				//捕获重置
		}
		Car_Turn_Speed_pwm(STRAIGHT, MAX_SPEED - 1000, 0);	//全速直行
	}
	Car_Turn_Speed_pwm(STOP, 0, 0);					//退出时小车复位
	Capture_Register_Reset();
	return;
}

void Trig_Init(void){								//PA12初始化，推挽输出，最大50MHz
	RCC->APB2ENR |= 1<<2;							//使能PA时钟
	GPIOA->CRH &= 0xFFF0FFFF;						//PA12设置清零
	GPIOA->CRH |= 0x00030000;						//PA12设置为通用推挽输出
	Trig = 0;
	return;
}

void Echo_Init(void){								//PA11初始化，上拉下拉输入模式
	RCC->APB2ENR |= 1<<2;							//使能PA时钟
	GPIOA->CRH &= 0xFFFF0FFF;						//PA11设置清零
	GPIOA->CRH |= 0x00008000;						//PA11设置为上拉下拉输入模式
	PAin(11) = 0;									//下拉
	return;
}

void TIM1_CH4_Init(u16 arr, u16 psc){				//TIM1_CH4输入捕获初始化
	RCC->APB2ENR |= 1<<11;							//TIM1时钟使能
	
	TIM1->ARR = arr;								//设定计数器的自动重装载值
	TIM1->PSC = psc;								//预分频
	
	TIM1->CCMR2 &= 0x00FF;							//TIM1->CCMR2通道4设置清零
	TIM1->CCMR2 |= 1<<8;							//CC4S设置为01，IC4映射到TI4上
	TIM1->CCMR2 |= 0<<10;							//IC4PSC设置为00，无预分频
	TIM1->CCMR2 |= 0<<12;							//IC4F设置为0000，无滤波器
	
	TIM1->CCER &= ~(1<<13);							//CC4P设置为0，上升沿捕获
	TIM1->CCER |= 1<<12;							//CC4E设置为1，捕获使能
	
	TIM1->DIER |= 1<<4;								//允许捕获4中断
	TIM1->DIER |= 1<<0;								//允许更新中断
	
	TIM1->CR1 |= 1<<0;								//使能TIM1定时器
	MY_NVIC_Init(2, 0, TIM1_CC_IRQn, 2);			//抢占2，子优先级0，组2
	return;
}

void Ultrasonic_Init(void){							//超声波模块的初始化
	Trig_Init();									//Trig(PA12)初始化，推挽输出，最大50MHz
	Echo_Init();									//Echo(PA11)初始化，上拉下拉输入
	TIM1_CH4_Init(0xFFFF, 72 - 1);					//TIM1_CH4初始化
	Capture_Register_Reset();						//捕获描述对象清零
	uart_init(72, 9600);							//串口初始化
	return;
}

void Ultrasonic_Work(void){							//给超声波模块一个激励信号，使其发射超声波
	Trig = 0;
	Trig = 1;
	delay_us(16);									//使用软件延时控制激励时间
	Trig = 0;
	return;
}

void Capture_Register_Reset(void){					//Capture_Register清零
	cap.flag1 = cap.flag2 = cap.times = cap.value = 0;
	return;
}

unsigned char Capture_Register_Empyt(void){			//判断是否完成一次完整的输入捕获，即返回cap.flag1的值
	return cap.flag1;
}

u32 Capture_Register_Display(void){					//向串口打印距离数据并返回距离数据
	u32 temp = ((cap.times * 65536 + cap.value) * Sound_Velocity) / 2;
	//printf("%d cm\r\n", temp);						//向串口打印距离
	return temp;
}

void TIM1_CC_IRQHandler(void){						//TIM1_CH4捕获中断服务函数
	u16 tsr = TIM1->SR;
	if(cap.flag1 == 0){								//未完成一次中断捕获
		if(tsr & 0x01 && cap.flag2){				//发生溢出中断，并且已经捕获到上升沿处于高电平状态
			if(cap.times == 0xFF){					//高电平时间太长
				cap.flag1 = 1;
				cap.value = 0xFFFF;
			}
			else									//溢出次数加一
				cap.times++;
		}
		if(tsr & 0x10){								//捕获4发生捕获事件
			if(cap.flag2 == 0){						//捕获到上升沿
				Capture_Register_Reset();
				cap.flag2 = 1;						//开始一次输入捕获，已经捕获到上升沿，等待下降沿
				TIM1->CNT = 0;						//计数器清零
				TIM1->CCER |= 1<<13;				//CC4P设置为下降沿捕获
			}
			else{									//捕获到下降沿
				cap.flag1 = 1;						//完成一次输入捕获
				cap.value = TIM1->CCR4;				//获取当前捕获值
				TIM1->CCER &= ~(1<<13);				//CC4P重新设置为上升沿捕获
			}
		}
	}
	TIM1->SR = 0;									//清除中断标志位
	return;
}
