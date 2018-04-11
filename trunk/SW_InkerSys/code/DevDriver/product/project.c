
#define SYSLOG 0

#include "inkersys.h"
#include "key.h"
#include "lamp.h"
#include "beep.h"
#include "input.h"
#include "key.h"

tPrjPkg _XDATA PrjPkg;

/************************************************************************/
/* #If Project Eeprom Store                                             */
/************************************************************************/
typedef struct{
u8	  PrjStatePosEE;
}tPRJ_EEPOM;
#define PrjEeprom(m)	(size_t)&(((tPRJ_EEPOM _ROM *)PRJ_EEPORM)->m)

GLOBAL void PrjLoadDefault(void)
{
	PrjPkg.PrjState  = PRJ_IDLE;
}

GLOBAL void PrjSaveEeprom(void)
{
	SaveU08Value(PrjEeprom(PrjStatePosEE),  PrjPkg.PrjState);
}

GLOBAL void PrjReadEeprom(void)
{
	ReadU08Value(PrjEeprom(PrjStatePosEE), &PrjPkg.PrjState);
}
/* #End Project Eeprom Store */


/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool Prj_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".Prj\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".prj\r\n") == NULL)
		{
		DPRINTF(("PrjMsg\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"prjmsg\r\n") == NULL)
		{
		DPRINTF(("PrjState:%bd SubState:%bd\r\n",PrjPkg.PrjState,PrjPkg.SubState));
		return TRUE;
		}
	
	return FALSE;
}
/* #End Comdbg Interface */

#endif

/************************************************************************/
/*      Application Interface                                           */
/************************************************************************/
GLOBAL void PrjInit(void)
{
	PrjPkg.PrjState  = PRJ_IDLE;
	PrjPkg.SubState  = 0;

	SetLamp(10,2,2);
	SetBeep(1,30,1);
	key_enable = 0;
}

GLOBAL void ProB_RealTime_PrjKey(void)
{
	if(GetInput(0))
		{
		
		}
#if 0
	u8 key = 0;

	if(key_enable)
		{
		key_enable = 0;
		key = _key;
		SetLamp(1,5,0);
		SetBeep(1,5,0);
		if(SysParam.SysPrf)
			DPRINTF(("KeyVal:%bd\r\n",key));
		}
#endif
}

