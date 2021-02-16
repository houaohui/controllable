#include "ball.h"
#include "LQ12864.h"
extern unsigned char adc[4];
struct BALL
{
	unsigned char x;//xλ��
	unsigned char y;
	int velocity_x; //x����
	int velocity_y;
	//�ĸ���Ե
	unsigned char top;
	unsigned char bottom;
	unsigned char left;
	unsigned char right;
}ball;

unsigned char bar_x = 30;//��������
unsigned char last_bar_x;//�ϴΰ�ֽλ�ã������жϰ�ֽ���˶�����
unsigned char bar_l = 10;//���ӳ���
unsigned char bar_movedir=30;//�����˶�����
unsigned char guanqia = 1;//�ؿ�
unsigned char delay = 29;//��ʱʱ��
unsigned char stop = 0;
unsigned char master_x=32;  //����λ��
unsigned char master_y=9;
unsigned char master_velocity_x=1;  //�����ٶ�
unsigned char master_velocity_y=1;
unsigned char master_life=32;

unsigned char myabs(int x)
{
	if(x>=0)
		return x;
	else
		return -x;
}
void Delay(unsigned char j)		//@30.000MHz
{
	unsigned char i=10;
	while(j--)
	{
		while(i--);
	}
}
//����
void OLED_DrawHinder(unsigned char center,unsigned char halflength)
{
	unsigned char i;
	OLED_Write_GRAM(center-halflength-1,60,0);
	OLED_Write_GRAM(center+halflength+1,60,0);
	for(i = center-halflength;i<center+halflength;i++)
	{
		OLED_Write_GRAM(i,60,1);
	}
}

void PrintMap() 					//��ӡ��ͼ
{
	u8 xdata i,j;
	for(i = 0; i<64 ; i++)
		for(j = 0; j<64; j++)
			if(i==0||j==0||i==63||j==63)
				OLED_Write_GRAM(i,j,1);
		OLED_Refresh();			//���Դ��е�ֵ����OLED 
}

void Print_Menu() 		//��ӡ�����˵�
{
	OLED_ShowCHinese(15,2,11);//��
	OLED_ShowCHinese(32,2,12);//Ϸ
	OLED_ShowCHinese(15,4,13);//��
	OLED_ShowCHinese(32,4,14);//��
}
void Print_num(unsigned char L,unsigned char score)				//��ӡ����
{
	unsigned char xdata ch[3]={0,0,'\0'};
	ch[0]=score/10%10+'0';
	ch[1]=score%10+'0';
	LCD_P8x16Str(103,L,ch);
}

