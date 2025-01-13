#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stm32f10x.h"
#include "Delay.h"

#define DATA GPIO_Pin_13

void DHT11_Mode(uint8_t mode);          // IO口模式切换
void DHT11_Init(void);                  // 初始化DHT11
uint8_t DHT11_Read_Data(int8_t *Array); // 读取温湿度数据

#endif
