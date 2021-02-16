#ifndef _SNACK_H
#define _SNACK_H
#include "stc12c5a60s2.h"
#include "oled.h"
#include "LQ12864.h"
#define u8 unsigned char
#define u16 unsigned int
#define MAX 100		//�ߵ���󳤶� 
/*************************************************************/
/*
key1=P3^5;		//1�� ��
key2=P3^4;		//2�� ��
key3=P3^6;		//3�� ��
key4=P3^7;		//4�� ��
key5=P3^3;		//5�� ȷ��/��ͣ
*/
/*************************************************************/

typedef struct Snake//�߽ṹ�壬���ڴ����ߵ������ʳ�������
{
	u8 x;			//xΪ������
	u8 y;			//yΪ������
}Snake;


//������ֲ
void StartSnack();
void While1Snack();
void SnackInterrupt();
#endif