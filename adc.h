#ifndef _ADC_H
#define _ADC_H
#include "stc12c5a60s2.h"
#include "intrins.h"

#define ADC_POWER 0x80
#define ADC_FLAG 0x10
#define ADC_START 0x08
#define ADC_SPEEDLL 0x00
#define ADC_SPEEDL 0x20
#define ADC_SPEEDH 0x40
#define ADC_SPEEDHH 0x60


void Init_ADC();
unsigned char GetADCResult(unsigned char ch);
#endif
