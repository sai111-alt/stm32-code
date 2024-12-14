#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h" // Device header

#define L1 GPIO_Pin_14
#define L2 GPIO_Pin_15

void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED1_Turn(void);
void LED2_Turn(void);

#endif