void Draw_master(unsigned char x,unsigned char y,bit i)
{
	x=x-8;
	y=y-8;
		OLED_Write_GRAM(x+1,y,i);
		OLED_Write_GRAM(x+2,y,i);
		OLED_Write_GRAM(x+12,y,i);
		OLED_Write_GRAM(x+2,y+1,i);
		OLED_Write_GRAM(x+3,y+1,i);
		OLED_Write_GRAM(x+5,y+1,i);
		OLED_Write_GRAM(x+6,y+1,i);
		OLED_Write_GRAM(x+7,y+1,i);
		OLED_Write_GRAM(x+8,y+1,i);
		OLED_Write_GRAM(x+9,y+1,i);
		OLED_Write_GRAM(x+10,y+1,i);
		OLED_Write_GRAM(x+11,y+1,i);
		OLED_Write_GRAM(x+12,y+1,i);
		OLED_Write_GRAM(x+3,y+2,i);
		OLED_Write_GRAM(x+4,y+2,i);
		OLED_Write_GRAM(x+5,y+2,i);
		OLED_Write_GRAM(x+6,y+2,i);
		OLED_Write_GRAM(x+7,y+2,i);
		OLED_Write_GRAM(x+8,y+2,i);
		OLED_Write_GRAM(x+9,y+2,i);
		OLED_Write_GRAM(x+10,y+2,i);
		OLED_Write_GRAM(x+11,y+2,i);
		OLED_Write_GRAM(x+12,y+2,i);
		OLED_Write_GRAM(x+3,y+3,i);
		OLED_Write_GRAM(x+4,y+3,i);
		OLED_Write_GRAM(x+5,y+3,i);
		OLED_Write_GRAM(x+6,y+3,i);
		OLED_Write_GRAM(x+9,y+3,i);
		OLED_Write_GRAM(x+10,y+3,i);
		OLED_Write_GRAM(x+11,y+3,i);
		OLED_Write_GRAM(x+12,y+3,i);
		OLED_Write_GRAM(x+2,y+4,i);
		OLED_Write_GRAM(x+3,y+4,i);
		OLED_Write_GRAM(x+4,y+4,i);
		OLED_Write_GRAM(x+5,y+4,i);
		OLED_Write_GRAM(x+10,y+4,i);
		OLED_Write_GRAM(x+11,y+4,i);
		OLED_Write_GRAM(x+12,y+4,i);
		OLED_Write_GRAM(x+13,y+4,i);
		OLED_Write_GRAM(x+2,y+5,i);
		OLED_Write_GRAM(x+3,y+5,i);
		OLED_Write_GRAM(x+4,y+5,i);
		OLED_Write_GRAM(x+5,y+5,i);
		OLED_Write_GRAM(x+8,y+5,i);
		OLED_Write_GRAM(x+10,y+5,i);
		OLED_Write_GRAM(x+11,y+5,i);
		OLED_Write_GRAM(x+12,y+5,i);
		OLED_Write_GRAM(x+13,y+5,i);
		OLED_Write_GRAM(x+2,y+6,i);
		OLED_Write_GRAM(x+3,y+6,i);
		OLED_Write_GRAM(x+4,y+6,i);
		OLED_Write_GRAM(x+5,y+6,i);
		OLED_Write_GRAM(x+8,y+6,i);
		OLED_Write_GRAM(x+10,y+6,i);
		OLED_Write_GRAM(x+11,y+6,i);
		OLED_Write_GRAM(x+12,y+6,i);
		OLED_Write_GRAM(x+13,y+6,i);
		OLED_Write_GRAM(x+14,y+6,i);
		OLED_Write_GRAM(x+2,y+7,i);
		OLED_Write_GRAM(x+3,y+7,i);
		OLED_Write_GRAM(x+4,y+7,i);
		OLED_Write_GRAM(x+5,y+7,i);
		OLED_Write_GRAM(x+6,y+7,i);
		OLED_Write_GRAM(x+9,y+7,i);
		OLED_Write_GRAM(x+10,y+7,i);
		OLED_Write_GRAM(x+12,y+7,i);
		OLED_Write_GRAM(x+13,y+7,i);
		OLED_Write_GRAM(x+1,y+8,i);
		OLED_Write_GRAM(x+2,y+8,i);
		OLED_Write_GRAM(x+3,y+8,i);
		OLED_Write_GRAM(x+4,y+8,i);
		OLED_Write_GRAM(x+6,y+8,i);
		OLED_Write_GRAM(x+7,y+8,i);
		OLED_Write_GRAM(x+8,y+8,i);
		OLED_Write_GRAM(x+9,y+8,i);
		OLED_Write_GRAM(x+12,y+8,i);
		OLED_Write_GRAM(x+13,y+8,i);
		OLED_Write_GRAM(x+14,y+8,i);
		OLED_Write_GRAM(x+1,y+9,i);
		OLED_Write_GRAM(x+2,y+9,i);
		OLED_Write_GRAM(x+3,y+9,i);
		OLED_Write_GRAM(x+4,y+9,i);
		OLED_Write_GRAM(x+12,y+9,i);
		OLED_Write_GRAM(x+13,y+9,i);
		OLED_Write_GRAM(x+14,y+9,i);
		OLED_Write_GRAM(x+2,y+10,i);
		OLED_Write_GRAM(x+3,y+10,i);
		OLED_Write_GRAM(x+4,y+10,i);
		OLED_Write_GRAM(x+5,y+10,i);
		OLED_Write_GRAM(x+11,y+10,i);
		OLED_Write_GRAM(x+12,y+10,i);
		OLED_Write_GRAM(x+13,y+10,i);
		OLED_Write_GRAM(x+14,y+10,i);
		
		OLED_Write_GRAM(x+2,y+11,i);
		OLED_Write_GRAM(x+3,y+11,i);
		OLED_Write_GRAM(x+5,y+11,i);
		OLED_Write_GRAM(x+6,y+11,i);
		OLED_Write_GRAM(x+10,y+11,i);
		OLED_Write_GRAM(x+11,y+11,i);
		OLED_Write_GRAM(x+12,y+11,i);
		OLED_Write_GRAM(x+13,y+11,i);
		OLED_Write_GRAM(x+14,y+11,i);
		OLED_Write_GRAM(x+2,y+12,i);
		OLED_Write_GRAM(x+3,y+12,i);
		OLED_Write_GRAM(x+4,y+12,i);
		OLED_Write_GRAM(x+5,y+12,i);
		OLED_Write_GRAM(x+6,y+12,i);
		OLED_Write_GRAM(x+7,y+12,i);
		OLED_Write_GRAM(x+8,y+12,i);
		OLED_Write_GRAM(x+9,y+12,i);
		OLED_Write_GRAM(x+10,y+12,i);
		OLED_Write_GRAM(x+11,y+12,i);
		OLED_Write_GRAM(x+12,y+12,i);
		OLED_Write_GRAM(x+13,y+12,i);
		OLED_Write_GRAM(x+4,y+13,i);
		OLED_Write_GRAM(x+5,y+13,i);
		OLED_Write_GRAM(x+6,y+13,i);
		OLED_Write_GRAM(x+7,y+13,i);
		OLED_Write_GRAM(x+9,y+13,i);
		OLED_Write_GRAM(x+10,y+13,i);
		OLED_Write_GRAM(x+11,y+13,i);
		OLED_Write_GRAM(x+5,y+14,i);
		OLED_Write_GRAM(x+6,y+14,i);
		OLED_Write_GRAM(x+10,y+14,i);
}
void master_play()
{
	static unsigned char delay=0;
	unsigned char i=0;
	delay++;
	if(master_life>30&&master_life<50)
		i=1;
	else if(master_life<=30)
		i=2;
	if(delay>=5)
	{
		delay=0;
		switch(i)
		{
			case 0:
				Draw_master(master_x,master_y,1);  //����ͼ��
				break;
			
			case 1:
				Draw_master(master_x,master_y,0);  //�����һ��ͼ��
				master_x += master_velocity_x;
				Draw_master(master_x,master_y,1);  //����ͼ��
				if ((master_x<=8)||(master_x>=55))
				{
					master_velocity_x = -master_velocity_x;
				}
				break;
			case 2:
				Draw_master(master_x,master_y,0);  //�����һ��ͼ��
				master_x += master_velocity_x;
				master_y += master_velocity_y;
				Draw_master(master_x,master_y,1);  //����ͼ��
				if ((master_x<=8)||(master_x+16>=55))
				{
					master_velocity_x = -master_velocity_x;
				}
				if ((master_y<=9)||(master_y>=44))
				{
					master_velocity_y = -master_velocity_y;
				}
				break;
		}
	}
	
}



