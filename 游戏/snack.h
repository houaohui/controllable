#ifndef _SNACK_H
#define _SNACK_H
#include "stc12c5a60s2.h"
#include "oled.h"
#include "LQ12864.h"
#define u8 unsigned char
#define u16 unsigned int
#define MAX 100		//蛇的最大长度 
/*************************************************************/
/*
key1=P3^5;		//1键 上
key2=P3^4;		//2键 左
key3=P3^6;		//3键 下
key4=P3^7;		//4键 右
key5=P3^3;		//5键 确认/暂停
*/
/*************************************************************/

typedef struct Snake//蛇结构体，用于储存蛇的身体和食物的坐标
{
	u8 x;			//x为横坐标
	u8 y;			//y为纵坐标
}Snake;


//用于移植
void StartSnack();
void While1Snack();
void SnackInterrupt();
#endif