#ifndef __MYSPI_H__
#define __MYSPI_H__

#include "stm32f10x.h"

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif
