#ifndef __PWM_STEERING_ENGINE_H__
#define __PWM_STEERING_ENGINE_H__

#include "stm32f10x.h" // Device header

void PWM_SE_Init(void);
void SE_SetAngle(float Angle);

#endif
