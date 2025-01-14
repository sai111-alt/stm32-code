#ifndef __LIGHTSENSOR_H__
#define __LIGHTSENSOR_H__

#include "stm32f10x.h" // Device header

void LightSensor_Init(void);
uint8_t LightSensor_Get(void);

#endif
