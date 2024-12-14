#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"
#include "Key.h"
#include "LED.h"
#include "DHT22.h"
#include "Buzzer.h"
#include "LED.h"

void HardWare_Init(void);
void ValueJudgeShow(int8_t *TemHemValue, int8_t *ArrayValue, uint8_t *KeyNum);
void ValueSet(int8_t *ArrayValue, uint8_t *KeyNum, uint8_t *SetPlace);
void DataStorage(int8_t *TemHemValue);

#endif
