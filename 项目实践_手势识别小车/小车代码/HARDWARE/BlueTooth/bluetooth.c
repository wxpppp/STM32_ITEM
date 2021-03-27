#include "bluetooth.h"

void Bluetooth_Init(void){								//��������ң��ģ���ʼ��
	USART2_Init(36, 9600);								//����2��ʼ����������Ϊ9600
	return;
}

void Bluetooth(void){									//��������ң�ع��ܺ���
	extern u8 ex_mode;
	int i = 0, flag = 0;								//����������������־(0��1��)
	int left = 0, right = 0;							//�������ֵ��ٶ�ֵ
	while(ex_mode == MODE_OFF){
		if(USART2_RX_STA & 0x8000){						//���յ�һ������
			USART2_RX_BUF[USART2_RX_STA & 0x7FFF] = '\0';//���������
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
	Car_Turn_Speed_pwm(STOP, 0, 0);						//����ʱͣ��
	return;
}





