#define SYSLOG 0

#include "InkerSys.h"
#include "MenuTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


MenuProcParam _XDATA ParamMenu2;
pMenuItem pItemMenu2;
u8 _XDATA AboveNumStore = 0; //同种属性的界面区分
u8 _XDATA EepromUpdate  = 0; //参数改变时标记升级
u8 _XDATA BitCurPosi    = 0; //参数位光标控制

u8 MenuPutStr(ItemPosi Posi,u8 mode,StrType* Str)
{	
	if(SysParam.Language)
		return LcdPrnText(Posi,mode,Str->StrEn);
	else
		return LcdPrnText(Posi,mode,Str->StrCh);
}

static void CalcU16Value(u16 * int_value,u8 bit_position,u8 key_val)
{
	u16 _XDATA temp = *int_value;
	char _XDATA StrBuf[16],Ch;

	if(temp >= 10000)
		temp -= 10000;

	sprintf(StrBuf, "%04d", temp);
	Ch = StrBuf[bit_position];

	if(Ch < '0')
		Ch = '0';

	switch(key_val)
		{
		case _KEY_UP:
			Ch += 1;
			if(Ch>'9')
				Ch = '0';
			break;
		case _KEY_DN:
			Ch -=1;
			if(Ch<'0')
				Ch = '9';
			break;
		}
	StrBuf[bit_position] = Ch;
	*int_value = atoi(StrBuf);
}

_ROM u8 FloatGainFocusPosi[] = {0,2,3,4,5,6,7,8};	/*用来显示当前输入位置的下划线*/
static void CalcFloatGainValue(float * float_value,u8 bit_position,u8 key_val)
{
	float temp = *float_value;
	char StrBuf[16],Ch;

	if(temp < 0)
		temp = 0 - temp;
	if(temp >= 10)
		temp -= 10;

	sprintf(StrBuf, "%1.5f", temp);

	if(bit_position)
		Ch = StrBuf[bit_position+1];
	else
		Ch = StrBuf[bit_position];

	switch(key_val)
		{
		case _KEY_UP:
			Ch += 1;
			if(Ch>'9')
				Ch = '0';
			break;
		case _KEY_DN:
			Ch -=1;
			if(Ch<'0')
				Ch = '9';
			break;
		}


	if(bit_position)
		StrBuf[bit_position+1] = Ch;
	else
		StrBuf[bit_position] = Ch;

	*float_value = atof(StrBuf);	 //把字符型转换成浮点型
}

#if 0
static void CalcAsciiValue(char * StrBuf,u8 bit_position,u8 key_val)
/*ASCII*/
{
	char Ch;
	Ch = *(StrBuf + bit_position);

	switch(key_val)
		{
		case _KEY_UP:
			Ch += 1;
			if(Ch>'9')
				Ch = '0';
			break;
		case _KEY_DN:
			Ch -=1;
			if(Ch<'0')
				Ch = '9';
			break;
		}

	*(StrBuf + bit_position) = Ch;
}
#endif

SETUP_T ProcGotoPage(void)
{
	switch(ParamMenu2.mode)
		{
		case _DISP_MODE:
			if(ParamMenu2.para1)
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CPL | LCD_UPDAT,pItemMenu2->pCap);
			else
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CAP | LCD_UPDAT,pItemMenu2->pCap);
			break;
		case _KEY_MODE:
			switch(ParamMenu2.para1)
				{
				case _KEY_RUN:
					ParamMenu2.mode = _GOTO_MODE;
					ParamMenu2.para1 = pItemMenu2->uPara;
					ParamMenu2.para2 = pItemMenu2->uOptionNum;
					break;
				}
			break;
		}
	return TRUE;
}

SETUP_T ProcBackPage(void)
{
	switch(ParamMenu2.mode)
		{
		case _DISP_MODE:
			if(ParamMenu2.para1)
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CPL | LCD_UPDAT,pItemMenu2->pCap);
			else
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CAP | LCD_UPDAT,pItemMenu2->pCap);
			break;
		case _KEY_MODE:
			switch(ParamMenu2.para1)
				{
				case _KEY_RUN:
					ParamMenu2.mode = _BACK_MODE;
					ParamMenu2.para1 = pItemMenu2->uPara;
					ParamMenu2.para2 = pItemMenu2->uOptionNum;
					
					if(Menu1GetCurPage() == SETUP_PAGE || Menu1GetCurPage() == CODE_PAGE)//退出设置界面 保存参数
						{
						if(EepromUpdate)
							{
							EepromUpdate = 0;
							MenuSys.EepromSave = 1;/* 通知参数改变存储 */
							//LoadDefault();
							}
						}	
					break;
				}
			break;
		}
	return TRUE;
}

