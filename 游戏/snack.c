#include "snack.h"
#include "yaogun.h"
#include "jianpan.h"
/*************************************************************/
Snake xdata snake[MAX],food,tail;				//蛇身体结构体，食物，尾巴 
u8 xdata tempkey,key = 4; 							//储存键盘键入值 同时蛇用于判断行走方向  初始移动方向为右 
u8 xdata length = 1;										//储存蛇的长度
u8 xdata mode = 5;											//储存选择的难度 20ms刷新率 困难mode=2 50ms刷新率 简单mode=5
bit wall = 1;											//储存是否有墙 1为有墙模式，0为无墙模式
u8 xdata time_count = 0;								//时间计数器 
extern unsigned char adc[4];
extern	unsigned char xdata keysta[10];
/*************************************************************/

void Snake_Init() 				//用于初始化蛇 
{
	snake[0].x=29;
	snake[0].y=29;
	snake[1].x=28;
	snake[1].y=29;
}
/*************************************************************/
void Snake_Move() 				//操控蛇移动 后一节给前一节赋值
{
	u8 xdata i;
	tail.x = snake[length].x;	//保留上次尾巴的坐标 
	tail.y = snake[length].y;	//当吃到食物的时候将该坐标赋给蛇结构第length个的坐标
	for(i = length;i > 0 ; i--)
	{
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}
	if(wall == 0)
	{
		if(snake[0].x == 63)	//如果与右墙相撞
		{
			snake[0].x = 1;			//到最左侧
		}
		else if(snake[0].x == 0)//如果与左墙相撞
		{
			snake[0].x = 62;			//到最右侧
		}
		else if(snake[0].y == 0)//如果与上墙相撞
		{
			snake[0].y = 62;			//到最下侧
		}
		else if(snake[0].y == 63)//如果与下墙相撞
		{
			snake[0].y = 0;			//到最上侧
		}
	}
}
/*************************************************************/
bit  Snake_eatself() 			//判断是否吃到自己
{
	u8 xdata i;
	for(i = 3; i <= length ; i++)	//蛇不可能和前三节相遇，因此从第四节开始遍历 
	{
		if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			return 1;	
	}
	return 0;
}
/*************************************************************/
bit  Snake_HitWall() 			//判断蛇是否撞墙 
{
	if(snake[0].x == 0 || snake[0].x == 63 || snake[0].y == 0 || snake[0].y == 63)
		return 1;
	else
		return 0; 
}
/*************************************************************/
/*************************************************************/
void Print_Map() 					//打印地图
{
	u8 xdata i,j;
	for(i = 0; i<64 ; i++)
		for(j = 0; j<64; j++)
			if(i==0||j==0||i==63||j==63)
				OLED_Write_GRAM(i,j,1);
		OLED_Refresh();			//把显存中的值赋给OLED 
}

u8 Scan_Keyboard(bit n)				//扫描键入值，返回键盘输入的值
{
	unsigned char result=0;
	static unsigned char ch=0;
	if (backupkey[4] != keysta[4])
	{
		if(backupkey[4]==1)
		{
			RunSnackFlag=~RunSnackFlag;
		}
		backupkey[4] = keysta[4];
	}
	if(n==1)
	{
		if(adc[1]<85)				//检测按键key1是否按下
		{	
				return 1;
		}
		else if(adc[0]<85)		//检测按键key2是否按下
		{	
				return 2;
		}
		else if(adc[1]>170)		//检测按键key3是否按下
		{
				return 3;
		}
		else if(adc[0]>170)		//检测按键key4是否按下
		{	
				return 4;
		}
		else if(adc[2]>170)		//检测按键key5是否按下
		{
			return 5;
		}

		return 0;				//ERROR
	}
	else
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					switch(keycodemap[ch])
					{
						case 0:result= 4;break;
						case 1:result= 3;break;
						case 6:result= 2;break;
						case 7:result= 1;break;
						case 2:result= 5;break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
			ch++;
			if(ch==8)
				ch=0;
			return result;
	}
}



