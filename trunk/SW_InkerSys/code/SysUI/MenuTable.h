#ifndef _MENUTABLE_H
#define _MENUTABLE_H

#include "lcd.h"
#include "key.h"
#include "Menu1.h"
#include "menu2.h"

#define SETUP_BACK_POSI	0
#define VERS_BACK_POSI  5


extern _ROM StrType StrOpen;
extern _ROM StrType StrClose;
extern _ROM StrType StrLowEn;
extern _ROM StrType StrHighEn;
extern _ROM StrType StrLangCh;
extern _ROM StrType StrLangEn;
extern _ROM StrType StrRunState;
extern _ROM StrType StrIdleState;
extern _ROM StrType StrWarnState;
extern _ROM StrType StrProductName;
extern _ROM StrType StrCompany;



extern _ROM MenuPage Menu1PageList[];

#endif	/*_MENUTABLE_H*/

