#ifndef __Buzzer_H__
#define __Buzzer_H__

#include "stm32f10x.h"

#define IO GPIO_Pin_12

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
void Buzzer_Turn(void);

#endif
