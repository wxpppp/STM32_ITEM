#include "changemode.h"
#include "sys.h"
#include "delay.h"
#include "key.h"
#include "nixietube.h"
#include "buzzer.h"

void ChangeMode_Init(void){
	Ex_NVIC_Config(GPIO_A, 0, RTIR);	//����WKUP(PA0)Ϊ�����ش���
	MY_NVIC_Init(2, 2, EXTI0_IRQn, 2);	//����WKUP(PA0)�жϣ���ռ2����Ӧ2����2
	
	return ;
}

void EXTI0_IRQHandler(void){			//�ⲿ�ж�0�������WKUP�����жϣ�����ģʽѡ������ֹͣ
	extern u8 ex_mode;
	extern u8 mode;
	
	delay_ms(10);						//����
	if(WKUP == 1){						//���WKUP���£�������˳�ģʽѡ���ٸ�����Ӧ����
		Buzzer(1, 10);					//������
		ex_mode = !ex_mode;				//ex_mode��ת
	}
	EXTI->PR = 1<<0;					//����жϷ�����־λ
	
	return ;
}

void ChangeMode(void){					//ѡ��ģʽ���ı�ȫ�ֱ���mode��ֵ
	extern u8 mode;
	extern u8 ex_mode;
	u8 i = MODE_STANDBY;				//��ʱ����mode
	
	NixieTube(i);						//��ʾ��ǰģʽ��Ĭ�ϴ�MODE_STANDBY��ʼ
	while(ex_mode == MODE_ON){			//��WKUP���£������ⲿ�ж����ı�ex_mode
		if(1 == KEY1_Scan(0)){			//ɨ��KEY1�Ƿ��£�����i��һ
			Buzzer(1, 10);				//������
			i++;
			NixieTube(i % MODE_NUMBER);	//��ʾ��ǰiָ���ģʽ0-MODE_NUMBER
		}
		else
			delay_ms(10);
	}
	mode = i % MODE_NUMBER;				//�����i��ֵ��ģʽmode
	
	return ;
}

u8 KEY1_Scan(u8 set){					//ɨ��KEY1�Ƿ��£����·���1�����򷵻�0��setΪ1ʱ����������Ϊ0ʱ�ر�����
	static u8 key_up = 1;				//����KEY1�ɿ���־
	
	if(set)
		key_up = 1;	
	if(key_up && KEY1 == 0){
		delay_ms(10);					//����
		key_up = 0;						//������־��ʾKEY1����
		return 1;
	}
	else if(KEY1 == 1)					//KEY1δ����
		key_up = 1;
	
	return 0;							//�ް�������
}
