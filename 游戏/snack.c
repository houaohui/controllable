#include "snack.h"
#include "yaogun.h"
#include "jianpan.h"
/*************************************************************/
Snake xdata snake[MAX],food,tail;				//������ṹ�壬ʳ�β�� 
u8 xdata tempkey,key = 4; 							//������̼���ֵ ͬʱ�������ж����߷���  ��ʼ�ƶ�����Ϊ�� 
u8 xdata length = 1;										//�����ߵĳ���
u8 xdata mode = 5;											//����ѡ����Ѷ� 20msˢ���� ����mode=2 50msˢ���� ��mode=5
bit wall = 1;											//�����Ƿ���ǽ 1Ϊ��ǽģʽ��0Ϊ��ǽģʽ
u8 xdata time_count = 0;								//ʱ������� 
extern unsigned char adc[4];
extern	unsigned char xdata keysta[10];
/*************************************************************/

void Snake_Init() 				//���ڳ�ʼ���� 
{
	snake[0].x=29;
	snake[0].y=29;
	snake[1].x=28;
	snake[1].y=29;
}
/*************************************************************/
void Snake_Move() 				//�ٿ����ƶ� ��һ�ڸ�ǰһ�ڸ�ֵ
{
	u8 xdata i;
	tail.x = snake[length].x;	//�����ϴ�β�͵����� 
	tail.y = snake[length].y;	//���Ե�ʳ���ʱ�򽫸����긳���߽ṹ��length��������
	for(i = length;i > 0 ; i--)
	{
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}
	if(wall == 0)
	{
		if(snake[0].x == 63)	//�������ǽ��ײ
		{
			snake[0].x = 1;			//�������
		}
		else if(snake[0].x == 0)//�������ǽ��ײ
		{
			snake[0].x = 62;			//�����Ҳ�
		}
		else if(snake[0].y == 0)//�������ǽ��ײ
		{
			snake[0].y = 62;			//�����²�
		}
		else if(snake[0].y == 63)//�������ǽ��ײ
		{
			snake[0].y = 0;			//�����ϲ�
		}
	}
}
/*************************************************************/
bit  Snake_eatself() 			//�ж��Ƿ�Ե��Լ�
{
	u8 xdata i;
	for(i = 3; i <= length ; i++)	//�߲����ܺ�ǰ������������˴ӵ��Ľڿ�ʼ���� 
	{
		if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			return 1;	
	}
	return 0;
}
/*************************************************************/
bit  Snake_HitWall() 			//�ж����Ƿ�ײǽ 
{
	if(snake[0].x == 0 || snake[0].x == 63 || snake[0].y == 0 || snake[0].y == 63)
		return 1;
	else
		return 0; 
}
/*************************************************************/
/*************************************************************/
void Print_Map() 					//��ӡ��ͼ
{
	u8 xdata i,j;
	for(i = 0; i<64 ; i++)
		for(j = 0; j<64; j++)
			if(i==0||j==0||i==63||j==63)
				OLED_Write_GRAM(i,j,1);
		OLED_Refresh();			//���Դ��е�ֵ����OLED 
}

u8 Scan_Keyboard(bit n)				//ɨ�����ֵ�����ؼ��������ֵ
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
		if(adc[1]<85)				//��ⰴ��key1�Ƿ���
		{	
				return 1;
		}
		else if(adc[0]<85)		//��ⰴ��key2�Ƿ���
		{	
				return 2;
		}
		else if(adc[1]>170)		//��ⰴ��key3�Ƿ���
		{
				return 3;
		}
		else if(adc[0]>170)		//��ⰴ��key4�Ƿ���
		{	
				return 4;
		}
		else if(adc[2]>170)		//��ⰴ��key5�Ƿ���
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



