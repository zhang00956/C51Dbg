
#define SYSLOG 0

#include "inkersys.h"

#define SHAKEDOWM_TIME_S  10

tLicenseParam _XDATA LicenseParam;

static bool LicenseDecode(void)
{
	u8 sum = 0,i;
	for(i=0;i<7;i++)
		{
		sum += LicenseParam.StcId[i];
		}
	
	for(i=0;i<LICENSE_NUM;i++)
		{
		if((u8)(sum + i) != LicenseParam.License[i])
			{
			return 0;
			}
		}
	return 1;
}

GLOBAL void LicenseInit(void)
{
	LicenseParam.LicensenCntS = SHAKEDOWM_TIME_S;
	StcCarId(LicenseParam.StcId,7);
	LicenseParam.LicenseEn = LicenseDecode();
}

GLOBAL void ProB_1S_License(void)
{
	if(LicenseParam.LicenseEn)
		{
		LicenseParam.LicensenCntS = SHAKEDOWM_TIME_S;
		}
	else
		{
		LicenseParam.LicensenCntS--;
		}
	
#ifndef KEIL_DEBUG
	if(LicenseParam.LicensenCntS == 0)
		{
		DPRINTF(("License Reset!\r\n"));
		while(1);
		}
#endif	
}


/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/

#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool License_Comdbg(void)
{
	u8 i;
	
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".License\r\n"));
		return FALSE;
		}
		
	if(strcmp(pComDbg.Revdata,".license\r\n") == NULL)
		{
		DPRINTF(("LicMsg;\r\n"));
		DPRINTF(("License <u8>;\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"licmsg\r\n") == NULL)
		{
		DPRINTF(("StcID:0X%b002X%b002X%b002X%b002X%b002X%b002X%b002X\r\n",
		LicenseParam.StcId[0],LicenseParam.StcId[1],LicenseParam.StcId[2],LicenseParam.StcId[3],
		LicenseParam.StcId[4],LicenseParam.StcId[5],LicenseParam.StcId[6]));

		pComDbg.Param8[0] = 0;
		for(i=0;i<LICENSE_NUM - 1;i++)
			{
			pComDbg.Param8[0] += LicenseParam.StcId[i];	
			}
		DPRINTF(("->License Code:0X%b002X\r\n",pComDbg.Param8[0]));
		
		DPRINTF(("认证状态:"));
		if(LicenseParam.LicenseEn)
			{
			DPRINTF(("已认证\r\n"));
			}
		else
			{
			DPRINTF(("未认证\r\n"));
			DPRINTF(("有效运行:%dS\r\n",(u16)SHAKEDOWM_TIME_S));
			}
		return TRUE;
		}
	
	if(strcmp(pComDbg.Revdata,"license") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		for(i=0;i<LICENSE_NUM;i++)
			{
			LicenseParam.License[i] = pComDbg.Param8[0]+i;	
			}
		LicenseParam.LicenseEn = LicenseDecode();
		if(LicenseParam.LicenseEn)
			{
			DPRINTF(("License Success!\r\n"));
			SysParam.EepromFlag = 1;
			}
		else
			{
			DPRINTF(("License Failure!\r\n"));
			DPRINTF(("Mail:dongmaowan99@163.com\r\n"));
			}
		return  TRUE;
		}
		
	return FALSE;
}

#endif

/* #End Comdbg Interface */



