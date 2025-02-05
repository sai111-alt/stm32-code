#ifndef __PWM_DC_MOTOR_H__
#define __PWM_DC_MOTOR_H__

#include "stm32f10x.h" // Device header

void PWM_DCM_Init(void);
void Motor_SetSpeed(int8_t Speed);
void PWM_SetCompare1(uint16_t Compare);

#endif
