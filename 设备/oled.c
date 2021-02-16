#include "oled.h"
#include "oledfont.h" 

unsigned char xdata OLED_GRAM[64][8] = {0};

/*****************************************************************/
//��ʱ���� 			   
/*****************************************************************/
//���ܣ���SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 xdata i;					//����д���ѭ������ 
	if(cmd)				 
		OLED_DC_Set();		//���д�����ݣ�DC���� 
	else 
		OLED_DC_Clr();		//����д�����DC���� 
	//OLED_CS_Clr();			//Ƭѡ���ͣ�׼��д�� 
	for(i=0;i<8;i++)		//ѭ��д�� 
	{			  
		OLED_SCLK_Clr();	//Ƭѡ���� 
		if(dat&0x80)		// 1000 0000 dat��0x80x���룬if�ж����λ��ȫ�����棬�Դ˴������� 
		{
		   OLED_SDIN_Set();	//���ݽ����� 
		}
		else
		{
		   OLED_SDIN_Clr();	//���ݽ����� 			
		}
		OLED_SCLK_Set();	//ʱ���ź������� 
		dat<<=1;   
	}				 		  
	//OLED_CS_Set();			//Ƭѡ�������� ֹͣд�� 
	OLED_DC_Set();   	  	//Ϊʲô������DC�ܽţ����� 
} 
/*****************************************************************/
//ҳѰַģʽ�µ�Ѱ�����꺯�� 
void OLED_Set_Pos(unsigned char x, unsigned char y) //����OLED��ʾλ�� 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);					//ҳ����ģʽ��+yѡ��ҳ 
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);		//���������� ���õ��ǵ�ַ��λ 10h-1fh
	OLED_WR_Byte((x&0x0f),OLED_CMD); 				//���������� ���õ��ǵ�ַ��λ 00h-0fh
}
/*****************************************************************/		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
//void OLED_Clear(void)  
//{
//	u8 xdata i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
//		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
//		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
//		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
//	} //������ʾ
//}
/*****************************************************************/
//��ʾ��ͼ�ϵĵ� 
//�����õ���y�����µ�����ϵ 
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
//��OLED�����Դ����� 
//�����Դ浽OLED	
void OLED_Refresh(void)
{
	unsigned char xdata i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //��������ʼ��ַ
	   OLED_WR_Byte(0x00,OLED_CMD);   //���õ�����ʼ��ַ
	   OLED_WR_Byte(0x10,OLED_CMD);   //���ø�����ʼ��ַ
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
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63 
//void OLED_ShowChar(u8 x,u8 y,u8 chr)
//{      	
//	unsigned char c=0,i=0;	
//		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
//		if(x>Max_Column-1){x=0;y=y+2;}
//		OLED_Set_Pos(x,y);	
//		for(i=0;i<8;i++)
//		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
//		OLED_Set_Pos(x,y+1);
//		for(i=0;i<8;i++)
//		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
//}
/*****************************************************************/		  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//num:��ֵ(0~4294967295);	 		  
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
//��ʾ����
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
