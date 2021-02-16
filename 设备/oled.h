#include "stc12c5a60s2.h"
#ifndef __OLED_H
#define __OLED_H			  	 

#define  u8 unsigned char 
#define  u16 unsigned int 
#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý
#define OLED_MODE 0


sbit OLED_CS=P2^5; //Æ¬Ñ¡
sbit OLED_DC =P0^7;//Êý¾Ý/ÃüÁî¿ØÖÆ
sbit OLED_RST =P0^6;//¸´Î»
sbit OLED_SDIN=P0^5;//D1£¨MOSI£© Êý¾Ý
sbit OLED_SCL=P0^4;//Ê±ÖÓ D0£¨SCLK£

#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1;





//OLEDÄ£Ê½ÉèÖÃ
//0:4Ïß´®ÐÐÄ£Ê½
//1:²¢ÐÐ8080Ä£Ê½

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED¶Ë¿Ú¶¨Òå----------------  					   

void delay_ms(u16 ms);


 		     

//OLED¿ØÖÆÓÃº¯Êý
void OLED_WR_Byte(u8 dat,u8 cmd);	   							   		    
//void OLED_Init(void);
//void OLED_Clear(void);
void OLED_Write_GRAM(u8 x,u8 y,bit value);
void OLED_Refresh(void);
//void OLED_ShowChar(u8 x,u8 y,u8 chr);
//void OLED_ShowNum(u8 x,u8 y,u16 num,u8 len,u8 size2); 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
//u16 oled_pow(u8 m,u8 n);
#endif  
	 



