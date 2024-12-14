#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h" // Device header
#include "Delay.h"

#define k1 GPIO_Pin_6
#define k2 GPIO_Pin_4
#define k3 GPIO_Pin_2
#define k4 GPIO_Pin_0

#define Key1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)
#define Key2 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)
#define Key3 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)
#define Key4 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

void Key_Init(void);
uint8_t Key_GetNum(uint8_t *KeyFlag);

#endif
