#include "jianpan.h"
#include "yaogun.h"
#include "LQ12864.h"
#include "display.h"
#include "snack.h"
#include "ball.h"
extern unsigned char ui;
extern unsigned char TIM[3];
extern unsigned char pageindex[4];
extern bit sendflag;
extern unsigned char sendmode;

unsigned char mima[5]="1234";
bit yi=1;
unsigned char ch;
unsigned char li=0;//控制所有page的选项箭头位置，用全局变量当切换page时可以方便初始化箭头到第一个位置
bit RunSnackFlag=1;

bit CmpMemory(unsigned char *ptr1, unsigned char *ptr2)//只用于密码比较
{
	while (*ptr2!='\0')
	{
		if (*ptr1++ != *ptr2++) //遇到不相等数据时即刻返回 0
		{
			return 0;
		}
	}
	return 1; //比较完全部长度数据都相等则返回 1 
}


//////////////////*密码驱动**/
void inputmima(unsigned char n)
{
	static unsigned char heng=0,num=100;
	static unsigned char mimabuf[5]="1234";
	unsigned char numchar[2]={0,'\0'};
	switch(n)
	{
		case 2:heng+=8; if(heng==32)heng=0; LCD_P6x8Str(heng,2,"-"); if(heng!=0)LCD_P6x8Str(heng-8,2," "); else LCD_P6x8Str(24,2," ");  break;
		case 3: num--; mimabuf[heng/8]=num%10+'0'; numchar[0]=num%10+'0';  LCD_P6x8Str(heng,1,numchar);  break;
		case 5:if(CmpMemory(mimabuf,mima)) { ui=1; LCD_CLS(); yi=1;} 
						else{ LCD_P6x8Str(54,0,"     "); LCD_P6x8Str(0,0,"Try again"); }break;
		case 8:heng-=8; if(heng>32)heng=24; LCD_P6x8Str(heng,2,"-"); if(heng!=24)LCD_P6x8Str(heng+8,2," "); else LCD_P6x8Str(0,2," ");  break;
		case 9: num++; mimabuf[heng/8]=num%10+'0'; numchar[0]=num%10+'0';  LCD_P6x8Str(heng,1,numchar);  break;
	}
	
}

void passworddriver()
{
	if(yi)
	{
		yi=0;
		//初始化输入密码界面
		LCD_P6x8Str(0,0,"input password");
		LCD_P6x8Str(0,1,"0");
		LCD_P6x8Str(8,1,"0");
		LCD_P6x8Str(16,1,"0");
		LCD_P6x8Str(24,1,"0");
		LCD_P6x8Str(0,2,"-");
	}
	
	for (ch=0; ch<10; ch++)
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					LCD_P6x8Str(0,0,"input password");//动一下按键显示
					switch(keycodemap[ch])
					{
						case 2:inputmima(2);break;
						case 3:inputmima(3);break;
						case 5:inputmima(5);break;
						case 8:inputmima(8);break;
						case 9:inputmima(9);break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
	}
}


///////////////*page1驱动*/

void Page1Switch(unsigned char n)
{
	switch(n)
	{
		case 3:li++;if(li==5)li=0;LCD_P6x8Str(115,li,"<");if(li!=0)LCD_P6x8Str(115,li-1," ");else LCD_P6x8Str(115,4," ");break;
		case 9:li--;if(li>5)li=4;LCD_P6x8Str(115,li,"<");if(li!=4)LCD_P6x8Str(115,li+1," ");else LCD_P6x8Str(115,0," ");break;
		case 4:ui=0;yi=1;li=0;LCD_CLS();break;   //返回
		case 5:pageindex[0]=li+1;LCD_CLS();yi=1;li=0;break;    //确认
		
	}
}

void Page1Control()
{
	if(yi)
	{
		LCD_P6x8Str(0,0,"control mode");
		LCD_P6x8Str(0,1,"time mode");
		LCD_P6x8Str(0,2,"game mode");
		LCD_P6x8Str(0,3,"set password");
		LCD_P6x8Str(0,4,"about device");
		LCD_P6x8Str(115,0,"<");
		//
		yi=0;
	}
	for (ch=0; ch<10; ch++)
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					switch(keycodemap[ch])
					{
						case 3:Page1Switch(3);break;
						case 9:Page1Switch(9);break;
						case 4:Page1Switch(4);break;
						case 5:Page1Switch(5);break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
	}
}