void Print_StartMenu() 		//��ӡ��ʼ�˵� д���ѶȺ�ģʽ ����һ��ѭ������ѡ��
{
	u8  xdata line = 0;//Ϊѡ�е���������ʼΪ0
	Print_Map();								//��ӡ��ͼ
	OLED_ShowCHinese(2,2,17);//̰
	OLED_ShowCHinese(24,2,18);//��
	OLED_ShowCHinese(46,2,19);//��
	OLED_ShowCHinese(87,0,0);//��
	OLED_ShowCHinese(103,0,1);//ʼ
	OLED_ShowCHinese(87,2,2);//��
	OLED_ShowCHinese(103,2,3);//��
	OLED_ShowCHinese(87,4,6);//��
	OLED_ShowCHinese(103,4,8);//ǽ
	OLED_ShowCHinese(67,0,15);//��ӡ��ʼλ�õ�->
	while(1)
	{
		u8  key = Scan_Keyboard(0);
		//Delay(200);
		if(key == 1)	//�������
		{
				OLED_ShowCHinese(67,line,16);//���֮ǰλ�õ�->
				if(line == 0)
				{
					line = 4;	//��������һ��			
				}
				else
				{
					line -= 2;	//�����ƶ�һ��				
				}
				OLED_ShowCHinese(67,line,15);//��ӡ֮��λ�õ�->		
		}
		else if(key == 3)//�������
		{
				OLED_ShowCHinese(67,line,16);//���֮ǰλ�õ�->
				if(line == 4)
				{
					line = 0;	//����һ��
				}
				else
				{
					line += 2;	//�����ƶ�һ��				
				}
				OLED_ShowCHinese(67,line,15);//��ӡ֮��λ�õ�->		
		}
		else if(key == 5)	//���ȷ��
		{
			if(line == 0)
			{
				break;//�˳�ѭ������Ϸ��ʼ
			}
			else if(line == 2)//ѡ���Ѷ�
			{
				if(mode == 5)	//�����Ǽ�ģʽ
				{
					mode = 2;		//�ĳ�����ģʽ
					OLED_ShowCHinese(87,2,4);//��
					OLED_ShowCHinese(103,2,5);//��
				}
				else if(mode == 2)//�������������ģʽ
				{
					mode = 5;		//�ĳɼ�ģʽ
					OLED_ShowCHinese(87,2,2);//��
					OLED_ShowCHinese(103,2,3);//��
				}
			}
			else if(line == 4)//ѡ����ǽ
			{
				if(wall == 1)	//�����������ǽģʽ
				{
						wall = 0;
						OLED_ShowCHinese(87,4,7);//��
				}
				else if(wall == 0)//�����������ǽģʽ
				{
						wall = 1;
						OLED_ShowCHinese(87,4,6);//��
				}
			}
		}
	}
}
/*************************************************************/
void Print_OverMenu() 		//��ӡ�����˵�
{
	OLED_ShowCHinese(15,2,11);//��
	OLED_ShowCHinese(32,2,12);//Ϸ
	OLED_ShowCHinese(15,4,13);//��
	OLED_ShowCHinese(32,4,14);//��
}
/*************************************************************/
void Print_window() 			//��ӡ�������Ѷȴ���
{
	LCD_P8x16Str(64,0,"       ");
	LCD_P8x16Str(64,1,"       ");
	LCD_P8x16Str(87,2,"    ");
	LCD_P8x16Str(103,3,"    ");
	LCD_P8x16Str(87,4,"    ");
	LCD_P8x16Str(103,5,"    ");
	OLED_ShowCHinese(64,2,9);//��
	OLED_ShowCHinese(81,2,10);//��
	if(mode==2)
	{
		OLED_ShowCHinese(64,4,4);//��
		OLED_ShowCHinese(81,4,5);//��
	}
	else if(mode==5)
	{
		OLED_ShowCHinese(64,4,2);//��
		OLED_ShowCHinese(81,4,3);//��
	}
	LCD_P8x16Str(103,2,"0  ");
}
/*************************************************************/
void Print_Score(unsigned char score)				//��ӡ����
{
	unsigned char xdata ch[3]={0,0,'\0'};
	ch[0]=score/10%10+'0';
	ch[1]=score%10+'0';
	LCD_P8x16Str(103,2,ch);
}
/*************************************************************/
void Print_Snake() 				//��ӡ�ߵ�����
{
	u8 xdata i;
	for(i = 0 ; i <= length ;i++)
	{
		OLED_Write_GRAM(snake[i].x,snake[i].y,1);
	}
} 
/*************************************************************/
void Print_Food() 				//��ӡʳ��  
{
	OLED_Write_GRAM(food.x,food.y,1);
} 
/*************************************************************/
void Print_Clear() 				//��������
{
	u8 xdata i,j;
	for(i = 1; i < 63 ;i++)
		for(j = 1 ; j < 63 ;j++)
			OLED_Write_GRAM(i,j,0);
} 
/*************************************************************/
void Food_Init() 					//�������ʳ�� ���ԭ��ʳ������� д���µ����� 
{
	//ʳ�����ɵ��㷨�����ö�ʱ����������62��Χ�ڵ�������������غ��������� 
	while(1)
	{
		u8 xdata i;
		food.x = TL1 % 61+2;//��1�Ƿ�ֹ�͵�ͼ��Ե�غ�
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
void Food_Is_Eaten() 			//�ж�ʳ���Ƿ񱻳��� 
{
	if(snake[0].x == food.x && snake[0].y == food.y)
	{
		length++;
		snake[length].x = tail.x;
		snake[length].y = tail.y;
		Food_Init();
		Print_Score(length);	//����һ�η���
	}	
}
/*************************************************************/








void StartSnack()
{
	LCD_CLS();
	Print_StartMenu();			//��ӡ��ʼ�˵� 
	Print_Map();						//��ӡ��ͼ
	Print_window();					//��ӡ�������Ѷȴ��� 
	Snake_Init();						//��ʼ���� 
	Food_Init();						//��ʼ��ʳ��	
}

void While1Snack()
{
	while(1)
	{
			tempkey =  Scan_Keyboard(1);	//¼������ķ���
			if(wall)//�ж����Ƿ�����
			{
				//�����ǽģʽ��
				if(Snake_HitWall()||Snake_eatself())//���ײǽ��Ե��Լ�
					break;
			}
			else
			{
				//��ǽģʽ��
				if(Snake_eatself())//����Ե��Լ�
					break;
			}
			if(length == MAX - 1)	//�������������峤��
				break;							//������Ϸ
	}
	length=1;
	Print_OverMenu();				//��ӡ�����˵�
}

void SnackInterrupt()
{
	time_count++;			//��ʱ 
	if(time_count >= mode)
	{
		if(key == 1)//�����˶�
		{
			if(tempkey == 2 || tempkey == 4)
			{
				key = tempkey;	
			} 
		}
		if(key == 2)//�����˶�
		{
			if(tempkey == 1 || tempkey == 3)
			{
				key = tempkey;	
			} 
		}
		if(key == 3)//�����˶�
		{
			if(tempkey == 2 || tempkey == 4)
			{
				key = tempkey;	
			} 
		}
		if(key == 4)//�����˶�
		{
			if(tempkey == 1 || tempkey == 3)
			{
				key = tempkey;	
			} 
		}
		switch(key)
		{
			case 1:			//�����ƶ� 
				Snake_Move(); 
				snake[0].y--;
				break;
			case 2:			//�����ƶ� 
				Snake_Move(); 
				snake[0].x--;
				break;
			case 3:			//�����ƶ� 
				Snake_Move(); 
				snake[0].y++;
				break;	
			case 4:			//�����ƶ� 
				Snake_Move(); 
				snake[0].x++;
				break;	
		} 
		Food_Is_Eaten(); 		//�ж�ʳ���Ƿ񱻳��� 
		Print_Clear();			//��������
		Print_Snake();			//��ӡ�ߵ�����	
		Print_Food();				//��ӡʳ��
		OLED_Refresh();			//���Դ��е�ֵ����OLED 
		time_count = 0;			//���¼�ʱ 
	}


}