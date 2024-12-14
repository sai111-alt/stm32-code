#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h" // Device header
#include "Delay.h"

#define k1 GPIO_Pin_6
#define k2 GPIO_Pin_4
#define k3 GPIO_Pin_2
#define k4 GPIO_Pin_0

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