///////////////////////
void ControlModeSwitch(unsigned char n)
{
	switch(n)
	{
		case 3:li++;if(li==3)li=0;LCD_P6x8Str(115,li,"<");if(li!=0)LCD_P6x8Str(115,li-1," ");else LCD_P6x8Str(115,2," ");break;
		case 9:li--;if(li>3)li=2;LCD_P6x8Str(115,li,"<");if(li!=2)LCD_P6x8Str(115,li+1," ");else LCD_P6x8Str(115,0," ");break;
		case 4:pageindex[0]=0;  yi=1;li=0;  LCD_CLS();break;   //返回
		case 5:pageindex[1]=li+1;  yi=1;li=0;  LCD_CLS();break;    //确认
		
	}
}
void ControlModeControl()
{
	if(yi)
	{
		yi=0;
		LCD_P6x8Str(0,0,"on/off");
		LCD_P6x8Str(0,1,"display mode");
		LCD_P6x8Str(0,2,"send mode");
		LCD_P6x8Str(115,0,"<");
	}
	
	for (ch=0; ch<10; ch++)
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					switch(keycodemap[ch])
					{
						case 3:ControlModeSwitch(3);break;
						case 9:ControlModeSwitch(9);break;
						case 4:ControlModeSwitch(4);break;
						case 5:ControlModeSwitch(5);break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
	}
	
}

////////////////////
void OnOffSwitch(unsigned char n)
{
	switch(n)
	{
		case 3:li++;if(li==2)li=0;LCD_P6x8Str(115,li,"<");if(li!=0)LCD_P6x8Str(115,li-1," ");else LCD_P6x8Str(115,1," ");break;
		case 9:li--;if(li>2)li=1;LCD_P6x8Str(115,li,"<");if(li!=1)LCD_P6x8Str(115,li+1," ");else LCD_P6x8Str(115,0," ");break;
		case 4:pageindex[1]=0;  yi=1;li=0;  LCD_CLS();break;   //返回
		case 5:if(li==0)sendflag=1;else sendflag=0; pageindex[1]=0; yi=1; li=0; LCD_CLS();break;    //确认
		
	}
}

void OnOffControl()
{
	if(yi)
	{
		yi=0;
		LCD_P6x8Str(0,0,"on");
		LCD_P6x8Str(0,1,"off");
		LCD_P6x8Str(115,0,"<");
	}
	for (ch=0; ch<10; ch++)
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					switch(keycodemap[ch])
					{
						case 3:OnOffSwitch(3);break;
						case 9:OnOffSwitch(9);break;
						case 4:OnOffSwitch(4);break;
						case 5:OnOffSwitch(5);break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
	}
}

/////////////////
void DisplayModeSwitch(unsigned char n)
{
	switch(n)
	{
		case 3:li++;if(li==2)li=0;LCD_P6x8Str(115,li,"<");if(li!=0)LCD_P6x8Str(115,li-1," ");else LCD_P6x8Str(115,1," ");break;
		case 9:li--;if(li>2)li=1;LCD_P6x8Str(115,li,"<");if(li!=1)LCD_P6x8Str(115,li+1," ");else LCD_P6x8Str(115,0," ");break;
		case 4:pageindex[1]=0;  yi=1;li=0;  LCD_CLS();break;   //返回
		case 5:pageindex[2]=li+1;  yi=1;li=0;   LCD_CLS();break;    //确认
		
	}
	
	
}
void DisplayModeControl()
{
	if(yi)
	{
		yi=0;
		LCD_P6x8Str(0,0,"image play");
		LCD_P6x8Str(0,1,"num play");
		LCD_P6x8Str(115,0,"<");
	}
	
	for (ch=0; ch<10; ch++)
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					switch(keycodemap[ch])
					{
						case 3:DisplayModeSwitch(3);break;
						case 9:DisplayModeSwitch(9);break;
						case 4:DisplayModeSwitch(4);break;
						case 5:DisplayModeSwitch(5);break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
	}
}

void ImagePlay()
{
	DisplayYaoGunMode1();
	if (backupkey[4] != keysta[4])
	{
		if(backupkey[4]==1)
		{
			pageindex[2]=0;  yi=1;li=0;  LCD_CLS();
		}
		backupkey[4] = keysta[4];
	}
}

void NumPlay()
{
	DisplayYaoGunMode2();
	
	if (backupkey[4] != keysta[4])
	{
		if(backupkey[4]==1)
		{
			pageindex[2]=0;  yi=1;li=0;  LCD_CLS();
		}
		backupkey[4] = keysta[4];
	}
}


