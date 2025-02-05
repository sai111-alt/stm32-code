#ifndef __PWM_BREATHING_LED_H__
#define __PWM_BREATHING_LED_H__

#include "stm32f10x.h" // Device header

void PWM_LED_Init(void);
void PWM_LED_SetCompare1(uint16_t Compare);

#endif
