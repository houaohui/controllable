#include "adc.h"
sfr PlASF = 0x9D;


void Init_ADC()
{
	PlASF = 0x0F;
	ADC_RES = 0;
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
}


unsigned char GetADCResult(unsigned char ch)
{
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | ch;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR&ADC_FLAG));
	ADC_CONTR &=~ADC_FLAG;
	return ADC_RES;
}

