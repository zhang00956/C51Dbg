#define SYSLOG 0

#include "InkerSys.h"
#include "key.h"
#include "lamp.h"
#include "beep.h"
#include "MenuTable.h"

/************************************************************************/
/*      Local Prototypes                                                */
/************************************************************************/
static u8 IsItemSkipCursor(MenuPage* pPage,MenuItem* pItem);
static u8 IsItemSkipDisp(MenuPage* pPage,const MenuItem* pItem);
static void Menu1Display(MenuPage* pCurPage,u8 mode);//mode 1:清屏更新 2:不清屏更新
static void sMenu1ProcKeyNext(void);
static void sMenu1ProcKeyPrev(void);


/************************************************************************/
/*      Static Variables                                                */
/************************************************************************/
static MenuPage*  _XDATA pMenu1CurPage=0;
static MenuItem*  _XDATA pMenu1CurItem=0;
static u8         _XDATA CurItemPos = 0;
static ScreenPosi _XDATA sPosi = {0,SCREEN_Y_MAX};
static PageId     _XDATA BakCurrentPage = NULL_PAGE;
static u8         _XDATA BakCurrentItem = 0;
/************************************************************************/
/*      Application Interface                                           */
/************************************************************************/
GLOBAL MenuPage* Menu1FindPage(PageId mId)
{
	pMenuPage _XDATA PageList = Menu1PageList;

	while(PageList->mId)
		{
		if(PageList->mId == mId)
			return PageList;
		PageList ++;
		}
	return NULL;
}

GLOBAL PageId Menu1GetCurPage(void)
{
	return BakCurrentPage;
}
#if 0
GLOBAL u8 Menu1GetCurItem(void)
{
	return BakCurrentItem;
}
#endif
GLOBAL void Menu1Open(PageId InitPage,u8 ItemPosi,u8 Display)//Display 0:不刷新 1:刷新
{
	u8 _XDATA NewPosiY = 0;
	u8 _XDATA ItemCount = 0;

	BakCurrentPage = InitPage;
	BakCurrentItem = ItemPosi;

	pMenu1CurPage = Menu1FindPage(InitPage);	/*Set current Page*/
//	assert(pMenu1CurPage);
	if(ItemPosi < pMenu1CurPage->uItemNum)
		CurItemPos = ItemPosi;
	else
		CurItemPos = 0;
	pMenu1CurItem = &pMenu1CurPage->pItemArray[CurItemPos];	/*Set current Item*/
	do{
		if(!IsItemSkipCursor(pMenu1CurPage,pMenu1CurItem))
			{
			break;
			}
		CurItemPos ++;
		if(CurItemPos >= pMenu1CurPage->uItemNum)
			{
			CurItemPos = 0;
			}
		pMenu1CurItem = &(pMenu1CurPage->pItemArray[CurItemPos]);

		ItemCount ++;
		}while(ItemCount < pMenu1CurPage->uItemNum);

	NewPosiY = pMenu1CurItem->Posi.Posi_Y;

	if((NewPosiY + pMenu1CurItem->Posi.Size) > SCREEN_Y_MAX)
		{
		sPosi.MaxY = NewPosiY + pMenu1CurItem->Posi.Size - 1;
		sPosi.MinY = sPosi.MaxY - SCREEN_Y_MAX; 
		}
	else
		{
		sPosi.MinY = 0;
		sPosi.MaxY = sPosi.MinY + SCREEN_Y_MAX;
		}

	if(Display)
		{
		Menu1Display(pMenu1CurPage,Display);
		}
}

/************************************************************************/
/*      Local Functions                                                 */
/************************************************************************/
static void sMenu1ProcKeyNext(void)
{
	u8 _XDATA NewPosiY=0;//PrevPosiY
	u8 _XDATA ItemCount = 0;

	BitCurPosi = 0;
	do{
//		PrevPosiY = pMenu1CurItem->Posi.Posi_Y;
		CurItemPos ++;
		if(CurItemPos >= pMenu1CurPage->uItemNum)
			{
			CurItemPos = 0;
			}
		pMenu1CurItem = (MenuItem*)(&(pMenu1CurPage->pItemArray[CurItemPos]));
		NewPosiY = pMenu1CurItem->Posi.Posi_Y;
		if(NewPosiY < sPosi.MinY)	/*返回开头*/
			{
			sPosi.MinY = NewPosiY;
			sPosi.MaxY = sPosi.MinY + SCREEN_Y_MAX;
			}
		else	/*正向*/
			{
			if((NewPosiY + pMenu1CurItem->Posi.Size) > (sPosi.MaxY + 1))
				{
				sPosi.MaxY = NewPosiY + pMenu1CurItem->Posi.Size - 1;
				sPosi.MinY = sPosi.MaxY - SCREEN_Y_MAX; 
				}
			}
		if(!IsItemSkipCursor(pMenu1CurPage,pMenu1CurItem))
			{
			break;
			}
		ItemCount ++;
		}while(ItemCount < pMenu1CurPage->uItemNum);
}

