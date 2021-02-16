#include "display.h"
#include "LQ12864.h"
extern bit flag;  //10ms
extern unsigned char adc[4];
extern unsigned char TIM[3];


void DisplayYaoGunMode1()
{
	unsigned char x0,x1,x2,x3,backup0,backup1,backup2,backup3;
	if(flag)
	{
		x0=adc[0]*50/255;
		x1=adc[1]*7/255+1;
		x2=adc[2]*50/255;
		x3=adc[3]*7/255+1;
		LCD_P6x8Str(backup0+70,4," ");
		LCD_P6x8Str(x0+70,4,"+");
		LCD_P6x8Str(96,backup1," "); 
		LCD_P6x8Str(96,x1,"+");
		
		LCD_P6x8Str(backup0+70,backup1," ");
		LCD_P6x8Str(x0+70,x1,"o");
		
		LCD_P6x8Str(backup2,4," "); 
		LCD_P6x8Str(x2,4,"+");  
		LCD_P6x8Str(26,backup3," "); 
		LCD_P6x8Str(26,x3,"+");  
		
		LCD_P6x8Str(backup2,backup3," "); 
		LCD_P6x8Str(x2,x3,"o");  
		
		backup0=x0;
		backup1=x1;
		backup2=x2;
		backup3=x3;
		flag=0;
	}
}

void DisplayYaoGunMode2()
{
	unsigned char ch[4];
	if(flag)
	{
		ch[0]=adc[0]/100%10+'0';
		ch[1]=adc[0]/10%10+'0';
		ch[2]=adc[0]%10+'0';
		ch[3]='\0';
		LCD_P6x8Str(92,2,ch); 
		ch[0]=adc[1]/100%10+'0';
		ch[1]=adc[1]/10%10+'0';
		ch[2]=adc[1]%10+'0';
		ch[3]='\0';
		LCD_P6x8Str(92,3,ch); 
		ch[0]=adc[2]/100%10+'0';
		ch[1]=adc[2]/10%10+'0';
		ch[2]=adc[2]%10+'0';
		ch[3]='\0';
		LCD_P6x8Str(20,2,ch); 
		ch[0]=adc[3]/100%10+'0';
		ch[1]=adc[3]/10%10+'0';
		ch[2]=adc[3]%10+'0';
		ch[3]='\0';
		LCD_P6x8Str(20,3,ch); 
		flag=0;
	}
}

void ShowTime()
{
	if(flag)
	{
		unsigned char buf[6];
		buf[0]=TIM[0]/10+'0';
		buf[1]=TIM[0]%10+'0';
		buf[2]=':';
		buf[3]=TIM[1]/10+'0';
		buf[4]=TIM[1]%10+'0';
		buf[5]='\0';
		LCD_P8x16Str(45,2,buf);
		buf[0]=(TIM[2]/10%10)+'0';
		buf[1]=TIM[2]%10+'0';
		buf[2]='\0';
		LCD_P6x8Str(58,4,buf);
	}
}





