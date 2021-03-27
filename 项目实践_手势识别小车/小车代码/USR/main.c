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

u8 ex_mode = MODE_OFF;					//�Ƿ�ʼģʽѡ���־
u8 mode = MODE_STANDBY;					//��ǰִ�е�ģʽ

int main()
{
	Stm32_Clock_Init(9);				//ϵͳʱ�ӳ�ʼ��
	delay_init(72);						//��ʱ��ʼ��
	KEY_Init();							//������ʼ����KEY1(PC5),KEY2(PA15),WKUP(PA0)
	ChangeMode_Init();					//WKUP�ⲿ�ж������жϳ�ʼ��������ģʽѡ��
	Buzzer_Init();						//������BUZZER(PC7)��ʼ��
	NixieTube_Init();					//����ܳ�ʼ����ʹ��PA2-PA8����ʾ0-4
	LED_Init();							//LED0(PC8)��LED1(PC9)��ʼ����ͨ����ʱ��(TIM3)�жϿ��������Ƶ����𣬷���ϵͳ״̬
	TIM3_Int_Init(4999, 7199);			//����LED0(PC8)LED1(PC9)�Ķ�ʱ���жϳ�ʼ��
	Ultrasonic_Init();					//������ģ���ʼ��
	Infrared_Init();					//����ѭ��ģ���ʼ��
	PWM_Init(MAX_SPEED, 0);				//PWM��ʼ��
	Bluetooth_Init();					//����ң��ģ���ʼ��
	
	NixieTube(mode);					//��ʼ��ʾmode
	while(1){							//ָʾ����˸������˫����ѡ��ģʽ������ִ�г�������˸
		delay_ms(10);
		if(ex_mode == MODE_ON)			//ex_modeΪMODE_ON������ģʽѡ��
			ChangeMode();				//ģʽѡ��
		switch(mode){					//ִ����Ӧģʽ����ʱex_mode��ΪMODE_OFF����ÿ������ִ���У�ͨ���ⲿ�жϸ���ex_mode��ֵ���Դ��˳���������
			case MODE_STANDBY:			//0����ģʽ
				break;
			case MODE_CHECK:			//1�Լ�ģʽ
				Check1();
				break;
			case MODE_COPYCAT:			//2��ƨ��
				Copycat();
				break;
			case MODE_ULTRASONIC:		//3����������
				Ultrasonic();
				break;
			case MODE_INFRARED:			//4����Ѳ��
				Infrared();
				break;
			case MODE_REMOTE:			//5����ң��
				Remote();
				break;
			case MODE_BLUETOOTH:		//6��������ң��
				Bluetooth();
				break;
		}
		mode = MODE_STANDBY;			//ģʽ�ָ�ΪĬ�ϴ���ģʽ		
	}
}
