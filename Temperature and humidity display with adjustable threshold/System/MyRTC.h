#ifndef __MYRTC_H
#define __MYRTC_H

#include "stm32f10x.h" 
#include <time.h>

#define Year MyRTC_Time[0]
#define Month MyRTC_Time[1]
#define Day MyRTC_Time[2]
#define Hour MyRTC_Time[3]
#define Minute MyRTC_Time[4]
#define Second MyRTC_Time[5]

extern uint16_t MyRTC_Time[];

void MyRTC_Init(void);
void MyRTC_SetTime(void);
void MyRTC_ReadTime(void);

#endif
