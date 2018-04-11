
#define SYSLOG 0

#include "inkersys.h"

#ifdef _GLOBAL_COMDBG_ENABLE
 
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define ComdbgUart(x) Uart0Send(x)

tComDbg _XDATA pComDbg;

static bool AnalyComDbg(void);

/*将字符串命令与参数之间第一个出现的字符fcahr替换为bchar */
static bool CmdAndDataIsolate(u8 *str,u8 fchar,u8 bchar)
{
	u8 i = 0;
	while(i < COMDBGBUFNUM)
		{
		if(*str == fchar)/*搜索到字符串的第一个空格*/
			{
			*str = bchar;
			return TRUE;
			}
		if(*str == '\r')//遇到回车 结束搜索
			return FALSE;
		str++;
		i++;
		}
	return FALSE;
}

static void UppertoLower(u8 *str)/*大写字母转小写字母*/
{
	while(*str != '\0')
		{
		if(*str >= 'A' && *str <= 'Z')
			{
			*str = *str + 32;
			}
		str++;
		}
}

GLOBAL u8 SearchChar(u8 *str,u8 asiic)/*搜索字符并返回字符所在地址*/
{
	u8 i=0;
	while(i < COMDBGBUFNUM)
		{
		if(*str == asiic)/*搜索到返回地址*/
			{
			return i;
			}
		str++;
		i++;
		}
	return 0XFF;
}

GLOBAL bool EndIsEnter(u8 *str)
{
	if(*str == '\r')
		return TRUE;
	else
		return FALSE;
}

GLOBAL bool TypeBool(u8 *str,bool *val)/*参数为位类型*/
{
	bool result = FALSE;
	if(*str == '1')
		{
		*val = TRUE;
		result = TRUE;
		}
	if(*str == '0')
		{
		*val = FALSE;
		result = TRUE;
		}
	pComDbg.Cnt += 2;
	return result;
}

GLOBAL bool TypeString(u8 *str,u8 Array[])/*参数为8位类型*/
{
	bool result = TRUE;
	u8 i = 0,temp8 = 0;

	if(*str != '"')
		return FALSE;
	str++;
	pComDbg.Cnt++;
	
	while(i < STRINGTYPESIZE-3)
		{
		if(*str == '"')
			{
			pComDbg.Cnt++;
			Array[i] = '\0';
			break;
			}
		else
			{
			Array[i] = *str;
			str++;
			i++;
			pComDbg.Cnt++;
			}
		}
	
	if(i == 0)/*参数之间不是一个空格错误*/
		{
		result = FALSE;
		}
	
	pComDbg.Cnt++;
	return result;
}

GLOBAL bool TypeU8(u8 *str,u8 *val)/*参数为8位类型*/
{
	bool result = TRUE;
	u8 i = 0,temp8 = 0;

	if(*str == 'H' || *str == 'h')//十六进制格式
		{
		str++;
		pComDbg.Cnt++;
		while(i < 2)
			{
			if(*str >= '0' && *str <= '9')
				{
				temp8 <<= 4;
				temp8 += (*str)&0X0F;
				str++;
				i++;
				pComDbg.Cnt++;
				}
			else if(*str >= 'a' && *str <= 'f')
				{
				temp8 <<= 4;
				temp8 += (*str)&0X0F;
				temp8 += 9;
				str++;
				i++;
				pComDbg.Cnt++;
				}
			else 
				{
				break;
				}
			}
		if(i == 0)/*参数之间不是一个空格错误*/
			{
			result = FALSE;
			}
		if(i == 2)/*数据范围错误*/
			{
			if(*str >= 'a' && *str <= 'f')
				{
				result = FALSE;
				}
			if(*str >= '0' && *str <= '9')
				{
				result = FALSE;
				}
			}
		}
	else
		{
		while(i < 3)
			{
			if(*str >= '0' && *str <= '9')
				{
				temp8 *= 10;
				temp8 += (*str)&0X0F;
				str++;
				i++;
				pComDbg.Cnt++;
				}
			else
				{
				break;
				}
			}
		if(i == 0)/*参数之间不是一个空格错误*/
			{
			result = FALSE;
			}
		if(i == 3)/*数据范围错误*/
			{
			if(*str >= '0' && *str <= '9')
				{
				result = FALSE;
				}
			}
		}

	*val = temp8;
	pComDbg.Cnt++;
	return result;
}

