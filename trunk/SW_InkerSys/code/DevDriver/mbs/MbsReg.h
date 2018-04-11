#ifndef _MBSREG_H
#define _MBSREG_H

enum{
/* 线圈       Cmd:0X01 0X05 0X0F */
MBSREG_COIL_START     = 0X0001,
MBSREG_COIL_RL01      = MBSREG_COIL_START,
MBSREG_COIL_RL02,
MBSREG_COIL_RL03,
MBSREG_COIL_RL04,
MBSREG_COIL_RL05,
MBSREG_COIL_RL06,
MBSREG_COIL_RL07,
MBSREG_COIL_RL08,
MBSREG_COIL_RL09,
MBSREG_COIL_RL10,
MBSREG_COIL_RL11,
MBSREG_COIL_RL12,
MBSREG_COIL_RL13,
MBSREG_COIL_RL14,
MBSREG_COIL_RL15,
MBSREG_COIL_RL16,
MBSREG_COIL_RL17,
MBSREG_COIL_RL18,
MBSREG_COIL_RL19,
MBSREG_COIL_RL20,
MBSREG_COIL_RL21,
MBSREG_COIL_RL22,
MBSREG_COIL_RL23,
MBSREG_COIL_RL24,
MBSREG_COIL_RL25,
MBSREG_COIL_RL26,
MBSREG_COIL_RL27,
MBSREG_COIL_RL28,
MBSREG_COIL_RL29,
MBSREG_COIL_ZODX,
MBSREG_COIL_ZOEN,
MBSREG_COIL_LEN,

/* 保持寄存器 Cmd:0X03 0X06 0X10 */
MBSREG_HOLD_S      = 0X0000,
MBSREG_HOLD_MBSID  = MBSREG_HOLD_S,
MBSREG_HOLD_INFT,
MBSREG_HOLD_DAC,
MBSREG_HOLD_SETADC0,
MBSREG_HOLD_GETADC0,
MBSREG_HOLD_FIX0,
MBSREG_HOLD_FIX0S,
MBSREG_HOLD_FIX1,
MBSREG_HOLD_FIX1S,
MBSREG_HOLD_LEN,
};

typedef struct{/* 中断有操作变量 必须定义成用 volatile 关键字修饰 */
/* Coil Bit  Cmd:0X01 0X05 0X0F */
u8  sCoil;
u8  Coil_RL01;
u8  Coil_RL02;
u8  Coil_RL03;
u8  Coil_RL04;
u8  Coil_RL05;
u8  Coil_RL06;
u8  Coil_RL07;
u8  Coil_RL08;
u8  Coil_RL09;

u8  Coil_RL10;
u8  Coil_RL11;
u8  Coil_RL12;
u8  Coil_RL13;
u8  Coil_RL14;
u8  Coil_RL15;
u8  Coil_RL16;
u8  Coil_RL17;
u8  Coil_RL18;
u8  Coil_RL19;

u8  Coil_RL20;
u8  Coil_RL21;
u8  Coil_RL22;
u8  Coil_RL23;
u8  Coil_RL24;
u8  Coil_RL25;
u8  Coil_RL26;
u8  Coil_RL27;
u8  Coil_RL28;
u8  Coil_RL29;

u8  Coil_ZODX;
u8  Coil_ZOEN;
u8  eCoil;

/* Hold  Reg  Cmd:0X03 0X06 0X10 */
/* 51是1字节处理器 所以无需将2字节变量与4字节变量分开 */	

u16   s2Hold;
u16   MbsId;
u16   InputFt;
u16   DacOutVal;
u16   SetAdc0Val;
u16   GetAdc0Val;
float Adc0Fix;
float Adc1Fix;
u16   e2Hold;
}tMbsReg,*ptMbsReg;

//extern tMbsReg volatile _XDATA MbsReg;
extern tMbsReg _XDATA MbsReg;

GLOBAL void RegLoadDefault(void);
GLOBAL void RegReadEeprom(void);
GLOBAL void RegSaveEeprom(void);
GLOBAL void RegMsg(void);
GLOBAL void MbsRegInit(void);

GLOBAL bool MbsReg0X01(u16 addr,u16 num,u8 txd[]);
GLOBAL bool MbsReg0X05(u16 addr,u16 val);
GLOBAL bool MbsReg0X0F(u16 addr,u16 num,u8 rxd[]);

GLOBAL bool MbsReg0X03(u16 addr,u16 num,u8 txd[]);
GLOBAL bool MbsReg0X06(u16 addr,u16 val);
GLOBAL bool MbsReg0X10(u16 addr,u16 num,u8 rxd[]);



#endif

