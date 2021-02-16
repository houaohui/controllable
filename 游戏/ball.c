#include "ball.h"
#include "LQ12864.h"
extern unsigned char adc[4];
struct BALL
{
	unsigned char x;//x位置
	unsigned char y;
	int velocity_x; //x方向
	int velocity_y;
	//四个边缘
	unsigned char top;
	unsigned char bottom;
	unsigned char left;
	unsigned char right;
}ball;

unsigned char bar_x = 30;//板子中心
unsigned char last_bar_x;//上次板纸位置，用于判断板纸的运动方向
unsigned char bar_l = 10;//板子长度
unsigned char bar_movedir=30;//板子运动方向
unsigned char guanqia = 1;//关卡
unsigned char delay = 29;//延时时间
unsigned char stop = 0;
unsigned char master_x=32;  //怪兽位置
unsigned char master_y=9;
unsigned char master_velocity_x=1;  //怪兽速度
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
//挡板
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

void PrintMap() 					//打印地图
{
	u8 xdata i,j;
	for(i = 0; i<64 ; i++)
		for(j = 0; j<64; j++)
			if(i==0||j==0||i==63||j==63)
				OLED_Write_GRAM(i,j,1);
		OLED_Refresh();			//把显存中的值赋给OLED 
}

void Print_Menu() 		//打印结束菜单
{
	OLED_ShowCHinese(15,2,11);//游
	OLED_ShowCHinese(32,2,12);//戏
	OLED_ShowCHinese(15,4,13);//结
	OLED_ShowCHinese(32,4,14);//束
}
void Print_num(unsigned char L,unsigned char score)				//打印分数
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
				Draw_master(master_x,master_y,1);  //怪兽图像
				break;
			
			case 1:
				Draw_master(master_x,master_y,0);  //清空上一个图像
				master_x += master_velocity_x;
				Draw_master(master_x,master_y,1);  //怪兽图像
				if ((master_x<=8)||(master_x>=55))
				{
					master_velocity_x = -master_velocity_x;
				}
				break;
			case 2:
				Draw_master(master_x,master_y,0);  //清空上一个图像
				master_x += master_velocity_x;
				master_y += master_velocity_y;
				Draw_master(master_x,master_y,1);  //怪兽图像
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



//球游戏主要函数
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
	delay = 0;//延时
	stop = 0;//暂停标志位
	PrintMap();   //打印地图
	OLED_ShowCHinese(64,0,24);//怪
	OLED_ShowCHinese(81,0,25);//兽
	OLED_ShowCHinese(64,2,22);//生
	OLED_ShowCHinese(81,2,23);//命
	OLED_ShowCHinese(64,4,26);//用
	OLED_ShowCHinese(81,4,27);//时
	Print_num(0,master_life);
	Print_num(2,balllife);
	
	while(1)
	{
		Delay(1);
		delay++;
		
		//下面的是刷新球
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
			//判断球的位置
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
			//相撞
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
		
		//刷新挡板
		if(adc[1]<85)				//检测按键key1是否按下  up
		{
				
		}
		else if(adc[0]<85)		//检测按键key2是否按下    left
		{
			bar_x -= 1;if(bar_x<5)bar_x=5;
		}
		else if(adc[1]>170)		//检测按键key3是否按下  down
		{
			
		}
		else if(adc[0]>170)		//检测按键key4是否按下    right
		{
			bar_x += 1;if(bar_x>59)bar_x=59;
		}
		//扫描挡板运动方向
		if(bar_x>last_bar_x)
		{
			bar_movedir=1;//向右运动
		}
		else if(bar_x==last_bar_x)
		{
			bar_movedir=2;//不运动
		}
		else if(bar_x<last_bar_x)
		{
			bar_movedir=0;//向左运动
		}
		last_bar_x=bar_x; 
		OLED_DrawHinder(bar_x,bar_l);
		
		master_play();
		OLED_Refresh();
	}
	Print_Menu(); 	//打印结束菜单
}