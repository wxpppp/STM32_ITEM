#include "remote.h"

//遥控器接收状态
//RmtSta[7]:收到了引导码标志
//RmtSta[6]:得到了一个按键的所有信息
//RmtSta[5]:保留	
//RmtSta[4]:标记上升沿是否已经被捕获								   
//RmtSta[3:0]:溢出计时器
static u8 RmtSta = 0;	  	  
static u16 Dval;									//下降沿时计数器的值
static u32 RmtRec = 0;								//红外接收到的数据

void Remote_Init(void){								//红外遥控初始化
	RCC->APB2ENR |= 1<<2;    						//使能PA时钟
	GPIOA->CRL &= 0xFFFFFF0F;						//PA1设置位清零
	GPIOA->CRL |= 0x00000080;						//PA1设置为上/下拉输入
	GPIOA->ODR |= 1<<1;								//PA1上拉
	
	RCC->APB1ENR |= 1<<3;							//TIM5时钟使能
 	TIM5->ARR = 10000;								//设定计数器自动重装值，最大10ms溢出
	TIM5->PSC = 71;									//预分频器，1M的计数频率，1us加1
	
	TIM5->CCMR1 |= 1<<8;							//CC2S=01，选择输入端，IC2映射到TI2上
 	TIM5->CCMR1 |= 3<<12;							//IC2F=0011，配置输入滤波器，8个定时器时钟周期滤波
 	TIM5->CCMR1 |= 0<<10;							//IC2PS=00，配置输入分频,不分频
	
	TIM5->CCER |= 0<<5;								//CC2P=0，上升沿捕获
	TIM5->CCER |= 1<<4;								//CC2E=1，允许捕获计数器的值到捕获寄存器中
	TIM5->DIER |= 1<<2;								//允许CC2IE捕获中断
	TIM5->DIER |= 1<<0;								//允许更新中断
	TIM5->CR1 |= 0x01;								//使能定时器5
  	MY_NVIC_Init(1, 3, TIM5_IRQn, 2);				//抢占1，子优先级3，组2
	return;
}

void TIM5_IRQHandler(void){							//TIM5中断服务函数
	u16 tsr = TIM5->SR;								//获取TIM5的SR
	if(tsr & 0x01){									//溢出
		if(RmtSta & 0x80){							//上次有数据被接收到了
			RmtSta &= ~0x10;						//取消上升沿已经被捕获标记
			if((RmtSta & 0x0F) == 0x00)				//标记已经完成一次按键的键值信息采集
				RmtSta |= 1<<6;
			if((RmtSta & 0x0F) < 14)
				++RmtSta;
			else{
				RmtSta&=~(1<<7);					//清空引导标识
				RmtSta&=0xF0;						//清空计数器	
			}
		}
	}
	if(tsr & (1<<2)){								//CC2IE中断
		if(RDATA){									//上升沿捕获
  			TIM5->CCER |= 1<<5;						//CC2P=1，设置为下降沿捕获
			TIM5->CNT = 0;							//清空定时器值
			RmtSta |= 0x10;							//标记上升沿已经被捕获
		}
		else{										//下降沿捕获
			Dval = TIM5->CCR2;						//读取CCR1也可以清CC2IF标志位
  			TIM5->CCER &= ~(1<<5);					//CC2P=0，设置为上升沿捕获
			if(RmtSta & 0x10){						//完成一次高电平捕获
 				if(RmtSta & 0x80){					//接收到了引导码
					if(Dval > 300 && Dval < 800){	//560为标准值，560us
						RmtRec <<= 1;				//左移一位
						RmtRec |= 0;				//接收到0
					}
					else if(Dval > 1400 && Dval < 1800){//1680为标准值，1680us
						RmtRec <<= 1;				//左移一位
						RmtRec |= 1;				//接收到1
					}
					else if(Dval > 2200 && Dval < 2600)//得到按键键值增加的信息，2500为标准值2.5ms
						RmtSta &= 0xF0;				//清空计时器
 				}
				else if(Dval > 4200 && Dval < 4700)	//4500为标准值4.5ms
					RmtSta |= 1<<7;					//标记成功接收到了引导码
			}
			RmtSta &= ~(1<<4);
		}
	}
	TIM5->SR = 0;									//清除中断标志位
	return;
}

u8 Remote_Scan(void){								//扫描红外键盘，没任何按键按下返回0，若有按键按下返回相应的键值
	u8 sta = 0;										//记录键值
    u8 t1 = 0, t2 = 0;								//t1记录地址码，t2记录地址反码
	if(RmtSta & (1<<6)){							//得到一个按键的所有信息了
	    t1 = RmtRec >> 24;							//得到地址码
	    t2 = (RmtRec >> 16) & 0xFF;					//得到地址反码 
 	    if((t1 == (u8)~t2) && t1 == REMOTE_ID){		//检验遥控识别码(ID)及地址
	        t1 = RmtRec>>8;
	        t2 = RmtRec;
	        if(t1 == (u8)~t2)
				sta = t1;							//键值正确
		}
		if((sta == 0) || ((RmtSta & 0x80) == 0))	//按键数据错误/遥控已经没有按下了
		 	RmtSta &= ~(1<<6);						//清除接收到有效按键标识
	}
    return sta;
}

void Remote(void){									//红外遥控模块功能函数
	extern u8 ex_mode;
	u8 key;
	Remote_Init();												//红外接收初始化
	while(ex_mode == MODE_OFF){
		key = Remote_Scan();									//获取红外遥控的键值
		switch(key){
			case 0:												//没有按下
				Car_Turn_Speed_pwm(STOP, Remote_Speed0, Remote_Speed0);
				break;
			case 104:											//1：左转弯
				Car_Turn_Speed_pwm(TURN_LIFT, Remote_Speed1, Remote_Speed1);
				break;
			case 152:											//2：直行
				Car_Turn_Speed_pwm(STRAIGHT, Remote_Speed2, Remote_Speed0);
				break;
			case 176:											//3：右转弯
				Car_Turn_Speed_pwm(TURN_RIGHT, Remote_Speed1, Remote_Speed1);
				break;
			case 48:											//4：原地左转
				Car_Turn_Speed_pwm(TURN_LIFT, Remote_Speed0, Remote_Speed2);
				break;
			case 24:											//5：蜂鸣器
				Buzzer(1, 10);
				break;
			case 122:											//6：原地右转
				Car_Turn_Speed_pwm(TURN_RIGHT, Remote_Speed0, Remote_Speed2);
				break;
			case 16:											//7：左后转弯
				Car_Turn_Speed_pwm(BACK_LIFT, Remote_Speed1, Remote_Speed1);
				break;
			case 56:											//8：后退
				Car_Turn_Speed_pwm(BACK, Remote_Speed2, Remote_Speed0);
				break;
			case 90:											//9：右后转弯
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
