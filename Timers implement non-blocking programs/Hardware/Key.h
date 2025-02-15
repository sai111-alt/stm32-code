#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h" // Device header
#include "Delay.h"

#define k1 GPIO_Pin_1
#define k2 GPIO_Pin_11

#define Key1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define Key2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

void Key_Init(void);
uint8_t Key_GetNum(void);
void IndependentKey_Loop(void);

#endif