GLOBAL bool TypeU16(u8 *str,u16 *val)/*参数为16位类型*/
{
	bool result = TRUE;
	u8 i = 0;
	u16 temp16 = 0;

	if(*str == 'H' || *str == 'h')//十六进制格式
		{
		str++;
		pComDbg.Cnt++;
		while(i < 4)
			{
			if(*str >= '0' && *str <= '9')
				{
				temp16 <<= 4;
				temp16 += (*str)&0X0F;
				str++;
				i++;
				pComDbg.Cnt++;
				}
			else if(*str >= 'a' && *str <= 'f')
				{
				temp16 <<= 4;
				temp16 += (*str)&0X0F;
				temp16 += 9;
				str++;
				i++;
				pComDbg.Cnt++;
				}
			else 
				{
				break;
				}
			}
		if(i == 0)/*参数之间不是一个空格错误*/
			{
			result = FALSE;
			}
		if(i == 4)/*数据范围错误*/
			{
			if(*str >= 'a' && *str <= 'f')
				{
				result = FALSE;
				}
			if(*str >= '0' && *str <= '9')
				{
				result = FALSE;
				}
			}
		}
	else
		{
		while(i < 6)
			{
			if(*str >= '0' && *str <= '9')
				{
				temp16 *= 10;
				temp16 += (*str)&0X0F;
				str++;
				i++;
				pComDbg.Cnt++;
				}
			else
				{
				break;
				}
			}
		if(i == 0)/*参数之间不是一个空格错误*/
			{
			result = FALSE;
			}
		if(i == 6)/*数据范围错误*/
			{
			if(*str >= '0' && *str <= '9')
				{
				result = FALSE;
				}
			}
		}

	*val = temp16;
	pComDbg.Cnt++;
	return result;
}

GLOBAL bool TypeU32(u8 *str,u32 *val)/*参数为16位类型*/
{
	bool result = TRUE;
	u8  i = 0;
	u32 tempu32 = 0;

	if(*str == 'H' || *str == 'h')//十六进制格式
		{
		str++;
		pComDbg.Cnt++;
		while(i < 8)
			{
			if(*str >= '0' && *str <= '9')
				{
				tempu32 <<= 4;
				tempu32 += (*str)&0X0F;
				str++;
				i++;
				pComDbg.Cnt++;
				}
			else if(*str >= 'a' && *str <= 'f')
				{
				tempu32 <<= 4;
				tempu32 += (*str)&0X0F;
				tempu32 += 9;
				str++;
				i++;
				pComDbg.Cnt++;
				}
			else 
				{
				break;
				}
			}
		if(i == 0)/*参数之间不是一个空格错误*/
			{
			result = FALSE;
			}
		if(i == 8)/*数据范围错误*/
			{
			if(*str >= 'a' && *str <= 'f')
				{
				result = FALSE;
				}
			if(*str >= '0' && *str <= '9')
				{
				result = FALSE;
				}
			}
		}
	else
		{
		while(i < 10)
			{
			if(*str >= '0' && *str <= '9')
				{
				tempu32 *= 10;
				tempu32 += (*str)&0X0F;
				str++;
				i++;
				pComDbg.Cnt++;
				}
			else
				{
				break;
				}
			}
		if(i == 0)/*参数之间不是一个空格错误*/
			{
			result = FALSE;
			}
		if(i == 6)/*数据范围错误*/
			{
			if(*str >= '0' && *str <= '9')
				{
				result = FALSE;
				}
			}
		}

	*val = tempu32;
	pComDbg.Cnt++;
	return result;
}

