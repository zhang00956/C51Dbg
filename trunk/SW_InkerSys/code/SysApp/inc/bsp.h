#ifndef _BSP_H
#define _BSP_H

#include "mcudef.h"

#define InterruptEn(m)  m?(EA=1):(EA=0)
#define SetLed(m)       m?(PIN_LED=1):(PIN_LED=0)
#define SetBeeper(m)    m?(PIN_BEEPER=1):(PIN_BEEPER=0)
#define GetKey1()       PIN_KEY1
#define GetKey2()       PIN_KEY2
#define GetKey3()       PIN_KEY3
#define GetKey4()       PIN_KEY4

extern vu8 TxdFlag,RxdFlag,RxdData;

GLOBAL void Stc125a60s2Init(void);
GLOBAL void StcIap(void);
GLOBAL void StcCarId(u8 *p,u8 size);
GLOBAL void PowerDownMode(void);
GLOBAL void Uart0Init(u32 Baudrate);
GLOBAL void Uart0Send(u8 send_data);
GLOBAL void ConfigPort(u8 port,u8 pxm1,u8 pxm0);
GLOBAL void SetPort(u8 port,u8 bits,bool state);
GLOBAL   u8 GetPort(u8 port);
GLOBAL   u8 GetInPut1(void);
GLOBAL   u8 GetInPut2(void);
GLOBAL void ProB_RealTime_StcMcu(void);

#endif