////////////////////

void SendModeSwitch(unsigned char n)
{
	
	switch(n)
	{
		case 3:li++;if(li==3)li=0;LCD_P6x8Str(115,li,"<");if(li!=0)LCD_P6x8Str(115,li-1," ");else LCD_P6x8Str(115,2," ");break;
		case 9:li--;if(li>3)li=2;LCD_P6x8Str(115,li,"<");if(li!=2)LCD_P6x8Str(115,li+1," ");else LCD_P6x8Str(115,0," ");break;
		case 4:pageindex[1]=0;  yi=1;li=0;  LCD_CLS();break;   //返回
		case 5:sendmode=li; pageindex[1]=0; yi=1; li=0; LCD_CLS();break;    //确认
	}
	
}
void SendModeControl()
{
	if(yi)
	{
		yi=0;
		LCD_P6x8Str(0,0,"send hex  fast");
		LCD_P6x8Str(0,1,"send char slow");
		LCD_P6x8Str(0,2,"balnce car mode");
		LCD_P6x8Str(115,0,"<");
	}
	
	for (ch=0; ch<10; ch++)
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					switch(keycodemap[ch])
					{
						case 3:SendModeSwitch(3);break;
						case 9:SendModeSwitch(9);break;
						case 4:SendModeSwitch(4);break;
						case 5:SendModeSwitch(5);break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
	}
}
///////////////////功能2时间

void TimeModeSwitch(unsigned char n)
{
	switch(n)
	{
		case 3:li++;if(li==2)li=0;LCD_P6x8Str(115,li,"<");if(li!=0)LCD_P6x8Str(115,li-1," ");else LCD_P6x8Str(115,1," ");break;
		case 9:li--;if(li>2)li=1;LCD_P6x8Str(115,li,"<");if(li!=1)LCD_P6x8Str(115,li+1," ");else LCD_P6x8Str(115,0," ");break;
		case 4:pageindex[0]=0;  yi=1;li=0;  LCD_CLS();break;   //返回
		case 5:pageindex[1]=li+1;  yi=1;li=0;  LCD_CLS();break;    //确认
	}
}

void TimeModeControl()
{
	if(yi)
	{
		yi=0;
		LCD_P6x8Str(0,0,"time play");
		LCD_P6x8Str(0,1,"set time");
		LCD_P6x8Str(115,0,"<");
	}
	for (ch=0; ch<10; ch++)
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					switch(keycodemap[ch])
					{
						case 3:TimeModeSwitch(3);break;
						case 9:TimeModeSwitch(9);break;
						case 4:TimeModeSwitch(4);break;
						case 5:TimeModeSwitch(5);break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
	}
}

///////////////////
void Timeplay()
{
	ShowTime();
	if (backupkey[4] != keysta[4])
	{
		if(backupkey[4]==1)
		{
			pageindex[1]=0;  yi=1;li=0;  LCD_CLS();
			
		}
		backupkey[4] = keysta[4];
	}
}

/////////////////

void SetTimeSwitch(unsigned char n)
{
	static unsigned char li=0,num=0;
	static unsigned char timebuf[3]={8,30,0};
	unsigned char numchar[3]={0,0,'\0'};
	switch(n)
	{
		case 3:li++;if(li==3)li=0;LCD_P6x8Str(115,li,"<");if(li!=0)LCD_P6x8Str(115,li-1," ");else LCD_P6x8Str(115,2," ");break;
		case 9:li--;if(li>3)li=2;LCD_P6x8Str(115,li,"<");if(li!=2)LCD_P6x8Str(115,li+1," ");else LCD_P6x8Str(115,0," ");break;
		
		case 2:num++; if(num==60)num=0;  timebuf[li]=num;   numchar[0]=num/10+'0'; numchar[1]=num%10+'0'; LCD_P6x8Str(64,li,numchar);break;
		case 8:num--; if(num>60)num=59;  timebuf[li]=num;   numchar[0]=num/10+'0'; numchar[1]=num%10+'0'; LCD_P6x8Str(64,li,numchar);break;
		case 4:pageindex[1]=0;  yi=1; li=0;  LCD_CLS();break;   //返回
		case 5:TIM[0]=timebuf[0];TIM[1]=timebuf[1];TIM[2]=timebuf[2]; LCD_P6x8Str(0,3,"ok");break;    //确认
	}
}

