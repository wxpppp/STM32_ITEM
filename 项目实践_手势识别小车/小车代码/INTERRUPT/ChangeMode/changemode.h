#ifndef CHANGEMODE_H
#define CHANGEMODE_H

#include "sys.h"

//С��ģʽ
#define MODE_NUMBER		7	//ģʽ����
#define MODE_STANDBY	0	//����
#define MODE_CHECK		1	//�Լ�
#define MODE_COPYCAT	2	//��ƨ��
#define MODE_ULTRASONIC	3	//������
#define MODE_INFRARED	4	//����ѭ��
#define MODE_REMOTE		5	//����ң��
#define MODE_BLUETOOTH	6	//��������ң��
//ѡ��ģʽ��־
#define MODE_OFF		0	//ѡ��ر�
#define MODE_ON			1	//ѡ���������������ж���Ӧ��ֹͣ

void ChangeMode_Init(void);	//ģʽ�ı䣬ʹWKUP(PA0)Ϊ�ж����룬����ģʽѡ�񣬳�ʼ������
void ChangeMode(void);		//ѡ��ģʽ���ı�ȫ�ֱ���mode��ֵ
u8 KEY1_Scan(u8 set);		//ɨ��KEY1�Ƿ��£����·���1�����򷵻�0��setΪ1ʱ����������Ϊ0ʱ�ر�����

#endif
