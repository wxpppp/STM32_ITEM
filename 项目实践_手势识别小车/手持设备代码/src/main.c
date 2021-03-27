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
	Stm32_Clock_Init(9);								//ϵͳʱ�ӳ�ʼ��
	delay_init(72);										//��ʱ��ʼ��
	uart_init(72, 9600);								//���ڳ�ʼ��
	LED_Init();											//LED��ʼ��
	Init_HC05();										//����ģ��HC05��ʼ��
	MPU_Init();											//MPU6050��ʼ��
	while(mpu_dmp_init())								//�ȴ�MPU6050��DMP��ʼ��
		delay_ms(100);
	u2_printf("Eveything is OK!\r\n");
	while(1){
		Send_Data();									//����MPU6050�⵽����ֵ
		delay_ms(200);
		LED = !LED;
	}
}
