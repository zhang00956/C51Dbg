#ifndef _LICENSE_H
#define _LICENSE_H

#define LICENSE_NUM 8

typedef struct{
	u8	  StcId[7];
	u8	  License[LICENSE_NUM];
	u8	  LicenseEn;
	u16   LicensenCntS;
}tLicenseParam;

extern tLicenseParam _XDATA LicenseParam;

GLOBAL void LicenseInit(void);
GLOBAL void ProB_1S_License(void);


#endif

