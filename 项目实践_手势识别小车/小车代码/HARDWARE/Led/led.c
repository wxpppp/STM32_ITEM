#include "led.h"
#include "sys.h"
#include "delay.h"
#include "nixietube.h"
#include "changemode.h"

void LED_Init(void){						//LED0(PC8)，LED1(PC9)初始化，通过定时器(TIM3)中断控制两个灯的亮灭，反馈系统状态
	RCC->APB2ENR |= 1<<4;					//PC时钟使能
	GPIOC->CRH &= 0xFFFFFF00;				//LED0(PC8)LED1(PC9)设置位刷零
	GPIOC->CRH |= 0x00000033;				//LED0(PC8)LED1(PC9)设置为通用推挽输出，最大50MHz
	LED0 = LED1 = Led_On;
	
	return ;
}

void TIM3_Int_Init(u16 arr, u16 psc){		//通用定时器3初始化，时钟选择为APB1(36MHz)的2倍72MHz，arr自动重装载值，psc时钟预分频数
	RCC->APB1ENR |= 1<<1;					//TIM3时钟使能
	TIM3->ARR = arr;						//设定计数器自动重装载值，刚好1ms
	TIM3->PSC = psc;						//预分频器7200，得到10KHz的计数时钟
	TIM3->DIER |= 1<<0;						//允许更新中断
	MY_NVIC_Init(2, 3, TIM3_IRQn, 2);		//设置定时器中断，抢占2，响应3，组2
	TIM3->CR1 |= 0x0001;					//使能计数器3	
	
	return ;
}

void TIM3_IRQHandler(void){								//TIM3时钟中断服务程序，LED0(PC8)LED1(PC9)闪烁控制
	extern u8 ex_mode;									//模式选择控制，为1时，进入选择模式，为0时，退出选择模式
	extern u8 mode;										//当前模式标志
	
	if(TIM3->SR & 0x0001){								//获取中断标志位，是否发生中断
		if(ex_mode == MODE_OFF && mode != MODE_STANDBY){//执行程序模式，LED0(PC8)LED1(PC9)交替闪
			switch(LED1 * 2 + LED0 * 1){				//采用加权编码，两个LED输入，共四个编码，通过编码实现转换
				case 0:									//偏离态
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
				case 3:									//偏离态
					LED0 = Led_Off;
					break;
			}
		}
		else if(ex_mode == MODE_ON)						//选择模式，LED0(PC8)LED1(PC9)常亮
			LED0 = LED1 = Led_On;
		else{											//待机模式，LED0(PC8)LED1(PC9)双闪，如果恰好为程序退出模式，什么都不会执行
			switch(LED1 * 2 + LED0 * 1){				//采用加权编码，两个LED输入，共四个编码，通过编码实现转换
				case 0:
					LED0 = LED1 = Led_On;
					break;
				case 1:									//偏离态
					LED0 = Led_Off;
					break;
				case 2:									//偏离态
					LED0 = Led_On;
					break;
				case 3:
					LED0 = LED1 = Led_Off;
					break;
			}
		}
	}
	TIM3->SR &= ~(1<<0);								//清除中断标志位
	
	return ;
}
