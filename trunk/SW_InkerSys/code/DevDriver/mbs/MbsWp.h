#ifndef _MBSWP_H
#define _MBSWP_H

enum{
SET_UNKNOWU,  /*����δд����*/
SET_ERROR,    /*���ò�������*/
SET_OK,       /*���ò����ɹ�*/
};

GLOBAL u8 WpReg(u16 Addr,u8 RxdBuf[],u8 *len);

#endif

