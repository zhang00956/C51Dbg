#ifndef _KEY_H
#define _KEY_H

typedef	unsigned char KEYT;

enum{
_NO_KEY=0,
_KEY_UP,         /* 1 */
_KEY_DN,         /* 2 */
_KEY_NEXT,       /* 3 */
_KEY_PREV,       /* 4 */
_KEY_RUN,        /* 5 */
_KEY_LONG_RUN,   /* 6 */
_KEY_SETUP,      /* 7 */
};

extern vu8 _IRAM key_enable,_key;

GLOBAL void ProF_10mS_Key(void);

#endif