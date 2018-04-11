#ifndef _PROJECT_H
#define _PROJECT_H

enum{
PRJ_IDLE,
};

typedef struct{
vu8 PrjState;
 u8 SubState;
}tPrjPkg;


extern tPrjPkg _XDATA PrjPkg;

GLOBAL void PrjLoadDefault(void);
GLOBAL void PrjSaveEeprom(void);
GLOBAL void PrjReadEeprom(void);
GLOBAL void PrjInit(void);
GLOBAL void ProB_RealTime_PrjKey(void);

#endif

