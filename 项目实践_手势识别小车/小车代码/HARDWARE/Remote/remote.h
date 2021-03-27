#ifndef _REMOTE_H
#define _REMOTE_H

#define RDATA PAin(1)							//���������
#define REMOTE_ID 0								//�趨ң����ʶ����
#define Remote_Speed0 0
#define Remote_Speed1 2500
#define Remote_Speed2 5000

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "changemode.h"
#include "pwm.h"
#include "buzzer.h"

void Remote(void);								//����ң��ģ�鹦�ܺ���
void Remote_Init(void);							//����ң�س�ʼ��
u8 Remote_Scan(void);							//ɨ�������̣�û�κΰ������·���0�����а������·�����Ӧ�ļ�ֵ

#endif
