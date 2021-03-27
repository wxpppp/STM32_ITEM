#include "bluetooth.h"

void Bluetooth_Init(void){								//蓝牙手势遥控模块初始化
	USART2_Init(36, 9600);								//串口2初始化，波特率为9600
	return;
}

void Bluetooth(void){									//蓝牙手势遥控功能函数
	extern u8 ex_mode;
	int i = 0, flag = 0;								//计数器，正负数标志(0正1负)
	int left = 0, right = 0;							//左右两轮的速度值
	while(ex_mode == MODE_OFF){
		if(USART2_RX_STA & 0x8000){						//接收到一次数据
			USART2_RX_BUF[USART2_RX_STA & 0x7FFF] = '\0';//加入结束符
			i = flag = 0;
			if(USART2_RX_BUF[i] != '@'){
				printf("Error[1]");
				continue;
			}
			if(USART2_RX_BUF[++i] == '-')
				i += (flag = 1);
			for(left = 0; USART2_RX_BUF[i] != '#'; ++i)
				left = left * 10 + (USART2_RX_BUF[i] - '0');
			if(flag){
				left = -left;
				flag = 0;
			}
			if(USART2_RX_BUF[++i] == '-')
				i += (flag = 1);
			for(right = 0; USART2_RX_BUF[i] != '$'; ++i)
				right = right * 10 + (USART2_RX_BUF[i] - '0');
			if(flag)
				right = -right;
			left *= 2;
			right *= 2;
			left < 0 ? Wheel_Turn_Pwm(LIFT_WHEEL, BACK, -left + BLUE_FIX) : Wheel_Turn_Pwm(LIFT_WHEEL, STRAIGHT, left + BLUE_FIX);
			right < 0 ? Wheel_Turn_Pwm(RIGHT_WHEEL, BACK, -right + BLUE_FIX) : Wheel_Turn_Pwm(RIGHT_WHEEL, STRAIGHT, right + BLUE_FIX);
			//printf("left:%d\t\tright:%d\r\n", left, right);
			//printf("[%d]\t%s\r\n", i, USART2_RX_BUF);
			USART2_RX_STA = 0;
		}
	}
	Car_Turn_Speed_pwm(STOP, 0, 0);						//返回时停车
	return;
}





