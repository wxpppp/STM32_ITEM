#ifndef KEY_H
#define KEY_H

#define KEY1 PCin(5)		//将PC5定义为KEY1
#define KEY2 PAin(15)		//将PA15定义位KEY2
#define WKUP PAin(0)		//将PA0定义为WKUP

void KEY_Init(void);		//按键初始化

#endif
