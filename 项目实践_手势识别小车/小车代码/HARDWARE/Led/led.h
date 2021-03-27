#ifndef LED_H
#define LED_H

#include "sys.h"

#define LED0 PCout(8)					//����LED0ΪPC8����
#define LED1 PCout(9)					//����LED1ΪPC9����
#define Led_On	1
#define Led_Off	0

void TIM3_Int_Init(u16 arr, u16 psc);	//ͨ�ö�ʱ��3��ʼ����ʱ��ѡ��ΪAPB1(36MHz)��2��72MHz��arr�Զ���װ��ֵ��pscʱ��Ԥ��Ƶ��
void LED_Init(void);					//LED0(PC8)��LED1(PC9)��ʼ����ͨ����ʱ��(TIM3)�жϿ��������Ƶ����𣬷���ϵͳ״̬
										//����ʱ��˫������ѡ��ģʽʱ����������ִ��ģʽʱ��������˸��
#endif