GLOBAL bool TypeFloat(u8 *str,float *val)/*参数为浮点类型*/
{
	bool result = TRUE;
	u8 i = 0,flag = 0,temp8 = 0,sign = 0;
	float tempf1 = 0,tempf2 = 0,tempf3 = 0;

	if(*str == '-')//负号
		{
		str++;
		pComDbg.Cnt++;
		sign = 1;
		}
	
	while(i < 8)
		{
		if(*str >= '0' && *str <= '9')
			{
			if(flag)
				{
				tempf3 = (*str)&0X0F;
				if(flag == 1)
					tempf2 += tempf3/10;
				if(flag == 2)
					tempf2 += tempf3/100;
				if(flag == 3)
					tempf2 += tempf3/1000;
				if(flag == 4)
					tempf2 += tempf3/10000;
				if(flag == 5)
					tempf2 += tempf3/100000;
				if(flag == 6)
					tempf2 += tempf3/1000000;
				flag++;
				}
			else
				{
				tempf1 *= 10;
				tempf1 += (*str)&0X0F;
				}
			str++;
			i++;
			pComDbg.Cnt++;
			}
		else if(*str == '.' && flag == 0)
			{
			flag = 1;
			str++;
			i++;
			pComDbg.Cnt++;
			}
		else
			{
			break;
			}
		}
	
	if(i == 0)/*参数之间不是一个空格错误*/
		{
		result = FALSE;
		}
	if(i == 8)/*数据范围错误*/
		{
		if(*str >= '0' && *str <= '9')
			{
			result = FALSE;
			}
		}

	tempf3 = tempf1 + tempf2;
	
	
	if(sign)
		*val = -tempf3;
	else
		*val = tempf3;
	pComDbg.Cnt++;
	return result;
}

static bool PasswordLicense(void)
{
	if(strcmp(pComDbg.Revdata,"inker\r\n") == NULL)
		{
		SysParam.Dbg = 1;
		DPRINTF(("Welcom Inker System Debug Mode!\r\n"));
		memcpy(pComDbg.Revdata,"help\r\n",7);/* 登录默认命令为打印帮助信息 */
		return TRUE;
		}
	return FALSE;
}

GLOBAL void ComDbgInit(void)
{
	pComDbg.ComDbgCnt = 0;
	
	if(SysParam.Dbg)
		{
		memcpy(pComDbg.Revdata,"help\r\n",7);/* 启动默认命令为打印帮助信息 */
		memcpy(pComDbg.Str,"InkerSys",9);
		DPRINTF(("%s>","InkerSys"));
		}
}

GLOBAL void ProB_RealTime_ComDbg(void)
{
	if(RxdFlag)
		{
		RxdFlag = 0;

		if(SysParam.Dbg)
			{
			if((RxdData == 0X0D) || (RxdData == 0X0A))
				{
				if(pComDbg.ComDbgCnt)/*接收新命令*/
					{
					ComdbgUart('\r');
					ComdbgUart('\n');
					pComDbg.Revdata[pComDbg.ComDbgCnt++] = '\r';
					pComDbg.Revdata[pComDbg.ComDbgCnt++] = '\n';
					pComDbg.Revdata[pComDbg.ComDbgCnt++] = '\0';
					pComDbg.ComDbgCnt = 0;
					}
				else/*回车执行最近一次命令*/
					{
					CmdAndDataIsolate(pComDbg.Revdata,'\0',' ');/* 还原接收解析命令时 将命令字符串与参数之间 空格替换为 '\0' */
					DPRINTF(("%s",pComDbg.Revdata));
					}
				if(AnalyComDbg() == FALSE)
					DPRINTF(("%s\r\n",SysParam.Language?"无效命令":"Unknown Cmd"));
				
				if(SysParam.Dbg)
					{
					DPRINTF(("%b002u:%b002u:%b002u>",SysParam.SysHours,SysParam.SysMinutes,SysParam.SysSeconds));
					}
				else
					{
					DPRINTF(("Exit Inker Debug System!\r\n"));
					}
				}
			else
				{
				if(RxdData == 0X08)/* 清除 */
					{
					if(pComDbg.ComDbgCnt)
						pComDbg.ComDbgCnt--;
					}
				else
					{
					pComDbg.Revdata[pComDbg.ComDbgCnt] = RxdData;
					pComDbg.ComDbgCnt++;
					}
				
				if(pComDbg.ComDbgCnt >= COMDBGBUFNUM - 3)
					{
					pComDbg.ComDbgCnt = 0;
					DPRINTF(("\r\n%b002u:%b002u:%b002u>",SysParam.SysHours,SysParam.SysMinutes,SysParam.SysSeconds));
					}
				else
					{
					ComdbgUart(RxdData);
					}
				}
			}
		else
			{
			if((RxdData == 0X0D) || (RxdData == 0X0A))
				{
				if(pComDbg.ComDbgCnt)/*接收新命令*/
					{
					pComDbg.Revdata[pComDbg.ComDbgCnt++] = '\r';
					pComDbg.Revdata[pComDbg.ComDbgCnt++] = '\n';
					pComDbg.Revdata[pComDbg.ComDbgCnt++] = '\0';
					pComDbg.ComDbgCnt = 0;
					AnalyComDbg();
					}
				}
			else
				{
				if(RxdData == 0X08)/* 清除 */
					{
					if(pComDbg.ComDbgCnt)
						pComDbg.ComDbgCnt--;
					}
				else
					{
					pComDbg.Revdata[pComDbg.ComDbgCnt] = RxdData;
					pComDbg.ComDbgCnt++;
					}

				if(pComDbg.ComDbgCnt >= COMDBGBUFNUM - 2)
					{
					pComDbg.ComDbgCnt = 0;
					}
				}
			}
		}
}

