#ifndef _LED_H
#define _LED_H

#include "sys.h"
#include "stm32f10x.h"

#define LED	PCout(13)			//PC13ΪLED
#define ON 0
#define OFF 1

void LED_Init(void);			//LED��ʼ��

#endif
