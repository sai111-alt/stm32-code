#ifndef __PWMI_H__
#define __PWMI_H__

#include "stm32f10x.h" // Device header

void PWMI_Init(void);
uint32_t PWMI_GetFreq(void);
uint32_t PWMI_GetDuty(void);

#endif
