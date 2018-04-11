
#define SYSLOG 0

#include "inkersys.h"
#include "mbsrtu.h"
#include "MenuTable.h"
#include "InAdc.h"
#include "LedSeg.h"


tSystParam _XDATA SysParam;

/************************************************************************/
/* #If System Eeprom Store                                              */
/************************************************************************/
typedef struct{
u16   BootCodePosEE;			  /*初始化引导	  */
u8	  LicensePosEE[LICENSE_NUM];  /*许可证		  */
u32   EraseEepromNumPosEE;		  /*Flash 擦除累计*/
u8	  LanguagePosEE;			  /*系统语言	  */
u8	  UpPowerDbgPosEE; 		      /*调试系统	  */
float SysBeatsFixPosEE; 		  /*系统节拍修正  */
u16   ProductIdPosEE;			  /*设备编号	  */
u8	  SysPrfPosEE;

}tSYS_EEPOM;
#define SysEeprom(m)	(size_t)&(((tSYS_EEPOM _ROM *)SYS_EEPROM)->m)

GLOBAL void SysLoadDefault(void)
{
	SysParam.UpPowerDbg     = 0;
	SysParam.EraseEepromNum = 0;
	SysParam.ProductId      = 0;
	SysParam.SysBeatsFix    = 1.0;
	SysParam.Language       = 0;
	SysParam.SysPrf         = 0;

	PrjLoadDefault();
	MbsLoadDefault();
	RegLoadDefault();
}

static void SysSaveEeprom(void)
{	
	u8 i;
	
	EraseBlock(SYS_EEPROM_PAGE);
	SysParam.EraseEepromNum ++;
	SaveU16Value(SysEeprom(BootCodePosEE),      EEPROMBOOTCODE);
	for(i=0;i<LICENSE_NUM;i++)
		SaveU08Value(SysEeprom(LicensePosEE[i]), LicenseParam.License[i]);

	SaveU08Value(SysEeprom(UpPowerDbgPosEE),    SysParam.UpPowerDbg);
	SaveU16Value(SysEeprom(ProductIdPosEE),     SysParam.ProductId);
	SaveU32Value(SysEeprom(EraseEepromNumPosEE),SysParam.EraseEepromNum);
	SaveF32Value(SysEeprom(SysBeatsFixPosEE),   SysParam.SysBeatsFix);
	SaveU08Value(SysEeprom(LanguagePosEE),      SysParam.Language);
	SaveU08Value(SysEeprom(SysPrfPosEE),        SysParam.SysPrf);
	
	PrjSaveEeprom();
	MbsSaveEeprom();
	RegSaveEeprom();
	
	if(SysParam.SysPrf)
		DPRINTF(("Save Data EEprom!\r\n"));
}

static void SysReadEeprom(void)
{	
	u8 i;
	
	for(i=0;i<LICENSE_NUM;i++)
		ReadU08Value(SysEeprom(LicensePosEE[i]),LicenseParam.License+i);

	ReadU08Value(SysEeprom(UpPowerDbgPosEE),    &SysParam.UpPowerDbg);
	ReadU16Value(SysEeprom(ProductIdPosEE),     &SysParam.ProductId);
	ReadU32Value(SysEeprom(EraseEepromNumPosEE),&SysParam.EraseEepromNum);
	ReadF32Value(SysEeprom(SysBeatsFixPosEE),   &SysParam.SysBeatsFix);
	ReadU08Value(SysEeprom(LanguagePosEE),      &SysParam.Language);
	ReadU08Value(SysEeprom(SysPrfPosEE),        &SysParam.SysPrf);
	
	PrjReadEeprom();
	MbsReadEeprom();
	RegReadEeprom();
}
/* #End System Eeprom Store */

static void SystemParamInit(void)
{
	u16 i;
	
	SysParam.SysMinutes  = 0;
	SysParam.SysHours    = 0;
	SysParam.EepromFlag  = 0;
	ReadU16Value(SysEeprom(BootCodePosEE),&i);
	if(i != EEPROMBOOTCODE)
		{
		SysLoadDefault();
		SysSaveEeprom();
		if(SysParam.SysPrf)
			DPRINTF(("LoadDefault!\r\n"));
		}
	else
		{
		SysReadEeprom();
		}

	SysParam.Dbg = SysParam.UpPowerDbg;

#ifdef KEIL_DEBUG
	SysParam.Dbg = SysParam.UpPowerDbg = 1;
#endif

	LicenseInit();
	RunTimeInit();
	ComDbgInit();
}

static void ProB_1S_Admin(void)
{
	SysParam.SysSeconds ++;
	if(SysParam.SysSeconds > 59)
		{
		SysParam.SysSeconds = 0;
		SysParam.SysMinutes++;
		}
	
	if(SysParam.SysMinutes > 59)
		{
		SysParam.SysMinutes = 0;
		SysParam.SysHours++;
		}
}

