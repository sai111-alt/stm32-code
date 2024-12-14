#ifndef __MYSPI_S_H__
#define __MYSPI_S_H__

#include "stm32f10x.h"

#define SS GPIO_Pin_12
#define SCK GPIO_Pin_9
#define MOSI GPIO_Pin_8
#define MISO GPIO_Pin_11

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif
