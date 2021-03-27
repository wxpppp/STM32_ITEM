#include "changemode.h"
#include "sys.h"
#include "delay.h"
#include "key.h"
#include "nixietube.h"
#include "buzzer.h"

void ChangeMode_Init(void){
	Ex_NVIC_Config(GPIO_A, 0, RTIR);	//设置WKUP(PA0)为上升沿触发
	MY_NVIC_Init(2, 2, EXTI0_IRQn, 2);	//设置WKUP(PA0)中断，抢占2，响应2，组2
	
	return ;
}

void EXTI0_IRQHandler(void){			//外部中断0服务程序，WKUP按键中断，进入模式选择，其余活动停止
	extern u8 ex_mode;
	extern u8 mode;
	
	delay_ms(10);						//消抖
	if(WKUP == 1){						//如果WKUP按下，进入或退出模式选择，再给出相应反馈
		Buzzer(1, 10);					//按键音
		ex_mode = !ex_mode;				//ex_mode反转
	}
	EXTI->PR = 1<<0;					//清除中断发生标志位
	
	return ;
}

void ChangeMode(void){					//选择模式，改变全局变量mode的值
	extern u8 mode;
	extern u8 ex_mode;
	u8 i = MODE_STANDBY;				//暂时代替mode
	
	NixieTube(i);						//显示当前模式，默认从MODE_STANDBY开始
	while(ex_mode == MODE_ON){			//当WKUP按下，输入外部中断来改变ex_mode
		if(1 == KEY1_Scan(0)){			//扫描KEY1是否按下，按下i加一
			Buzzer(1, 10);				//按键音
			i++;
			NixieTube(i % MODE_NUMBER);	//显示当前i指向的模式0-MODE_NUMBER
		}
		else
			delay_ms(10);
	}
	mode = i % MODE_NUMBER;				//将间接i赋值给模式mode
	
	return ;
}

u8 KEY1_Scan(u8 set){					//扫描KEY1是否按下，按下返回1，否则返回0，set为1时开启连按，为0时关闭连按
	static u8 key_up = 1;				//按键KEY1松开标志
	
	if(set)
		key_up = 1;	
	if(key_up && KEY1 == 0){
		delay_ms(10);					//消抖
		key_up = 0;						//按键标志表示KEY1按下
		return 1;
	}
	else if(KEY1 == 1)					//KEY1未按下
		key_up = 1;
	
	return 0;							//无按键按下
}