SETUP_T ProLogoPage(void)
{
	switch(ParamMenu2.mode)
		{			
		case _DISP_MODE:
			SetPosi(&Pos,24, 0,16);
			LcdPrnText(Pos,LCD_CAP,"%s",GetProName());

			SetPosi(&Pos,32, 16,16);
			LcdPrnText(Pos,LCD_CAP,"%s",GetProModel());
			
			SetPosi(&Pos,0, 32,16);
			LcdPrnText(Pos,LCD_CAP,"%s",GetCompany());

			SetPosi(&Pos,8, 48,16);
			LcdPrnText(Pos,LCD_CAP,"N:%00004u",SysParam.ProductId);

			SetPosi(&Pos,82,48,16);
			LcdPrnText(Pos,LCD_CAP,"V:%s",GetVersionNum());
			break;
			
		case _KEY_MODE:
			ParamMenu2.mode  = _BACK_MODE;
			ParamMenu2.para1 = pItemMenu2->uPara;
			ParamMenu2.para2 = pItemMenu2->uOptionNum;
			break;
		}
	return TRUE;
}

SETUP_T ProVersPage(void)
{
	switch(ParamMenu2.mode)
		{
		case _DISP_MODE:		
			SetPosi(&Pos,0, 0,16);
			LcdPrnText(Pos,LCD_CAP,"S:%s",GetSystem());
			SetPosi(&Pos,0,16,16);
			LcdPrnText(Pos,LCD_CAP,"T:%s",GetBuildedDate());
			SetPosi(&Pos,0,32,16);
			LcdPrnText(Pos,LCD_CAP,"V:%s",GetVersionNum());
			SetPosi(&Pos,64,32,16);
			LcdPrnText(Pos,LCD_CAP,"EE:%000005lu",SysParam.EraseEepromNum);
			SetPosi(&Pos,0,48,16);
			LcdPrnText(Pos,LCD_CAP,"N:%s",GetAuther());		
			break;
		case _KEY_MODE:
			ParamMenu2.mode = _BACK_MODE;
			ParamMenu2.para1 = pItemMenu2->uPara;
			ParamMenu2.para2 = pItemMenu2->uOptionNum;
			break;
		}
	return TRUE;
}

SETUP_T ProMainPage(void)
{
	switch(ParamMenu2.mode)
		{
		case _DISP_MODE:	
			SetPosi(&Pos, 0, 0,16);
			LcdPrnText(Pos,LCD_CAP,"****************");	
			
			SetPosi(&Pos,24,16,16);
			MenuPutStr(Pos,LCD_CAP,&StrProductName);
			
			SetPosi(&Pos, 0,32,16);
			MenuPutStr(Pos,LCD_CAP,&StrCompany);
			
			SetPosi(&Pos,0,48,16);
			LcdPrnText(Pos,LCD_CAP,"================");	
			break;
		case _KEY_MODE:
			switch(ParamMenu2.para1)
				{
				case _KEY_RUN:
					ParamMenu2.mode = _GOTO_MODE;
					ParamMenu2.para1 = SETUP_PAGE;
					ParamMenu2.para2 = 0;
					break;
				case _KEY_SETUP:
					if(ParamMenu2.para2 == 5 || SysParam.Dbg)
						{
						ParamMenu2.mode = _GOTO_MODE;
						ParamMenu2.para1 = CODE_PAGE;
						ParamMenu2.para2 = 0;
						}
					break;
				}
			break;
		}
	return TRUE;
}


SETUP_T ProcBatterFix(void)
{
	static u8 _XDATA BitCurPosi = 0;

	switch(ParamMenu2.mode)
		{
		case _INIT_MODE:
			break;
		case _LOAD_EEPROM_MODE:
			break;
		case _LOAD_DEFAULT_MODE:
			SysParam.BatterFix = 1.0;
			break;
		case _DISP_MODE:
			if(ParamMenu2.para1)
				{
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CPL,pItemMenu2->pCap);
				SetPosi(&Pos,ParamMenu2.Posi.Posi_X + (ParamMenu2.Posi.Size/2) * FloatGainFocusPosi[BitCurPosi],ParamMenu2.Posi.Posi_Y,ParamMenu2.Posi.Size);
				ParamMenu2.Posi.Posi_X += LcdPrnText(ParamMenu2.Posi,LCD_CAP,"%0.3f",SysParam.BatterFix);
				LcdPrnText(Pos,LCD_ORL | LCD_UPDAT,"_");
				}
			else
				{
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CAP,pItemMenu2->pCap);
				ParamMenu2.Posi.Posi_X += LcdPrnText(ParamMenu2.Posi,LCD_CAP | LCD_UPDAT,"%0.3f",SysParam.BatterFix);
				}
			break;
		case _KEY_MODE:
			switch(ParamMenu2.para1)
				{
				case _KEY_RUN:
					BitCurPosi ++;
					BitCurPosi %= 4;
					break;
				case _KEY_DN:
				case _KEY_UP:	
					CalcFloatGainValue(&SysParam.BatterFix,BitCurPosi,ParamMenu2.para1);
					EepromUpdate = 1;
					break;
				}
			break;
		}
	return TRUE;
}


