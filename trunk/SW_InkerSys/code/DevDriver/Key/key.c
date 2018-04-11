
#define SYSLOG 0

#include "inkersys.h"
#include "key.h"

#define ALONE_KEY1V 0X01
#define ALONE_KEY2V 0X02
#define ALONE_KEY3V 0X04
#define ALONE_KEY4V 0X08

#define _NEW_KEY_PRESS_TIME 6  //按键去斗时间     （单位为一个系统节拍）
#define _KEY_HOLD_TIME      12 //按键连按间隔时间 （单位为一个系统节拍）

enum{
KEY_NONE, 
KEY_PRESS,
KEY_HOLD,
KEY_UP,
};

typedef struct {
u8 key_value;
u8 key_mode;
}_PANEL_KEY,*_pPANEL_KEY;

struct PanelKey{
u8 key_value;
u8 key;
};

vu8 _IRAM key_enable = 0,_key = 0;

_ROM struct PanelKey alone_key_table[] = 
{
{ALONE_KEY1V,_KEY_UP},     /*1*/
{ALONE_KEY2V,_KEY_DN},     /*2*/
{ALONE_KEY3V,_KEY_NEXT},   /*3*/
{ALONE_KEY4V,_KEY_RUN},	   /*4*/
{ALONE_KEY1V | ALONE_KEY2V,_KEY_SETUP},
{0,0}
};

static u8 GetAloneKey(void)
{
	u8 rtn_key = 0;
	
	if(GetKey1() == 0)
		rtn_key |= ALONE_KEY1V;
	if(GetKey2() == 0)
		rtn_key |= ALONE_KEY2V;
	if(GetKey3() == 0)
		rtn_key |= ALONE_KEY3V;
	if(GetKey4() == 0)
		rtn_key |= ALONE_KEY4V;
	return rtn_key;
}

static KEYT alone_convert_key_value(u8 key)
{
	u8 i;
	for(i=0;alone_key_table[i].key_value;i++)
		{
		if(key == alone_key_table[i].key_value)
			return alone_key_table[i].key;
		}
	return _NO_KEY;
}

static _PANEL_KEY read_key(void)
{
	static u8 _IRAM pre_key = 0;
	static u8 _IRAM key_counter = 0;
	
	_PANEL_KEY temp_key;
	u8 key,key_mode;

	key = alone_convert_key_value(GetAloneKey());

	key_mode = KEY_NONE;
	if(key)
		{
		if(key != pre_key)	/*新按键，重新开始按键计数*/
			{
			key_counter = 0;
			pre_key = key;
			}
		else	
			{
			key_counter ++;
			if(key_counter == _NEW_KEY_PRESS_TIME)	/*按键要连续按住_NEW_KEY_PRESS_TIME*10ms (周期为10ms)才算有效*/
				{
				key_mode = KEY_PRESS;	/*新按键按下*/
				}
			if(key_counter == (_NEW_KEY_PRESS_TIME * 2 + _KEY_HOLD_TIME + 1))	/*连按之间的间隔为_KEY_HOLD_TIME*10ms */
				{
				key_counter = _NEW_KEY_PRESS_TIME * 2 +1;
				key_mode = KEY_HOLD;
				}
			}
		}
	else
		{
		if(pre_key)
			{
			if(key_counter >= _NEW_KEY_PRESS_TIME)	/*之前必须已经有按键按下才能认为是按键松开*/
				{
				key_mode = KEY_UP;	/*按键释放执行模式*/
				key = pre_key;
				}
			pre_key = 0;
			}
		key_counter = 0;
		}
	temp_key.key_value = key;
	temp_key.key_mode = key_mode;
	return temp_key;
}

static void proc_key_press(KEYT key_value)
{
	switch(key_value)
		{
		case _KEY_NEXT:
		case _KEY_RUN:
			break;
			
		default:
			key_enable = 1;
			_key = key_value;
			break;
		}
}

static void proc_key_hold(KEYT key_value,u8 hold_time)
{
	switch(key_value)
		{
		case _KEY_UP:
		case _KEY_DN:	
			if(hold_time > 3)
				{
				key_enable = 1;
				_key = key_value;
				}
			break;
		
		case _KEY_NEXT:
			if((hold_time == 5) || ((hold_time > 9) && (hold_time & 0x1)))
				{
				key_enable = 1;
				_key = _KEY_PREV;
				}
			break;
			
		case _KEY_RUN:
			if(hold_time == 10)
				{
				key_enable = 1;
				_key = _KEY_LONG_RUN;
				}
			break;
		}
}

static void proc_key_up(KEYT key_value,u8 hold_time)
{
	switch(key_value)
		{		
		case _KEY_NEXT:
			if(hold_time < 6)
				{
				key_enable = 1;
				_key = key_value;
				}
			break;
			
		case _KEY_RUN:
			if(hold_time < 6)
				{
				key_enable = 1;
				_key = key_value;
				}
			break;	
		}
}

GLOBAL void ProF_10mS_Key(void)
{
	static u8 _IRAM key_hold_timer;
	
	_PANEL_KEY panel_key;
	
	panel_key = read_key();
	switch(panel_key.key_mode)
		{
		case KEY_PRESS:
			key_hold_timer = 0;
			proc_key_press(panel_key.key_value);
			break;
		case KEY_HOLD:	/*连按之间的间隔为_KEY_HOLD_TIME*10ms ,在这里定为100ms*/
			key_hold_timer ++;
			proc_key_hold(panel_key.key_value,key_hold_timer);
			break;
		case KEY_UP:
			proc_key_up(panel_key.key_value,key_hold_timer);
			key_hold_timer = 0;
			break;
		}
}


/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool Key_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".Key\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".key\r\n") == NULL)
		{
		DPRINTF(("Key <u8> 1+ 2- 3U 4D 5G 6LG 7C;\r\n"));
		return TRUE;
		}
		
	if(strcmp(pComDbg.Revdata,"key\r\n") == NULL)
		{
		DPRINTF(("Key = %bd\r\n",_key));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"key") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		_key = pComDbg.Param8[0];
		key_enable = 1;
		DPRINTF(("Key <= %bd\r\n",_key));
		return TRUE;
		}
	
	return FALSE;
}

#endif

