#ifndef __AD_H__
#define __AD_H__

#include "stm32f10x.h" // Device header

void AD_Init(void);
uint16_t ADC_GetValue(uint8_t ADC_Channel);

#endif