static void sMenu1ProcKeyPrev(void)
{
	u8 _XDATA PrevPosiY,NewPosiY;
	u8 _XDATA ItemCount = 0;

	BitCurPosi = 0;
	do{
		PrevPosiY = pMenu1CurItem->Posi.Posi_Y;
		if(CurItemPos)
			{
			CurItemPos --;
			}
		else
			{
			CurItemPos = pMenu1CurPage->uItemNum - 1;
			}
		pMenu1CurItem = (MenuItem*)(&(pMenu1CurPage->pItemArray[CurItemPos]));
		NewPosiY = pMenu1CurItem->Posi.Posi_Y;
		if(NewPosiY > PrevPosiY)	/*返回结尾*/
			{
			if((NewPosiY + pMenu1CurItem->Posi.Size) > (sPosi.MaxY + 1))
				{
				sPosi.MaxY = NewPosiY + pMenu1CurItem->Posi.Size - 1;
				sPosi.MinY = sPosi.MaxY - SCREEN_Y_MAX;
				}
			}
		else	/*逆向*/
			{
			if(NewPosiY < sPosi.MinY)
				{
				sPosi.MinY = NewPosiY;
				sPosi.MaxY = sPosi.MinY + SCREEN_Y_MAX;
				}
			}
		if(!IsItemSkipCursor(pMenu1CurPage,pMenu1CurItem))
			{
			break;
			}
		ItemCount ++;
		}while(ItemCount < pMenu1CurPage->uItemNum);
}

static u8 IsItemSkipCursor(MenuPage* pCurPage,MenuItem* pItem)
{
	ParamMenu2.mode = _CHECK_SKIP_MODE;
	ParamMenu2.para1 = 0;
	ParamMenu2.mId = pCurPage->mId;
	if(pItem->pFunc)
		{
		pItemMenu2 = pItem; 
		pItem->pFunc();
		}
	if(ParamMenu2.para1)
		return 1;	/*bit2:SKIP display,bit1:SKIP cursor*/
	return 0;
}

static u8 IsItemSkipDisp(MenuPage* pCurPage,const MenuItem* pItem)
{
	ParamMenu2.mode = _CHECK_SKIP_MODE;
	ParamMenu2.para1 = 0;
	ParamMenu2.mId = pCurPage->mId;

	if(pItem->pFunc)
		{
		pItemMenu2 = pItem;
		pItem->pFunc();
		}
	if(ParamMenu2.para1 & 0x02)
		return 1;	/*bit2:SKIP display,bit1:SKIP cursor*/
	return 0;
}

static void Menu1Display(MenuPage* pCurPage,u8 mode)//mode 1:清屏更新 2:不清屏更新
{
	u8 _XDATA lines;	/*当前页总共的item数*/
	u8 _XDATA i,ItemMinY = 0,ItemMaxY = 0;
	static pMenuItem _XDATA pItem;

	lines = pCurPage->uItemNum;
	pItem = &pCurPage->pItemArray[0];

	if(mode != 2)
		LcdClearScreen();//debugg

	for(i=0;i<lines;i++,pItem++)
		{
		if(IsItemSkipDisp(pCurPage,pItem))
			{
			continue;
			}

		ParamMenu2.mode = _DISP_MODE;
		if(pMenu1CurItem == pItem)
			ParamMenu2.para1 = 1;
		else
			ParamMenu2.para1 = 0;
		ParamMenu2.Posi.Posi_X = pItem->Posi.Posi_X;
		ParamMenu2.Posi.Posi_Y = pItem->Posi.Posi_Y - sPosi.MinY;
		ParamMenu2.Posi.Size   = pItem->Posi.Size;
		if(pItem->Posi.Posi_Y < ItemMinY)
			{
			ItemMinY = pItem->Posi.Posi_Y;
			}
		if(pItem->Posi.Posi_Y > ItemMaxY)
			{
			ItemMaxY = pItem->Posi.Posi_Y;
			}

		if(ParamMenu2.Posi.Posi_Y > SCREEN_Y_MAX)
			{
			continue;
			}
		else
			{
			pItemMenu2 = pItem; 
			pItem->pFunc();
			}
		}

	if(ItemMinY < sPosi.MinY)
		{
		SetPosi(&Pos,LCD_X_MAX-8,0,16);
		LcdPrnText(Pos,LCD_CAP,"↑");
		}
	if(ItemMaxY > sPosi.MaxY)
		{
		SetPosi(&Pos,LCD_X_MAX-8,LCD_Y_MAX-16,16);
		LcdPrnText(Pos,LCD_CAP,"↓");
		}

	LcdBufToLcd(MenuSys.PrfToCom);
}

