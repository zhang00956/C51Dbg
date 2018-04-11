
#define SYSLOG 0

#include "inkersys.h"

#include <intrins.h>


/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks


#define EN_ADC_CH 0X03

GLOBAL void InitAdc(void)
{
	P1M1    |=   EN_ADC_CH;
	P1M0    &= (~EN_ADC_CH);
	
	P1ASF     = EN_ADC_CH;
	ADC_RES   = 0;
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
}

GLOBAL u8 ReadAdcCh(u8 ch)// ch<0..7
{
	u8 tmp8;
	
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

	tmp8 = ADC_RES;
	return tmp8;
}

