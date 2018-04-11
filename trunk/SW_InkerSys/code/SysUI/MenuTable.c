
#include "InkerSys.h"
#include "MenuTable.h"

						  	/* 	  中文				   英文	*/
_ROM StrType StrProductId  =	{"编号:",              "PDID:"};
_ROM StrType StrDdg        =	{"Dbg:",               "Dbg:"};
_ROM StrType StrLanguage   =	{"Lang:",              "语言:"};
_ROM StrType StrVersion    =	{"V:",                 "V:"};
_ROM StrType StrBatterFix  =	{"修正:",		       "FixB:"};
_ROM StrType StrDate	   =	{"日期:",              "Date:"};
_ROM StrType StrTime	   =	{"时间:",              "Time:"};
_ROM StrType StrsBrightness=    {"亮度:",	           "Ligh:"};

_ROM StrType StrLangCh     =	{"Ch",                 "中文"};
_ROM StrType StrLangEn     =	{"En",                 "英文"};
_ROM StrType StrCompany    =	{"智游电子有限公司",   " Zhi Yong .co"};
_ROM StrType StrAbout      =	{"关于",               "About"};
_ROM StrType StrProductName=	{"智能管家婆",	       "Remote System"};
_ROM StrType StrUpData     =    {"升级",	           "UpData"};
_ROM StrType StrBack	   =	{"返回",               "Back"};
_ROM StrType StrSetup	   =	{"设置",               "Setup"};
_ROM StrType StrTest	   =	{"测试",               "Test"};
_ROM StrType StrOpen	   =	{"开",                 "ON"};
_ROM StrType StrClose	   =	{"关",                 "OFF"};
_ROM StrType StrLowEn	   =	{"低有效",             "Low"};
_ROM StrType StrHighEn	   =	{"高有效",             "High"};

_ROM MenuItem SysLogoItem[]= 
{
	{NULL,	            pFUNC_TYPE,	0,	{   0,  0,  16},	0,                  (MenuProc)ProLogoPage},
};

_ROM MenuItem SysVersItem[] =
{
	{NULL,		        pFUNC_TYPE,	0,	{   0,  0,  16},	VERS_BACK_POSI,     (MenuProc)ProVersPage},
};

_ROM MenuItem SysCodeItem[] = 
{
	{&StrProductId,		pFUNC_TYPE,	0,	{   0,  0,  16},	0, 					(MenuProc)ProcProductIdSet},
	{&StrDdg,		    pFUNC_TYPE,	0,	{  80,  0,  16},	0, 					(MenuProc)ProcDbgSet},	
	{&StrLanguage,		pFUNC_TYPE,	0,	{   0, 16,  16},	0, 					(MenuProc)ProcLanguage},
	{&StrsBrightness,	pFUNC_TYPE,	0,	{  80, 16,  16},	0, 					(MenuProc)ProcBrightness},
	{&StrBatterFix,		pFUNC_TYPE,	0,	{   0, 32,  16},	0, 					(MenuProc)ProcBatterFix},
	{&StrAbout,			pGOTO_TYPE,	0,	{  80, 32,  16},	VERS_PAGE, 	        (MenuProc)ProcGotoPage},
	{&StrUpData,		pFUNC_TYPE,	0,	{   0, 48,  16},	0, 					(MenuProc)ProcUpData},
	{&StrBack,			pFUNC_TYPE,	0,	{  80, 48,  16},	0, 					(MenuProc)ProcBackPage},
};

_ROM MenuItem SysMainItem[] = 
{
	{&StrSetup,	    	pFUNC_TYPE,	0,	{   0,  0,  16},	SETUP_PAGE,         (MenuProc)ProMainPage},
};

_ROM MenuItem SysSetupItem[] = 
{
	{&StrBack,			pFUNC_TYPE,	0,	{   0, 48,  16},	SETUP_BACK_POSI, 	(MenuProc)ProcBackPage},
};

_ROM MenuPage Menu1PageList[] = 
{
	{LOGO_PAGE,	        ARRAY_SIZE(SysLogoItem), 	   SysLogoItem,	    MAIN_PAGE },/*开机界面*/
	{CODE_PAGE,	        ARRAY_SIZE(SysCodeItem), 	   SysCodeItem,	    MAIN_PAGE },/*调试界面*/
	{VERS_PAGE,	        ARRAY_SIZE(SysVersItem),       SysVersItem,	    CODE_PAGE },/*关于    */
	{MAIN_PAGE,	        ARRAY_SIZE(SysMainItem), 	   SysMainItem,	    SETUP_PAGE},/*主界面  */
	{SETUP_PAGE,	    ARRAY_SIZE(SysSetupItem), 	   SysSetupItem,	MAIN_PAGE },/*设置界面*/
	
	{NULL_PAGE},	/*0*/
};

