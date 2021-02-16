#ifndef _YAOGUN_H
#define _YAOGUN_H
#include "stc12c5a60s2.h"

extern unsigned char xdata keysta[10];
extern unsigned char xdata backupkey[10];
extern unsigned char code keycodemap[10];

void YaoGunScan();
//void keydriver();



#endif