void Print_StartMenu() 		//打印开始菜单 写入难度和模式 内置一个循环用于选择
{
	u8  xdata line = 0;//为选中的行数，初始为0
	Print_Map();								//打印地图
	OLED_ShowCHinese(2,2,17);//贪
	OLED_ShowCHinese(24,2,18);//吃
	OLED_ShowCHinese(46,2,19);//蛇
	OLED_ShowCHinese(87,0,0);//开
	OLED_ShowCHinese(103,0,1);//始
	OLED_ShowCHinese(87,2,2);//简
	OLED_ShowCHinese(103,2,3);//单
	OLED_ShowCHinese(87,4,6);//有
	OLED_ShowCHinese(103,4,8);//墙
	OLED_ShowCHinese(67,0,15);//打印开始位置的->
	while(1)
	{
		u8  key = Scan_Keyboard(0);
		//Delay(200);
		if(key == 1)	//如果按上
		{
				OLED_ShowCHinese(67,line,16);//清除之前位置的->
				if(line == 0)
				{
					line = 4;	//到最下面一行			
				}
				else
				{
					line -= 2;	//向上移动一行				
				}
				OLED_ShowCHinese(67,line,15);//打印之后位置的->		
		}
		else if(key == 3)//如果按下
		{
				OLED_ShowCHinese(67,line,16);//清除之前位置的->
				if(line == 4)
				{
					line = 0;	//到第一行
				}
				else
				{
					line += 2;	//向下移动一行				
				}
				OLED_ShowCHinese(67,line,15);//打印之后位置的->		
		}
		else if(key == 5)	//如果确定
		{
			if(line == 0)
			{
				break;//退出循环，游戏开始
			}
			else if(line == 2)//选择难度
			{
				if(mode == 5)	//现在是简单模式
				{
					mode = 2;		//改成困难模式
					OLED_ShowCHinese(87,2,4);//困
					OLED_ShowCHinese(103,2,5);//难
				}
				else if(mode == 2)//如果现在是困难模式
				{
					mode = 5;		//改成简单模式
					OLED_ShowCHinese(87,2,2);//简
					OLED_ShowCHinese(103,2,3);//单
				}
			}
			else if(line == 4)//选中了墙
			{
				if(wall == 1)	//如果现在是有墙模式
				{
						wall = 0;
						OLED_ShowCHinese(87,4,7);//无
				}
				else if(wall == 0)//如果现在是无墙模式
				{
						wall = 1;
						OLED_ShowCHinese(87,4,6);//有
				}
			}
		}
	}
}
/*************************************************************/
void Print_OverMenu() 		//打印结束菜单
{
	OLED_ShowCHinese(15,2,11);//游
	OLED_ShowCHinese(32,2,12);//戏
	OLED_ShowCHinese(15,4,13);//结
	OLED_ShowCHinese(32,4,14);//束
}
/*************************************************************/
void Print_window() 			//打印分数和难度窗口
{
	LCD_P8x16Str(64,0,"       ");
	LCD_P8x16Str(64,1,"       ");
	LCD_P8x16Str(87,2,"    ");
	LCD_P8x16Str(103,3,"    ");
	LCD_P8x16Str(87,4,"    ");
	LCD_P8x16Str(103,5,"    ");
	OLED_ShowCHinese(64,2,9);//得
	OLED_ShowCHinese(81,2,10);//分
	if(mode==2)
	{
		OLED_ShowCHinese(64,4,4);//困
		OLED_ShowCHinese(81,4,5);//难
	}
	else if(mode==5)
	{
		OLED_ShowCHinese(64,4,2);//简
		OLED_ShowCHinese(81,4,3);//单
	}
	LCD_P8x16Str(103,2,"0  ");
}
/*************************************************************/
void Print_Score(unsigned char score)				//打印分数
{
	unsigned char xdata ch[3]={0,0,'\0'};
	ch[0]=score/10%10+'0';
	ch[1]=score%10+'0';
	LCD_P8x16Str(103,2,ch);
}
/*************************************************************/
void Print_Snake() 				//打印蛇的坐标
{
	u8 xdata i;
	for(i = 0 ; i <= length ;i++)
	{
		OLED_Write_GRAM(snake[i].x,snake[i].y,1);
	}
} 
/*************************************************************/
void Print_Food() 				//打印食物  
{
	OLED_Write_GRAM(food.x,food.y,1);
} 
/*************************************************************/
void Print_Clear() 				//清屏函数
{
	u8 xdata i,j;
	for(i = 1; i < 63 ;i++)
		for(j = 1 ; j < 63 ;j++)
			OLED_Write_GRAM(i,j,0);
} 
/*************************************************************/
void Food_Init() 					//随机生成食物 清除原先食物的坐标 写入新的坐标 
{
	//食物生成的算法是利用定时器生成两个62范围内的数，如果与蛇重合重新生成 
	while(1)
	{
		u8 xdata i;
		food.x = TL1 % 61+2;//加1是防止和地图边缘重合
		food.y = TL0 % 61+2;
		for(i = 0; i < length ; i++)
		{
			if(food.x == snake[i].x && food.y == snake[i].y)
				continue;
		}
		break;
	}
} 
/*************************************************************/
void Food_Is_Eaten() 			//判断食物是否被吃了 
{
	if(snake[0].x == food.x && snake[0].y == food.y)
	{
		length++;
		snake[length].x = tail.x;
		snake[length].y = tail.y;
		Food_Init();
		Print_Score(length);	//更新一次分数
	}	
}
/*************************************************************/








