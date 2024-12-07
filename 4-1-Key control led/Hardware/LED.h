#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h" // Device header
#include "Delay.h"

void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED1_Turn(void);
void LED2_Turn(void);

#endif
