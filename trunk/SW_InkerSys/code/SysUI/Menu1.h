#ifndef _MENU1_H
#define _MENU1_H

/*菜单页编号*/
typedef enum{
	NULL_PAGE,
	LOGO_PAGE,            /* 1 */
	CODE_PAGE,            /* 2 */
	VERS_PAGE,            /* 3 */
	MAIN_PAGE,            /* 4 */
	SETUP_PAGE,           /* 5 */

	END_PAGE,
}PageId;

typedef enum
{
	_INIT_MODE,
	_KEY_MODE,
	_DISP_MODE,
	_GOTO_MODE,
	_BACK_MODE,
	_LOAD_DEFAULT_MODE,
	_LOAD_EEPROM_MODE,
	_SET_MODE,	        /*直接设置某个值,PC通讯控制时使用*/
	_CHECK_SKIP_MODE,   /*bit2:SKIP display,bit1:SKIP cursor*/
}ParaMode;

/*菜单处理函数的指针*/
typedef void (*MenuProc)(void);

typedef struct{
	u8 MinY;
	u8 MaxY;
}ScreenPosi, *pScreenPosi;

/*菜单处理函数的参数*/
typedef struct{
	u8 mode;
	u8 para1;
	u8 para2;
	PageId mId;
	ItemPosi Posi;
}MenuProcParam, * pMenuProcParam;

/*菜单项类型*/
typedef enum{
	pFUNC_TYPE,	/* pointer to a function */
	pGOTO_TYPE,	/*pointer to a Menu Page*/
}ItemType;

typedef struct{
	const char * StrCh;
	const char * StrEn;
}StrType;

typedef struct{
	u8 value;
	const StrType * Str;
}OptionType;

typedef struct{
	u8 value;
	const char * Str;
}OptionTypeA;

/*菜单项结构*/
typedef struct{
	StrType *pCap;
	ItemType uType;// 未使用
	u8 uOptionNum; // 同种属性的选项区分 比如多档修正
	ItemPosi Posi;
	u8 uPara;
	MenuProc pFunc;
}MenuItem, *pMenuItem;

/*菜单页结构*/
typedef struct{
	PageId mId;
	u8 uItemNum;
	pMenuItem pItemArray;
	PageId ParId;	/*Parent Page*/
	ScreenPosi SPosi;
}MenuPage, *pMenuPage;

typedef struct{
	u8 MenuPrf;
	u8 EepromSave;
	u8 PrfToCom;
	u8 CntS;
}tMenuSys;

extern tMenuSys _XDATA MenuSys;

extern PageId Menu1GetCurPage(void);
//extern u8     Menu1GetCurItem(void);
extern void   Menu1Open(PageId InitPage,u8 ItemPosi,u8 display);

GLOBAL void MenuInit(void);
GLOBAL void ProB_100mS_Menu(void);

#endif	/*_MENU1_H*/

