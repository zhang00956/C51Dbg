#ifndef _SYSTEM_H
#define _SYSTEM_H

typedef struct{
	u8    Dbg;            /*调试系统    */
	u8    UpPowerDbg;     /*上电调试状态*/
	u16   ProductId;      /*设备编号    */
	u32   EraseEepromNum; /*Flash 擦除累计*/
	float SysBeatsFix;    /*系统节拍修正*/
	u8    Language;       /*系统语言 0:English 1:中文*/
	u8    SysPrf;         /*打印级别*/

	float BatterFix;
	u8    Brightness;
	
	u8    EepromFlag;
	u8    SysSeconds;
	u8    SysMinutes;
	u8    SysHours;
}tSystParam;

extern tSystParam _XDATA SysParam;

GLOBAL void SysLoadDefault(void);
GLOBAL void SystemParamInit(void);

#endif

