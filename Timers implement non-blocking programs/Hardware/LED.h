#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h" // Device header

void LED_Init(void);
void LED1_SetMode(uint8_t Mode);
void LED2_SetMode(uint8_t Mode);
void LED_Loop(void);

#endif
