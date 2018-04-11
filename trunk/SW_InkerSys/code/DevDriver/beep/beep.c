
#define SYSLOG 0

#include "inkersys.h"

volatile u8  _IRAM beeper_on_timer = 0,beeper_off_timer = 0,BeepNum = 0;
volatile u16 _IRAM BeepCnt = 0;

GLOBAL void SetBeep(u8 num,u8 time_on,u8 time_off)
{
	BeepNum = num;
	beeper_on_timer  = time_on;
	beeper_off_timer = time_off;
	SetBeeper(0);
	BeepCnt = 0;
}

GLOBAL void ProF_10mS_Beeper(void)
{
	if(BeepCnt)
		{
		BeepCnt--;
		if(BeepCnt == beeper_off_timer)
			SetBeeper(0);
		}
	else
		{
		if(BeepNum == 0XFF)
			{
			SetBeeper(1);
			BeepCnt = beeper_on_timer + beeper_off_timer;
			}
		else if(BeepNum)
			{
			SetBeeper(1);
			BeepNum --;
			BeepCnt = beeper_on_timer + beeper_off_timer;
			}
		}
}

/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool Beep_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".Beep\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".beep\r\n") == NULL)
		{
		DPRINTF(("Beep <num8> <on8> <off8>;\r\n"));
		DPRINTF((">Beep 1 0 1\r\n"));
		DPRINTF((">Beep 1 30 20\r\n"));
		DPRINTF((">Beep hff 30 20\r\n"));
		return TRUE;
		}
		
	if(strcmp(pComDbg.Revdata,"beep") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(TypeU8(pComDbg.Revdata+pComDbg.Cnt,pComDbg.Param8+1) == FALSE)
			return FALSE;
		if(TypeU8(pComDbg.Revdata+pComDbg.Cnt,pComDbg.Param8+2) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;
		
		SetBeep(pComDbg.Param8[0],pComDbg.Param8[1],pComDbg.Param8[2]);
		return TRUE;
		}

	return FALSE;
}

#endif