static void ProB_100mS_Admin(void)  //后台100mS进程
{
	static u8 Modulator_100mS = 0;
	
	Modulator_100mS++;
	switch(Modulator_100mS)
		{
		case 1:
			if(SysParam.Dbg == 0)
				ProB_1S_License();
			break;
		case 2:
			ProB_1S_RunTimeMask();
			break;
		case 3:
			if(SysParam.EepromFlag)
				{
				SysParam.EepromFlag = 0;
				SysSaveEeprom();
				}
			break;
			
		case 4:
			ProB_1S_GetAdc();
			break;
			
		case 10:
			Modulator_100mS = 0;
			ProB_1S_Admin();
			break;
		}
}

void main(void)
{
	Stc125a60s2Init();
	HearBeatInit();
	InterruptEn(1);
	VersionInit();
	SystemParamInit();

	MenuInit();
	MbsInit();
	LedSegInit();
	
	PrjInit();
	WatchDogEn();
	while(1)
		{
		ProB_RealTime_StcMcu();
		ProB_RealTime_WatchDogFeed_Soc();
		ProB_RealTime_ComDbg();
		ProB_RealTime_PrjKey();
		ProB_RealTime_MbsRtu();
		if(SysTimer10mS >= 10)
			{
			SysTimer10mS = SysTimer10mS - 10;
			ProB_100mS_Admin();
			ProB_100mS_Menu();
			}
		}
}


/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/

#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool System_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".System\r\n"));
		return FALSE;
		}
	
	if(strcmp(pComDbg.Revdata,".system\r\n") == NULL)
		{
		DPRINTF(("SysMsg\r\n"));
		DPRINTF(("Version\r\n"));
		DPRINTF(("SystemInit\r\n"));
		DPRINTF(("ProductID <Id16>\r\n"));
		DPRINTF(("BeatsFix  <float>\r\n"));
		DPRINTF(("SysPrf    <0~1>\r\n"));
		DPRINTF(("SetLang   <0~1> 0En 1Ch\r\n"));
		DPRINTF(("SetTime    <H> <M> <S>\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"sysmsg\r\n") == NULL)
		{
		DPRINTF(("UpPowerDbg :%bd\r\n",     SysParam.UpPowerDbg));
		DPRINTF(("EraseNumCnt:%lu\r\n",     SysParam.EraseEepromNum));
		DPRINTF(("ProductID  :%00004u\r\n", SysParam.ProductId));
		DPRINTF(("BeatsFix   :%f\r\n",      SysParam.SysBeatsFix));
		DPRINTF(("Language   :%s\r\n",      SysParam.Language?"中文":"English"));
		DPRINTF(("SysPrf     :%bd\r\n",     SysParam.SysPrf));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"version\r\n") == NULL)
		{
		Version();
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"systeminit\r\n") == NULL)
		{
		SysLoadDefault();
		SysParam.EepromFlag = 1;
		DPRINTF(("System Param Init To Default!\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"productid") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU16(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param16) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;
		
		if(pComDbg.Cnt <= 15)
			{
			SysParam.ProductId  = pComDbg.Param16[0];
			SysParam.EepromFlag = 1;
			DPRINTF(("Product <= %u\r\n",SysParam.ProductId));
			}
		else
			{
			DPRINTF(("Input ID Range:0000~9999\r\n"));
			}
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"beatsfix") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeFloat(pComDbg.Revdata + pComDbg.Cnt,&pComDbg.ParamF[0]) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		SysParam.SysBeatsFix = pComDbg.ParamF[0];
		DPRINTF(("SysBeatsFix <= %f\r\n",SysParam.SysBeatsFix));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"sysprf") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		SysParam.SysPrf = pComDbg.Param8[0];
		DPRINTF(("SysPrf <= %bd\r\n",SysParam.SysPrf));
		return TRUE;
		}

	
	if(strcmp(pComDbg.Revdata,"setlang") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		SysParam.Language = pComDbg.Param8[0];
		DPRINTF(("Language <= %bd\r\n",SysParam.Language));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"settime") == NULL)
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
	
		if(pComDbg.Param8[1] < 60 && pComDbg.Param8[2] < 59)
			{
			SysParam.SysHours	= pComDbg.Param8[0];
			SysParam.SysMinutes = pComDbg.Param8[1];
			SysParam.SysSeconds = pComDbg.Param8[2];
			DPRINTF(("时间:%b002u:%b002u:%b002u\r\n",SysParam.SysHours,SysParam.SysMinutes,SysParam.SysSeconds));
			return TRUE;
			}
		}
	
	return FALSE;
}

#endif

/* #End Comdbg Interface */

