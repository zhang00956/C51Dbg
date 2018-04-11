#ifndef _MBSWP_H
#define _MBSWP_H

enum{
SET_UNKNOWU,  /*设置未写保护*/
SET_ERROR,    /*设置参数错误*/
SET_OK,       /*设置参数成功*/
};

GLOBAL u8 WpReg(u16 Addr,u8 RxdBuf[],u8 *len);

#endif

