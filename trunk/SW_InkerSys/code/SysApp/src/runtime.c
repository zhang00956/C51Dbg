
#define SYSLOG 0

#include "inkersys.h"

#define RUN_TIME_PAGE_NUM   1
#define RUN_TIME_PAGE_SIZE  0X200
#define RUN_TIME_START_ADDR 0X200

u32 _XDATA RunTimeCnt60S = 0;
u16 _XDATA RunTimeWordAddr;
u8  _IRAM  MaskRunTime   = 0;



static void EarseRunTimeData(void)
{
	EraseSector(RUN_TIME_START_ADDR);
	RunTimeWordAddr = 0;
}

GLOBAL void RunTimeInit(void)
{
	u32 temp32,data32;
	u16 i;

	ReadU32Value(RUN_TIME_START_ADDR,&temp32);

	if(temp32 == 0XFFFFFFFF)
	//if(1)
		{
		EarseRunTimeData();
		RunTimeCnt60S   = 0;
		}
	else
		{
		for(i=0;i<RUN_TIME_PAGE_SIZE/4;i++)
			{
			ReadU32Value(RUN_TIME_START_ADDR + i*4,&temp32);
			if(temp32 == 0XFFFFFFFF)
				{
				RunTimeWordAddr = i*4;
				break;
				}
			data32 = temp32;
			}
		RunTimeCnt60S = data32;
		}
	
	//DPRINTF(("WordAddr:%00004X Time:%000000008lX\r\n",RunTimeWordAddr,RunTimeCnt60S));
}

GLOBAL void ProF_10mS_RunTimeCnt(void)
{
	static u8 _IRAM cnt1S=0,cnt60S=30;

	cnt1S++;
	if(cnt1S >= 100)
		{
		cnt1S = 0;
		cnt60S++;
		if(cnt60S >= 60)
			{
			cnt60S = 0;
			MaskRunTime = 1;
			}
		}
}

GLOBAL u8 ProB_1S_RunTimeMask(void)
{
	u8 result = FALSE;
	
	if(MaskRunTime)
		{
		MaskRunTime = 0;
		RunTimeCnt60S++;
		result = TRUE;
		if(RunTimeWordAddr >= RUN_TIME_PAGE_SIZE)
			{
			EraseSector(RUN_TIME_START_ADDR);
			RunTimeWordAddr = 0;
			SaveU32Value(RUN_TIME_START_ADDR,RunTimeCnt60S);
			}
		else
			{
			SaveU32Value(RUN_TIME_START_ADDR + RunTimeWordAddr,RunTimeCnt60S);
			}
		RunTimeWordAddr += 4;

		//DPRINTF(("WordAddr:%00004X Time:%000000008lX\r\n",RunTimeWordAddr,RunTimeCnt60S));
		}
		
	return result;
}

/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool RunTime_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".RunTime\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".runtime\r\n") == NULL)
		{
		DPRINTF(("RunTimeMsg\r\n"));
		DPRINTF(("RunTime <u32>\r\n"));
		return TRUE;
		}
		
	if(strcmp(pComDbg.Revdata,"runtimemsg\r\n") == NULL)
		{
		DPRINTF(("RunTime:%lu\r\n",RunTimeCnt60S));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"runtime") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU32(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Paramu32) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		EarseRunTimeData();
		RunTimeCnt60S = pComDbg.Paramu32[0];
		MaskRunTime = 1;

		DPRINTF(("RunTime <= %lu\r\n",RunTimeCnt60S));
		//DPRINTF(("%lu分钟\r\n",RunTimeCnt60S));
		//DPRINTF(("%lu天%lud时%lu分钟\r\n",RunTimeCnt60S/(24*60),RunTimeCnt60S%(24*60)/60,RunTimeCnt60S%60));
		return TRUE;
		}

	return FALSE;
}

#endif