//����Ϸ��Ҫ����
void ball_test(void)
{
	unsigned char balllife=0;
	unsigned char fen=0,miao=0,timeindex=0;
	master_velocity_x=1;
	master_velocity_y=1;
	master_x=32;
	master_y=9;
	LCD_CLS();
	master_life=60;
	ball.x = 6;
	ball.y = 6;
	ball.velocity_x = 1;
	ball.velocity_y = 1;
	ball.top = 5;
	ball.bottom = 62;
	ball.left = 4;
	ball.right = 60;
	delay = 0;//��ʱ
	stop = 0;//��ͣ��־λ
	PrintMap();   //��ӡ��ͼ
	OLED_ShowCHinese(64,0,24);//��
	OLED_ShowCHinese(81,0,25);//��
	OLED_ShowCHinese(64,2,22);//��
	OLED_ShowCHinese(81,2,23);//��
	OLED_ShowCHinese(64,4,26);//��
	OLED_ShowCHinese(81,4,27);//ʱ
	Print_num(0,master_life);
	Print_num(2,balllife);
	
	while(1)
	{
		Delay(1);
		delay++;
		
		//�������ˢ����
		if(delay == 5)
		{
			delay=0;
			timeindex++;
			if(timeindex>=20)
			{
				timeindex=0;
				miao++;
				Print_num(4,fen); 
				 
				if(miao==60)
				{
					miao=0;
					fen++;
				}
			}
			OLED_Write_GRAM(ball.x,ball.y,0);
			ball.x += ball.velocity_x;
			ball.y += ball.velocity_y;
			OLED_Write_GRAM(ball.x,ball.y,1);
			//�ж����λ��
			if ((ball.x<=ball.left)||(ball.x>=ball.right))
			{
				ball.velocity_x = -ball.velocity_x;
			}
			if (ball.y<=ball.top)
			{
				ball.velocity_y = -ball.velocity_y;
			}
			if(ball.y==59&&myabs(ball.x-bar_x)<=bar_l)
			{
				balllife++;
				ball.velocity_y = -ball.velocity_y;
				switch(bar_movedir)
				{
					case 0:ball.velocity_x--;break;
					case 1:ball.velocity_x++;break;
					case 2:break;
				}
			}
			if (ball.y>=ball.bottom)
			{
				balllife-=2;
				ball.velocity_y = -ball.velocity_y;
			}
			//��ײ
			if((myabs(ball.x-master_x)<=8)&&(myabs(ball.y-master_y)<=8))
			{
				ball.velocity_x = -ball.velocity_x;
				ball.velocity_y = -ball.velocity_y;
				master_life--;
			}
			if(balllife>=255||master_life>=255)
			{
				break;
			}
			Print_num(2,balllife);
			Print_num(0,master_life);
			Print_num(6,miao); 
		}
		
		//ˢ�µ���
		if(adc[1]<85)				//��ⰴ��key1�Ƿ���  up
		{
				
		}
		else if(adc[0]<85)		//��ⰴ��key2�Ƿ���    left
		{
			bar_x -= 1;if(bar_x<5)bar_x=5;
		}
		else if(adc[1]>170)		//��ⰴ��key3�Ƿ���  down
		{
			
		}
		else if(adc[0]>170)		//��ⰴ��key4�Ƿ���    right
		{
			bar_x += 1;if(bar_x>59)bar_x=59;
		}
		//ɨ�赲���˶�����
		if(bar_x>last_bar_x)
		{
			bar_movedir=1;//�����˶�
		}
		else if(bar_x==last_bar_x)
		{
			bar_movedir=2;//���˶�
		}
		else if(bar_x<last_bar_x)
		{
			bar_movedir=0;//�����˶�
		}
		last_bar_x=bar_x; 
		OLED_DrawHinder(bar_x,bar_l);
		
		master_play();
		OLED_Refresh();
	}
	Print_Menu(); 	//��ӡ�����˵�
}