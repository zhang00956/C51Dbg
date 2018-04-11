#ifndef _MENU2_H
#define _MENU2_H

typedef u8 SETUP_T;

extern u8 _XDATA EepromUpdate;
extern u8 _XDATA AboveNumStore;
extern u8 _XDATA BitCurPosi;
extern MenuProcParam _XDATA ParamMenu2;
extern pMenuItem pItemMenu2;

SETUP_T ProcGotoPage(void);
SETUP_T ProcBackPage(void);
SETUP_T ProLogoPage(void);
SETUP_T ProVersPage(void);
SETUP_T ProMainPage(void);

SETUP_T ProcBatterFix(void);
SETUP_T ProcLanguage(void);
SETUP_T ProcBrightness(void);
SETUP_T ProcProductIdSet(void);
SETUP_T ProcDbgSet(void);
SETUP_T ProcUpData(void);

#endif	/*_MENU2_H*/

