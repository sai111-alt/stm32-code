#ifndef __COUNTSENSOR_H__
#define __COUNTSENSOR_H__

#include "stm32f10x.h" // Device header

void CountSensor_Init(void);
uint16_t CountSensor_Get(void);
// 中断函数无需声明，触发后自动执行

#endif
