#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "hc05.h"
#include "usart2.h"
#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"

int main()
{
	Stm32_Clock_Init(9);								//系统时钟初始化
	delay_init(72);										//延时初始化
	uart_init(72, 9600);								//串口初始化
	LED_Init();											//LED初始化
	Init_HC05();										//蓝牙模块HC05初始化
	MPU_Init();											//MPU6050初始化
	while(mpu_dmp_init())								//等待MPU6050的DMP初始化
		delay_ms(100);
	u2_printf("Eveything is OK!\r\n");
	while(1){
		Send_Data();									//发送MPU6050测到的数值
		delay_ms(200);
		LED = !LED;
	}
}
