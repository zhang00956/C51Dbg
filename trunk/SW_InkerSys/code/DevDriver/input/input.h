#ifndef _INPUT_H
#define _INPUT_H

enum{
INPUT0,
INPUT1,
};

GLOBAL void ProF_10mS_InPut(void);
GLOBAL   u8 GetInput(u8 index);

#endif