void Menu1KeyService(KEYT key)
{
	static u8 _XDATA same_key_cnter = 0;
	static KEYT _XDATA old_key=0;

	if(old_key == key)
		{
		same_key_cnter ++;
		}
	else
		{
		same_key_cnter = 1;
		old_key = key;
		}

	ParamMenu2.mode  = _KEY_MODE;
	ParamMenu2.mId   = pMenu1CurPage->mId;
	ParamMenu2.para1 = key;
	ParamMenu2.para2 = same_key_cnter;/* 连续按键同一个按键次数 */

	pItemMenu2 = pMenu1CurItem; 
	pMenu1CurItem->pFunc();
	
	if(ParamMenu2.mode == _GOTO_MODE)
		{
		LcdClearScreen();//debugg
		pMenu1CurPage->SPosi.MinY = sPosi.MinY;
		pMenu1CurPage->SPosi.MaxY = sPosi.MaxY;
		Menu1Open((PageId)ParamMenu2.para1,ParamMenu2.para2,0);
		}
	if(ParamMenu2.mode == _BACK_MODE)
		{
		if(pMenu1CurPage->ParId != NULL_PAGE)
			{
			LcdClearScreen();//debugg
			sPosi.MinY = pMenu1CurPage->SPosi.MinY;
			sPosi.MaxY = pMenu1CurPage->SPosi.MaxY;
			Menu1Open(pMenu1CurPage->ParId,ParamMenu2.para1,0);
			}
		}
	if(ParamMenu2.mode == _KEY_MODE)
		{
		key = ParamMenu2.para1;	/*返回KEY值*/

		switch(key)
			{
			case _KEY_NEXT:
				sMenu1ProcKeyNext();
				break;
			case _KEY_PREV:
				sMenu1ProcKeyPrev();
				break;
			}
		}
	Menu1Display(pMenu1CurPage,1);
}

/************************************************************************/
/*     Application Interface                                            */
/************************************************************************/

tMenuSys _XDATA MenuSys;

GLOBAL void MenuInit(void)
{
	MenuSys.MenuPrf    = 0;
	MenuSys.EepromSave = 0;
	MenuSys.PrfToCom   = 0;// 串口打印液晶显示数据
	MenuSys.CntS       = 3;// Logo界面停留时间
	
	Menu1Open(LOGO_PAGE,0,1);
}

GLOBAL void ProB_100mS_Menu(void)
{
	static u8 _XDATA Cnt10mS = 0;
	KEYT _XDATA key;
	
	Cnt10mS ++;
	if(Cnt10mS >= 10)
		{
		Cnt10mS = 0;
		if(MenuSys.CntS)
			MenuSys.CntS--;
		
		if(Menu1GetCurPage() == LOGO_PAGE)
			{
			if(MenuSys.CntS == 0)
				Menu1Open(MAIN_PAGE,0,1); 
			}

		if(Menu1GetCurPage() == MAIN_PAGE)
			{
			if(MenuSys.EepromSave)
				{
				MenuSys.EepromSave   = 0;
				SysParam.EepromFlag  = 1;
				}
			}
		}
	
	if(key_enable)
		{
		key_enable = 0;
		key = _key;
		SetLamp(1,5,0);
		SetBeep(1,5,0);
		Menu1KeyService(key);
		if(MenuSys.MenuPrf)
			DPRINTF(("KeyVal:%d\r\n",key));
		}
}


/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool Menu_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".Menu\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".menu\r\n") == NULL)
		{
		DPRINTF(("MenuMsg\r\n"));
		DPRINTF(("MenuPg  <0~%bd> 1L 2C 3V 4M 5S\r\n",END_PAGE-1));
		DPRINTF(("MenuDis <0~1> Dis Buf Printf\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"menumsg\r\n") == NULL)
		{
		DPRINTF(("PrfToCom:%bd\r\n",MenuSys.PrfToCom));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"menupg") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		if(pComDbg.Param8[0] && (pComDbg.Param8[0] < END_PAGE))
			{
			Menu1Open(pComDbg.Param8[0],0,1);
			return TRUE;
			}
		}

	if(strcmp(pComDbg.Revdata,"menudis") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		MenuSys.PrfToCom = pComDbg.Param8[0];
		DPRINTF(("PrfToCom <= %bd\r\n",MenuSys.PrfToCom));
		return TRUE;
		}
	return FALSE;
}

#endif