SETUP_T ProcLanguage(void)
{
	_ROM OptionType ModeTab[] = {
		{0,	&StrLangCh},
		{1,	&StrLangEn}};

	switch(ParamMenu2.mode)
		{
		case _DISP_MODE:
			if(ParamMenu2.para1)
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CPL,pItemMenu2->pCap);
			else
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CAP,pItemMenu2->pCap);
			ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CAP | LCD_UPDAT,ModeTab[SysParam.Language].Str);
			break;
		case _KEY_MODE:
			switch(ParamMenu2.para1)
				{
				case _KEY_UP:
				case _KEY_DN:
					if(SysParam.Language)
						SysParam.Language = 0;
					else
						SysParam.Language = 1;
					EepromUpdate = 1;
					break;
				}
			break;
		}
	return TRUE;
}

SETUP_T ProcBrightness(void)
{
	_ROM u8 MaxVal = 8;
	switch(ParamMenu2.mode)
		{	
		case _DISP_MODE:
			if(ParamMenu2.para1)
				{
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CPL,pItemMenu2->pCap);
				ParamMenu2.Posi.Posi_X += LcdPrnText(ParamMenu2.Posi,LCD_CAP,"%bd",SysParam.Brightness);
				}
			else
				{
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CAP,pItemMenu2->pCap);
				ParamMenu2.Posi.Posi_X += LcdPrnText(ParamMenu2.Posi,LCD_CAP,"%bd",SysParam.Brightness);
				}
			break;
		case _KEY_MODE:
			switch(ParamMenu2.para1)
				{
				case _KEY_DN:
					if(SysParam.Brightness)
						SysParam.Brightness--;
					break;
				case _KEY_UP:
					if(SysParam.Brightness < (MaxVal-1))
						{
						SysParam.Brightness++;
						}
					else
						{
						SysParam.Brightness = 7;
						}
					break;
				}
			break;
		}												
	return TRUE;
}

SETUP_T ProcProductIdSet(void)
{
	switch(ParamMenu2.mode)
		{		
		case _DISP_MODE:
			if(ParamMenu2.para1)
				{
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CPL,pItemMenu2->pCap);
				SetPosi(&Pos,ParamMenu2.Posi.Posi_X + (ParamMenu2.Posi.Size/2) * BitCurPosi,ParamMenu2.Posi.Posi_Y,ParamMenu2.Posi.Size);
				ParamMenu2.Posi.Posi_X += LcdPrnText(ParamMenu2.Posi,LCD_CAP,"%04d",SysParam.ProductId);
				LcdPrnText(Pos,LCD_ORL | LCD_UPDAT,"_");
				}
			else
				{
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CAP,pItemMenu2->pCap);
				ParamMenu2.Posi.Posi_X += LcdPrnText(ParamMenu2.Posi,LCD_CAP | LCD_UPDAT,"%04d",SysParam.ProductId);
				}
			break;
		case _KEY_MODE:
			switch(ParamMenu2.para1)
				{
				case _KEY_RUN:
					BitCurPosi ++;
					BitCurPosi %= 4;
					break;
				case _KEY_DN:
				case _KEY_UP:
					CalcU16Value(&SysParam.ProductId,BitCurPosi,ParamMenu2.para1);
					EepromUpdate = 1;
					break;
				}
			break;
		}
	return TRUE;
}

SETUP_T ProcDbgSet(void)
{
	switch(ParamMenu2.mode)
		{
		case _DISP_MODE:
			if(ParamMenu2.para1)
				{
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CPL,pItemMenu2->pCap);
				}
			else
				{
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CAP,pItemMenu2->pCap);
				}
			ParamMenu2.Posi.Posi_X += LcdPrnText(ParamMenu2.Posi,LCD_CAP | LCD_UPDAT,"%c",SysParam.Dbg?'Y':'N');
			break;
		case _KEY_MODE:
			switch(ParamMenu2.para1)
				{
				case _KEY_UP:
				case _KEY_DN:
					if(SysParam.Dbg)
						SysParam.Dbg = 0;
					else
						SysParam.Dbg = 1;
					EepromUpdate = 1;
					break;
				}
			break;
		}

	return TRUE;
}

SETUP_T ProcUpData(void)
{
	switch(ParamMenu2.mode)
		{
		case _DISP_MODE:
			if(ParamMenu2.para1)
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CPL,pItemMenu2->pCap);
			else
				ParamMenu2.Posi.Posi_X += MenuPutStr(ParamMenu2.Posi,LCD_CAP,pItemMenu2->pCap);
			break;
		case _KEY_MODE:
			switch(ParamMenu2.para1)
				{
				case _KEY_LONG_RUN:
					StcIap();
					break;
				}
			break;
		}

	return TRUE;
}