void StartSnack()
{
	LCD_CLS();
	Print_StartMenu();			//打印开始菜单 
	Print_Map();						//打印地图
	Print_window();					//打印分数和难度窗口 
	Snake_Init();						//初始化蛇 
	Food_Init();						//初始化食物	
}

void While1Snack()
{
	while(1)
	{
			tempkey =  Scan_Keyboard(1);	//录入输入的方向
			if(wall)//判断蛇是否死亡
			{
				//如果有墙模式下
				if(Snake_HitWall()||Snake_eatself())//如果撞墙或吃到自己
					break;
			}
			else
			{
				//无墙模式下
				if(Snake_eatself())//如果吃到自己
					break;
			}
			if(length == MAX - 1)	//如果等于最大身体长度
				break;							//结束游戏
	}
	length=1;
	Print_OverMenu();				//打印结束菜单
}

void SnackInterrupt()
{
	time_count++;			//计时 
	if(time_count >= mode)
	{
		if(key == 1)//向上运动
		{
			if(tempkey == 2 || tempkey == 4)
			{
				key = tempkey;	
			} 
		}
		if(key == 2)//向左运动
		{
			if(tempkey == 1 || tempkey == 3)
			{
				key = tempkey;	
			} 
		}
		if(key == 3)//向下运动
		{
			if(tempkey == 2 || tempkey == 4)
			{
				key = tempkey;	
			} 
		}
		if(key == 4)//向右运动
		{
			if(tempkey == 1 || tempkey == 3)
			{
				key = tempkey;	
			} 
		}
		switch(key)
		{
			case 1:			//向上移动 
				Snake_Move(); 
				snake[0].y--;
				break;
			case 2:			//向左移动 
				Snake_Move(); 
				snake[0].x--;
				break;
			case 3:			//向下移动 
				Snake_Move(); 
				snake[0].y++;
				break;	
			case 4:			//向右移动 
				Snake_Move(); 
				snake[0].x++;
				break;	
		} 
		Food_Is_Eaten(); 		//判断食物是否被吃了 
		Print_Clear();			//清屏函数
		Print_Snake();			//打印蛇的坐标	
		Print_Food();				//打印食物
		OLED_Refresh();			//把显存中的值赋给OLED 
		time_count = 0;			//重新计时 
	}


}