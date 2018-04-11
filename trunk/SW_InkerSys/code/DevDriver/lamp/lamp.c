
#define SYSLOG 0

#include "inkersys.h"

vu8  _IRAM Lamp_on_timer = 0,Lamp_off_timer = 0,LampNum = 0;
vu16 _IRAM LampCnt = 0;

GLOBAL void SetLamp(u8 num,u8 time_on,u8 time_off)
{
	LampNum        = num;
	Lamp_on_timer  = time_on;
	Lamp_off_timer = time_off;
	SetLed(0);
}

GLOBAL void ProF_10mS_Lamper(void)
{
	if(LampCnt)
		{
		LampCnt--;
		if(LampCnt == Lamp_off_timer)			 
			SetLed(0);
		}
	else
		{
		if(LampNum == 0XFF)
			{
			LampCnt = Lamp_on_timer + Lamp_off_timer;
			SetLed(1);
			}
		else if(LampNum)
			{
			LampNum --;
			LampCnt = Lamp_on_timer + Lamp_off_timer;
			SetLed(1);
			}
		}
}

/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool Lamp_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".Lamp\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".lamp\r\n") == NULL)
		{
		DPRINTF(("Lamp <num8> <on8> <off8>;\r\n"));
		DPRINTF((">Lamp 1 0 1\r\n"));
		DPRINTF((">Lamp 1 30 20\r\n"));
		DPRINTF((">Lamp hff 30 20\r\n"));
		return TRUE;
		}
		
	if(strcmp(pComDbg.Revdata,"lamp") == NULL)
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
		
		SetLamp(pComDbg.Param8[0],pComDbg.Param8[1],pComDbg.Param8[2]);
		return TRUE;
		}

	return FALSE;
}

#endif


