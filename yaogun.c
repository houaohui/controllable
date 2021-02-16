#include "yaogun.h"
#include "LQ12864.h"
sbit key0=P3^2;
sbit key1=P3^3;

unsigned char xdata keysta[10] = {
 1, 1, 1, 1 ,1, 1,   1, 1 ,1, 1};

//adc0,adc1,adc2,adc3,key0,key1,    adc6,adc7,adc8,adc9
//
unsigned char code keycodemap[10] = {
 0, 1, 2, 3 ,4, 5,   6 ,7, 8, 9};
//adc0,adc1,adc2,adc3,key0,key1,    adc6,adc7,adc8,adc9
 
 
unsigned char xdata backupkey[10] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	
extern unsigned char adc[4];
extern unsigned char GetADCResult(unsigned char ch);
extern void keyaction(unsigned char keycode);
 
 
void YaoGunScan()
{
	//unsigned char i;
	static unsigned char keyout=0;
//	static unsigned char xdata keybuf[2]={
//	0xff,0xff};
	if(keyout==4) keyout=0;
	switch(keyout)
	{
		case 0:adc[keyout]=GetADCResult(keyout);if(adc[keyout]>170)keysta[keyout]=0;else keysta[keyout]=1;if(adc[keyout]<85)keysta[6]=0;else keysta[6]=1;break;
		case 1:adc[keyout]=GetADCResult(keyout);if(adc[keyout]>170)keysta[keyout]=0;else keysta[keyout]=1;if(adc[keyout]<85)keysta[7]=0;else keysta[7]=1;break;
		case 2:adc[keyout]=GetADCResult(keyout);if(adc[keyout]>170)keysta[keyout]=0;else keysta[keyout]=1;if(adc[keyout]<85)keysta[8]=0;else keysta[8]=1;break;
		case 3:adc[keyout]=GetADCResult(keyout);if(adc[keyout]>170)keysta[keyout]=0;else keysta[keyout]=1;if(adc[keyout]<85)keysta[9]=0;else keysta[9]=1;break;
		default:break;
	}
//	keybuf[0]=(keybuf[0]<<1)|key0;
//	keybuf[1]=(keybuf[1]<<1)|key1;
	keysta[4]=key0;
	keysta[5]=key1;
//	for(i=0;i<2;i++)
//	{
//		if((keybuf[i]&0x0f)==0x00)
//		{
//			keysta[i+4]=0;
//		}
//		else if((keybuf[i]&0x0f)==0x0f)
//		{
//			keysta[i+4]=1;
//		}
//	}
	keyout++;
}

//void keydriver()
//{
//	unsigned char i;
//	for (i=0; i<10; i++)
//	{
//			if (backupkey[i] != keysta[i])
//			{
//				if (backupkey[i] == 1)
//				{
//					keyaction(keycodemap[i]);
//				}
//				backupkey[i] = keysta[i];
//			}
//	}
//}




