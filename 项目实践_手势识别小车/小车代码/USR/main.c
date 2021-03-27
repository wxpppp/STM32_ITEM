#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "key.h"
#include "changemode.h"
#include "nixietube.h"
#include "led.h"
#include "buzzer.h"
#include "pwm.h"
#include "check.h"
#include "ultrasonic.h"
#include "copycat.h"
#include "infrared.h"
#include "remote.h"
#include "bluetooth.h"

u8 ex_mode = MODE_OFF;					//是否开始模式选择标志
u8 mode = MODE_STANDBY;					//当前执行的模式

int main()
{
	Stm32_Clock_Init(9);				//系统时钟初始化
	delay_init(72);						//延时初始化
	KEY_Init();							//按键初始化，KEY1(PC5),KEY2(PA15),WKUP(PA0)
	ChangeMode_Init();					//WKUP外部中断输入中断初始化，进入模式选择
	Buzzer_Init();						//蜂鸣器BUZZER(PC7)初始化
	NixieTube_Init();					//数码管初始化，使用PA2-PA8，显示0-4
	LED_Init();							//LED0(PC8)，LED1(PC9)初始化，通过定时器(TIM3)中断控制两个灯的亮灭，反馈系统状态
	TIM3_Int_Init(4999, 7199);			//控制LED0(PC8)LED1(PC9)的定时器中断初始化
	Ultrasonic_Init();					//超声波模块初始化
	Infrared_Init();					//红外循迹模块初始化
	PWM_Init(MAX_SPEED, 0);				//PWM初始化
	Bluetooth_Init();					//蓝牙遥控模块初始化
	
	NixieTube(mode);					//初始显示mode
	while(1){							//指示灯闪烁，待机双闪，选择模式常亮，执行程序交替闪烁
		delay_ms(10);
		if(ex_mode == MODE_ON)			//ex_mode为MODE_ON，进入模式选择
			ChangeMode();				//模式选择
		switch(mode){					//执行响应模式，此时ex_mode仍为MODE_OFF，在每个程序执行中，通过外部中断更改ex_mode的值，以此退出单个程序
			case MODE_STANDBY:			//0待机模式
				break;
			case MODE_CHECK:			//1自检模式
				Check1();
				break;
			case MODE_COPYCAT:			//2跟屁虫
				Copycat();
				break;
			case MODE_ULTRASONIC:		//3超声波避障
				Ultrasonic();
				break;
			case MODE_INFRARED:			//4红外巡线
				Infrared();
				break;
			case MODE_REMOTE:			//5红外遥控
				Remote();
				break;
			case MODE_BLUETOOTH:		//6蓝牙手势遥控
				Bluetooth();
				break;
		}
		mode = MODE_STANDBY;			//模式恢复为默认待机模式		
	}
}
