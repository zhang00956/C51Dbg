#ifndef _SYSTEM_H
#define _SYSTEM_H

typedef struct{
	u8    Dbg;            /*����ϵͳ    */
	u8    UpPowerDbg;     /*�ϵ����״̬*/
	u16   ProductId;      /*�豸���    */
	u32   EraseEepromNum; /*Flash �����ۼ�*/
	float SysBeatsFix;    /*ϵͳ��������*/
	u8    Language;       /*ϵͳ���� 0:English 1:����*/
	u8    SysPrf;         /*��ӡ����*/

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

