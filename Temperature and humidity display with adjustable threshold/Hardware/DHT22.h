#ifndef INC_DHT22_H_
#define INC_DHT22_H_

#include "stm32f10x.h"
#include "Delay.h"

#define DATA GPIO_Pin_11
#define DHT22_Low GPIO_ResetBits(GPIOB, DATA)
#define DHT22_High GPIO_SetBits(GPIOB, DATA)

void DHT22_Init(void);                   // 初始化DHT22
uint8_t DHT22_Read_Data(uint8_t *Array); // 读取温湿度数据

#endif
