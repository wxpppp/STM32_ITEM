/********************************************************************************************************

HC-SR04���������ģ��Ļ���ʹ�÷�����
	(1)����IO��TRIG������࣬������10us�ĸߵ�ƽ�ź�
	(2)ģ���Զ�����8��40KHz�ķ������Զ�����Ƿ����źŷ���
	(3)���źŷ��أ�ͨ��IO��ECHO���һ���ߵ�ƽ���ߵ�ƽ������ʱ����ǳ������ӷ��䵽���ص�ʱ��
	(4)���Ծ���=(�ߵ�ƽʱ��*����(340m/s))/2
	
********************************************************************************************************/

#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#define Trig PAout(12)						//������ģ����ƶˣ�����Ϊ�������
#define Echo 								//������ģ��ķ����ˣ�����Ϊ���벶��ʹ��TIM1_CH4
#define Sound_Velocity 0.034				//���٣�0.034 cm/us
#define Limit_Distance 20					//���ƾ��룬��λ����(cm)

#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

struct Capture_Register{					//��¼����״̬����
	unsigned char flag1;					//��־�Ƿ���������һ�����벶��0��ʱ���ޣ�1��ʱ����һ�������Ĳ���
	unsigned char flag2;					//��־�Ƿ���һ�������صĲ���0û�У�1��
	u8 times;								//��¼����������Ĵ�������ʱ���ԼΪtimes*65536
	u16 value;								//��¼δ�ﵽ���޵�ʱ��
};

void Ultrasonic(void);						//���������Ϲ��ܺ���
void Ultrasonic_Init(void);					//������ģ��ĳ�ʼ��
void Capture_Register_Reset(void);			//Capture_Register����
u32 Capture_Register_Display(void);			//�򴮿ڴ�ӡ�������ݲ����ؾ�������
unsigned char Capture_Register_Empyt(void);			//�ж��Ƿ����һ�����������벶�񣬼�����cap.flag1��ֵ
void TIM1_CH4_Init(u16 arr, u16 psc);		//TIM1_CH4���벶���ʼ��
void Trig_Init(void);						//PA12��ʼ����������������50MHz
void Echo_Init(void);						//PA11��ʼ����������������ģʽ
void Ultrasonic_Work(void);					//��������ģ��һ�������źţ�ʹ�䷢�䳬����

#endif
