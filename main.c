#include <stc12c5a60s2.h>
#include "LQ12864.h"
#include "yaogun.h"
#include "adc.h"
#include "display.h"
#include "jianpan.h"

sbit CS=P2^5;
unsigned char ui=0;
unsigned char adc[4];
unsigned char TIM[3]={8,25,0};
bit flag;
bit sendflag=0;
unsigned char sendmode=0;
void UartInit(void);
void Timer0Init(void);
void Sendchar(unsigned char dat);
void SendResult();
void SendCharReasult();//只能在主函数执行了
void Send_BlanceMode();//平衡车模式
//void delay(unsigned int n);
//void delayms(int ms);
unsigned char pageindex[4]={0,0,0,0};


void uiplay()
{
	switch(pageindex[0])
	{
		case 1:switch(pageindex[1])  //controlmode
						{
							case 1:switch(pageindex[2])        //on/off
										{
											case 1:break;
											case 2:break;
											case 0:OnOffControl();  break;
											
										}
										break; 
							case 2:switch(pageindex[2])       //displaymode
										{
											case 1:ImagePlay();break;    //imageplay
											case 2:NumPlay();break;                         //numberplay
											case 0:DisplayModeControl();break;                         //displaymodecontrol
										}
										break;
										
							case 3:switch(pageindex[2])             //sendmode
										{
											case 1:break;
											case 2:break;
											case 0:SendModeControl();break;
										}
										break;  
							case 0:ControlModeControl();break;
						}
						break;
			
		case 2:switch(pageindex[1])       //timemode
					{
						case 1:Timeplay();break;         //timeplay
						case 2:SetTimeControl();break;         //changetime
						case 0:TimeModeControl();break;         //timemodecontrol
					}
					break;
		case 3:switch(pageindex[1])      //gamemode
					{
						case 1:game1control();break;
						case 2:game2control();break;
						case 0:GameModeControl();break;
					}
					break;
		case 4:SetPasswordComtrol();break;     //setpassword
		case 5:AboutDeviceControl();  break;  //about device
		case 0:Page1Control();break;  //主页面control
	}
	
}



void main(void)
{
	CS=0;
	TR1 = 1;			//打开定时器1开关
	EA=1;
	LCD_Init();
	Init_ADC();
	UartInit();
	Timer0Init();
	while(1)
  {
		
		if(ui)
		{
			Send_BlanceMode();
			SendCharReasult();
			uiplay();
		//ShowTime();
		//DisplayYaoGunMode1();
		}
		else
		{
			passworddriver();
		}
		
	}
}


//10ms中断
void timer0() interrupt 1
{
	static unsigned char index=0;
	TL0 = 0x58;		//设置定时初值
	TH0 = 0x9E;		//设置定时初值
	index++;
	if(index>=100)
	{
		index=0;
		TIM[2]++;
	}
	
	if(TIM[2]>=60)
	{
		TIM[2]=0;
		TIM[1]++;
	}
	if(TIM[1]>=60)
	{
		TIM[1]=0;
		TIM[0]++;
	}
	if(TIM[0]>=24)
	{
		TIM[0]=0;
	}
if(pageindex[0]==3&&pageindex[1]==1)
{
	if(RunSnackFlag)
	SnackInterrupt();
}
	YaoGunScan();
	SendResult();
	flag=1;
}




void SendCharReasult()
{
	if(sendflag)
	{
		if(sendmode==1)
		{
			Sendchar(adc[0]/100%10+'0');
			Sendchar(adc[0]/10%10+'0');
			Sendchar(adc[0]%10+'0');
			Sendchar(' ');
			Sendchar(adc[1]/100%10+'0');
			Sendchar(adc[1]/10%10+'0');
			Sendchar(adc[1]%10+'0');
			Sendchar(' ');
			Sendchar(adc[2]/100%10+'0');
			Sendchar(adc[2]/10%10+'0');
			Sendchar(adc[2]%10+'0');
			Sendchar(' ');
			Sendchar(adc[3]/100%10+'0');
			Sendchar(adc[3]/10%10+'0');
			Sendchar(adc[3]%10+'0');
			Sendchar('\r');
			Sendchar('\n');
		}
	}
}

void SendResult()
{
	if(sendflag)
	{
		if(sendmode==0)
		{
			Sendchar('A');
			Sendchar(adc[0]);
			Sendchar(adc[1]);
			Sendchar(adc[2]);
			Sendchar(adc[3]);
			Sendchar('\r');
			Sendchar('\n');
		}
	}
}

void Send_BlanceMode()
{
	static unsigned int delay=0;
	if(sendflag)
	{
		if(sendmode==2)
		{
			delay++;
			if(delay>=4000)
			{
				delay=0;
				if(adc[0]<80)
				{
					Sendchar('3');
				}//////////////左
				else if(adc[0]>180)
				{
					Sendchar('4');
				}//////////////右
				else if(adc[3]<70)
				{
					Sendchar('2');
				}//////////////前
				else if(adc[3]>170)
				{
					Sendchar('1');
				}//////////////后
				else
				{
					Sendchar('0');//////////////刹车
				}
			}
		}
	}
}

//void delay(unsigned int n)
//{
//	while(--n);
//}


//void delayms(int ms)
//{
//   int ii,jj;
//   if (ms<1) ms=1;
//   for(ii=0;ii<ms;ii++)
//     for(jj=0;jj<500;jj++);  
//}

void Sendchar(unsigned char dat)
{
	SBUF=dat;
	while(TI==0);
	TI=0;
}


void Timer0Init(void)		//1毫秒@30.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x3C;		//设置定时初值
	TH0 = 0xF6;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}


void UartInit(void)		//9600bps@30.000MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT = 0x9E;		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
}


