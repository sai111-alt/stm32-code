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

#define THigh ArrayValue[0]
#define TLow ArrayValue[1]
#define HHigh ArrayValue[2]
#define HLow ArrayValue[3]
#define Tem TemHemValue[2]
#define Hem TemHemValue[0]

void HardWare_Init(void);
void ValueJudgeShow(int8_t *TemHemValue, int8_t *ArrayValue, uint8_t *KeyNum);
void ValueSet(int8_t *ArrayValue, uint8_t *KeyNum, uint8_t *SetPlace);
void DataStorage(int8_t *TemHemValue);

#endif
