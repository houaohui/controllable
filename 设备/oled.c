#include "oled.h"
#include "oledfont.h" 

unsigned char xdata OLED_GRAM[64][8] = {0};

/*****************************************************************/
//延时函数 			   
/*****************************************************************/
//功能：向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 xdata i;					//用于写入的循环变量 
	if(cmd)				 
		OLED_DC_Set();		//如果写入数据，DC拉高 
	else 
		OLED_DC_Clr();		//否则写入命令，DC拉低 
	//OLED_CS_Clr();			//片选拉低，准备写入 
	for(i=0;i<8;i++)		//循环写入 
	{			  
		OLED_SCLK_Clr();	//片选拉低 
		if(dat&0x80)		// 1000 0000 dat与0x80x相与，if判断最高位，全真则真，以此传入数据 
		{
		   OLED_SDIN_Set();	//数据脚拉高 
		}
		else
		{
		   OLED_SDIN_Clr();	//数据脚拉低 			
		}
		OLED_SCLK_Set();	//时钟信号上升沿 
		dat<<=1;   
	}				 		  
	//OLED_CS_Set();			//片选数据拉高 停止写入 
	OLED_DC_Set();   	  	//为什么又拉高DC管脚？？？ 
} 
/*****************************************************************/
//页寻址模式下的寻找坐标函数 
void OLED_Set_Pos(unsigned char x, unsigned char y) //设置OLED显示位置 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);					//页设置模式，+y选中页 
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);		//设置纵坐标 设置的是地址高位 10h-1fh
	OLED_WR_Byte((x&0x0f),OLED_CMD); 				//设置纵坐标 设置的是地址低位 00h-0fh
}
/*****************************************************************/		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
//void OLED_Clear(void)  
//{
//	u8 xdata i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
//		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
//		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
//		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
//	} //更新显示
//}
/*****************************************************************/
//显示地图上的点 
//我设置的是y轴向下的坐标系 
void OLED_Write_GRAM(u8 x,u8 y,bit value)
{
	u8 xdata OLED_page = y/8;
	u8 xdata OLED_page_value = 1 << y%8;
	if(x>=64)
			return;
	if(value)OLED_GRAM[x][OLED_page]|=OLED_page_value;
	else OLED_GRAM[x][OLED_page]&=~OLED_page_value;
} 
/*****************************************************************/
//向OLED传输显存数据 
//更新显存到OLED	
void OLED_Refresh(void)
{
	unsigned char xdata i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<64;n++)
		{
			OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
			//delay_ms(1);
		}
	}
		for(i=0;i<64;i++)
			OLED_WR_Byte(0x00,OLED_DATA);
}
/*****************************************************************/
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63 
//void OLED_ShowChar(u8 x,u8 y,u8 chr)
//{      	
//	unsigned char c=0,i=0;	
//		c=chr-' ';//得到偏移后的值			
//		if(x>Max_Column-1){x=0;y=y+2;}
//		OLED_Set_Pos(x,y);	
//		for(i=0;i<8;i++)
//		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
//		OLED_Set_Pos(x,y+1);
//		for(i=0;i<8;i++)
//		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
//}
/*****************************************************************/		  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//num:数值(0~4294967295);	 		  
//void OLED_ShowNum(u8 x,u8 y,u16 num,u8 len,u8 size2)
//{         	
//	u8 t,temp;
//	u8 enshow=0;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/oled_pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				OLED_ShowChar(x+(size2/2)*t,y,' ');
//				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); 
//	}
//}
/*****************************************************************/
//u16 oled_pow(u8 m,u8 n)
//{
//	u16 xdata result=1;	 
//	while(n--)result*=m;    
//	return result;
//}
/*****************************************************************/
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 xdata t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/*****************************************************************/ 
