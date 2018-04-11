#ifndef _MBSRTU_H
#define _MBSRTU_H

#include "MbsReg.h"
#include "MbsWp.h"

GLOBAL void MbsLoadDefault(void);
GLOBAL void MbsReadEeprom(void);
GLOBAL void MbsSaveEeprom(void);
GLOBAL void MbsMsg(void);
GLOBAL void SetMbsID(u8 val);

GLOBAL void MbsInit(void);
GLOBAL void MbsRxd(u8 rxddata);
GLOBAL void ProF_10mS_MbsTimeOut(void);
GLOBAL void ProB_RealTime_MbsRtu(void);


#endif

