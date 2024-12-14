#ifndef INC_DHT22_H_
#define INC_DHT22_H_

#include "stm32f10x.h"
#include "Delay.h"

#define DATA GPIO_Pin_13

void DHT22_Mode(uint8_t mode);
void DHT22_Init(void);                   // 初始化DHT22
uint8_t DHT22_Read_Data(int8_t *Array); // 读取温湿度数据

#endif