static bool AnalyComDbg(void)
{
	if(SysParam.Dbg)
		{
		/* 命令字符串与参数隔离 并且命令字符串不区分大小写字母 而参数区分大小写 */
		CmdAndDataIsolate(pComDbg.Revdata,' ','\0');/* 将空格前的命令与参数隔离 回车重复执行命令时 需要还原此处改变 */
		UppertoLower(pComDbg.Revdata);              /* 将大写字母转成小写 从而不区分大小写*/

		if(Dbg_Comdbg()     == TRUE)
			return TRUE;
		if(System_Comdbg()  == TRUE)
			return TRUE;
		if(Format_Comdbg()  == TRUE)
			return TRUE;
		if(License_Comdbg() == TRUE)
			return TRUE;
		if(RunTime_Comdbg() == TRUE)
			return TRUE;
		if(McuBsp_Comdbg()  == TRUE)
			return TRUE;
		if(Prj_Comdbg()     == TRUE)
			return TRUE;
		if(Lamp_Comdbg()    == TRUE)
			return TRUE;
		if(Beep_Comdbg()    == TRUE)
			return TRUE;
		if(Key_Comdbg()     == TRUE)
			return TRUE;
		if(MbsRtu_Comdbg()     == TRUE)
			return TRUE;
		if(MbsReg_Comdbg()     == TRUE)
			return TRUE;
		if(Menu_Comdbg()    == TRUE)
			return TRUE;
		if(InPut_Comdbg()   == TRUE)
			return TRUE;
		if(InAdc_Comdbg()   == TRUE)
			return TRUE;
		if(LedSeg_Comdbg()   == TRUE)
			return TRUE;
		
		if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
			return TRUE;
		}
	else
		{
		return PasswordLicense();
		}
	return FALSE;
}

/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
GLOBAL bool Dbg_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".Dbg\r\n"));
		return FALSE;
		}
		
	if(strcmp(pComDbg.Revdata,".dbg\r\n") == NULL)
		{
		DPRINTF(("DbgMsg\r\n"));
		DPRINTF(("LogoName <\"str16\">\r\n"));
		DPRINTF(("ComDbg   <En1> <save1>\r\n"));
		DPRINTF(("Exit\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"dbgmsg\r\n") == NULL)
		{
		DPRINTF(("LogoName:%s\r\n",pComDbg.Str));
		DPRINTF(("InPut 0X3A => h3a or H3a\r\n"));
		return TRUE;
		}
	
	if(strcmp(pComDbg.Revdata,"logoname") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeString(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Str) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;
		
		DPRINTF(("%s\r\n",pComDbg.Str));
		return TRUE;
		}
		
	if(strcmp(pComDbg.Revdata,"comdbg") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeBool(pComDbg.Revdata + pComDbg.Cnt,pComDbg.ParamB) == FALSE)
			return FALSE;
		if(TypeBool(pComDbg.Revdata + pComDbg.Cnt,pComDbg.ParamB + 1) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;
		
		SysParam.Dbg = pComDbg.ParamB[0];
		if(pComDbg.ParamB[1])//退出并保存数据
			{
			SysParam.SysPrf     = 0;
			SysParam.UpPowerDbg = SysParam.Dbg;
			SysParam.EepromFlag = 1;
			}
		DPRINTF(("ComDbg <= %bd\r\n",SysParam.Dbg));
		return  TRUE;
		}

	if(strcmp(pComDbg.Revdata,"exit\r\n") == NULL)
		{
		SysParam.Dbg      = 0;
		SysParam.SysPrf   = 0;
		return  TRUE;
		}
	
	return FALSE;
}

/* #End Comdbg Interface */

#endif

