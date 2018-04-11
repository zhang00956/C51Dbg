#ifndef _COMDBG_H
#define _COMDBG_H

#ifdef _GLOBAL_COMDBG_ENABLE

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define COMDBGBUFNUM   32
#define STRINGTYPESIZE 16

typedef struct{
	u8    DbgEn;
	u8    ComDbgCnt;
	u8    Revdata[COMDBGBUFNUM];// \n \0
	u8    Cnt;
	
	u8    Str[STRINGTYPESIZE];
	bool  ParamB[3];
	u8    Array[8];
	u8    Param8[3];
	u16   Param16[3];
	u32   Paramu32[1];
	float ParamF[1];
	
}tComDbg;

extern tComDbg _XDATA pComDbg;

GLOBAL u8   SearchChar(u8 *str,u8 asiic);
GLOBAL bool EndIsEnter(u8 *str);
GLOBAL bool TypeBool(u8 *str,bool *val);
GLOBAL bool TypeString(u8 *str,u8 Array[]);
GLOBAL bool TypeU8(u8 *str,u8 *val);
GLOBAL bool TypeU16(u8 *str,u16 *val);
GLOBAL bool TypeU32(u8 *str,u32 *val);
GLOBAL bool TypeFloat(u8 *str,float *val);

GLOBAL void ComDbgInit(void);
GLOBAL void ProB_RealTime_ComDbg(void);

/* Comdbg Task List */
GLOBAL bool Dbg_Comdbg(void);
GLOBAL bool System_Comdbg(void);
GLOBAL bool License_Comdbg(void);
GLOBAL bool RunTime_Comdbg(void);
GLOBAL bool Format_Comdbg(void);
GLOBAL bool McuBsp_Comdbg(void);
GLOBAL bool Prj_Comdbg(void);
GLOBAL bool Lamp_Comdbg(void);
GLOBAL bool Beep_Comdbg(void);
GLOBAL bool Key_Comdbg(void);
GLOBAL bool MbsRtu_Comdbg(void);
GLOBAL bool MbsReg_Comdbg(void);
GLOBAL bool Menu_Comdbg(void);
GLOBAL bool InPut_Comdbg(void);
GLOBAL bool InAdc_Comdbg(void);
GLOBAL bool LedSeg_Comdbg(void);

#else

#define ComDbgInit()
#define ProB_RealTime_ComDbg()

#endif

#endif

