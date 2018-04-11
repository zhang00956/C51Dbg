
#include "InkerSys.h"
#include "MenuTable.h"

						  	/* 	  ����				   Ӣ��	*/
_ROM StrType StrProductId  =	{"���:",              "PDID:"};
_ROM StrType StrDdg        =	{"Dbg:",               "Dbg:"};
_ROM StrType StrLanguage   =	{"Lang:",              "����:"};
_ROM StrType StrVersion    =	{"V:",                 "V:"};
_ROM StrType StrBatterFix  =	{"����:",		       "FixB:"};
_ROM StrType StrDate	   =	{"����:",              "Date:"};
_ROM StrType StrTime	   =	{"ʱ��:",              "Time:"};
_ROM StrType StrsBrightness=    {"����:",	           "Ligh:"};

_ROM StrType StrLangCh     =	{"Ch",                 "����"};
_ROM StrType StrLangEn     =	{"En",                 "Ӣ��"};
_ROM StrType StrCompany    =	{"���ε������޹�˾",   " Zhi Yong .co"};
_ROM StrType StrAbout      =	{"����",               "About"};
_ROM StrType StrProductName=	{"���ܹܼ���",	       "Remote System"};
_ROM StrType StrUpData     =    {"����",	           "UpData"};
_ROM StrType StrBack	   =	{"����",               "Back"};
_ROM StrType StrSetup	   =	{"����",               "Setup"};
_ROM StrType StrTest	   =	{"����",               "Test"};
_ROM StrType StrOpen	   =	{"��",                 "ON"};
_ROM StrType StrClose	   =	{"��",                 "OFF"};
_ROM StrType StrLowEn	   =	{"����Ч",             "Low"};
_ROM StrType StrHighEn	   =	{"����Ч",             "High"};

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
	{LOGO_PAGE,	        ARRAY_SIZE(SysLogoItem), 	   SysLogoItem,	    MAIN_PAGE },/*��������*/
	{CODE_PAGE,	        ARRAY_SIZE(SysCodeItem), 	   SysCodeItem,	    MAIN_PAGE },/*���Խ���*/
	{VERS_PAGE,	        ARRAY_SIZE(SysVersItem),       SysVersItem,	    CODE_PAGE },/*����    */
	{MAIN_PAGE,	        ARRAY_SIZE(SysMainItem), 	   SysMainItem,	    SETUP_PAGE},/*������  */
	{SETUP_PAGE,	    ARRAY_SIZE(SysSetupItem), 	   SysSetupItem,	MAIN_PAGE },/*���ý���*/
	
	{NULL_PAGE},	/*0*/
};