void SetTimeControl()
{
	if(yi)
	{
		yi=0;
		LCD_P6x8Str(0,0,"hour");
		LCD_P6x8Str(0,1,"min");
		LCD_P6x8Str(0,2,"sec");
		LCD_P6x8Str(115,0,"<");
		LCD_P6x8Str(0,6,"pass SW4 to updata");
	}
		for (ch=0; ch<10; ch++)
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					LCD_P6x8Str(0,3,"  ");
					switch(keycodemap[ch])
					{
						case 2:SetTimeSwitch(2);break;
						case 3:SetTimeSwitch(3);break;
						case 4:SetTimeSwitch(4);break;
						case 5:SetTimeSwitch(5);break;
						case 8:SetTimeSwitch(8);break;
						case 9:SetTimeSwitch(9);break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
	}
}

////////////////////功能3游戏

void GameModeSwitch(unsigned char n)
{
	switch(n)
	{
		case 3:li++;if(li==2)li=0;LCD_P6x8Str(115,li,"<");if(li!=0)LCD_P6x8Str(115,li-1," ");else LCD_P6x8Str(115,1," ");break;
		case 9:li--;if(li>2)li=1;LCD_P6x8Str(115,li,"<");if(li!=1)LCD_P6x8Str(115,li+1," ");else LCD_P6x8Str(115,0," ");break;
		case 4:pageindex[0]=0;  yi=1;li=0;  LCD_CLS();break;   //返回
		case 5:pageindex[1]=li+1;  yi=1;li=0;  LCD_CLS();break;    //确认
	}
}


void GameModeControl()
{
	if(yi)
	{
		yi=0;
		LCD_P6x8Str(0,0,"Snack");
		LCD_P6x8Str(0,1,"ball oh!");
		LCD_P6x8Str(115,0,"<");
	}
	for (ch=0; ch<10; ch++)
	{
			if (backupkey[ch] != keysta[ch])
			{
				if (backupkey[ch] == 1)
				{
					switch(keycodemap[ch])
					{
						case 3:GameModeSwitch(3);break;
						case 9:GameModeSwitch(9);break;
						case 4:GameModeSwitch(4);break;
						case 5:GameModeSwitch(5);break;
					}
				}
				backupkey[ch] = keysta[ch];
			}
	}
}


void AboutDeviceControl()
{
	if(yi)
	{
		yi=0;
		LCD_P6x8Str(0,0,"developer: HouAohui");
		LCD_P6x8Str(0,1,"version: 0.0.7");
		LCD_P6x8Str(0,2,"mcu model:");
		LCD_P6x8Str(0,3,"stc12c5a60s2");
		LCD_P6x8Str(0,4,"mcu frequency: 30Mhz");
		LCD_P6x8Str(0,5,"development date:");
		LCD_P6x8Str(0,6,"2020/10/2");
		LCD_P6x8Str(0,7,"oled version: 1306");
	}
	
		if (backupkey[4] != keysta[4])
	{
		if(backupkey[4]==1)
		{
			pageindex[0]=0;  yi=1;li=0;  LCD_CLS();
			
		}
		backupkey[4] = keysta[4];
	}
}

/////////////////
void SetPasswordComtrol()
{
	if(yi)
	{
		yi=0;
		LCD_P6x8Str(0,0,"undeveloped...");
		LCD_P6x8Str(0,1,"pass SW3 to return");
	}
	
		if (backupkey[4] != keysta[4])
	{
		if(backupkey[4]==1)
		{
			pageindex[0]=0;  yi=1;li=0;  LCD_CLS();
			
		}
		backupkey[4] = keysta[4];
	}
}


void game1control()
{
	if(RunSnackFlag)
	{
		RunSnackFlag=0;
		StartSnack();
		RunSnackFlag=1;
		While1Snack();
		RunSnackFlag=0;
	}
	
	if (backupkey[4] != keysta[4])
	{
		if(backupkey[4]==1)
		{
			pageindex[1]=0;  yi=1;li=0;  LCD_CLS();
			RunSnackFlag=1;
		}
		backupkey[4] = keysta[4];
	}
		
	
}


void game2control()
{
	if(yi)
	{
		yi=0;
		ball_test();
	}
	if (backupkey[4] != keysta[4])
	{
		if(backupkey[4]==1)
		{
			pageindex[1]=0;  yi=1;li=0;  LCD_CLS();
			
		}
		backupkey[4] = keysta[4];
	}
}