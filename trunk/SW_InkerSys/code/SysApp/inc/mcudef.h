#ifndef _MCUDEF_H 
#define _MCUDEF_H

#include "stc12c5a60s2.h"
#include "eeprom_soc.h"
#include "watchdog_soc.h"
#include "adc_soc.h"

/* P0 */
sbit PIN_INPUT1       = P0^0;
sbit PIN_INPUT2       = P0^1;
sbit PIN_INPUT3       = P0^2;
sbit PIN_INPUT4       = P0^3;
sbit PIN_INPUT5       = P0^4;
sbit PIN_INPUT6       = P0^5;
sbit PIN_INPUT7       = P0^6;
sbit PIN_INPUT8       = P0^7;


/* P1 */
sbit PIN_LED          = P1^0;//
sbit PIN_P11          = P1^1;//
//                      P1^2;//SIM_RXD
//                      P1^3;//SIM_TXD
sbit PIN_TIME_OUT     = P1^4;//时间开关输出控制 sbit PIN_RS485_EN     = P1^4;
sbit PIN_DS1302_SCL   = P1^5;//时钟芯片时钟管脚 sbit PIN_TXD_433MHZ   = P1^5;
sbit PIN_DS1302_SDA   = P1^6;//时钟芯片数据管脚 sbit PIN_TXD_315MHZ   = P1^6;
sbit PIN_DS1302_CS    = P1^7;//时钟芯片片选管脚 sbit PIN_RXD_433MHZ   = P1^7;


/* P2 */
sbit PIN_KEY3         = P2^5;//
sbit PIN_KEY4         = P2^6;//


/* P3 */
//                      P3^0;//PC RXD
//                      P3^1;//PC TXD



/* P4 */
sbit PIN_RELAY_BEEP   = P4^0;
sbit PIN_BEEPER       = P4^1;
sbit PIN_RELAY_LED    = P4^2;//布防 撤防灯屏蔽 目前当拍照辅助灯控制
sbit PIN_RXD_315MHZ   = P4^3;
sbit PIN_TXD_315MHZ   = P4^4;//sbit PIN_KEY3         = P4^4;//单火线
sbit PIN_KEY1         = P4^5;//按键
sbit PIN_KEY2         = P4^6;//防区
sbit PIN_LIGHT_IN     = P4^7;//光敏电阻 1:白天 0:晚上 //sbit PIN_LED          = P4^7;


#endif

