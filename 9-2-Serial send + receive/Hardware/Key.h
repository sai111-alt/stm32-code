#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h" // Device header
#include "Delay.h"

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
