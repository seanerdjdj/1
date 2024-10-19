#include <REGX52.H>
#include "LCD1602.h"
#include "Timer0.h"
#include "Timer1.h"
#include "MatrixKey.h"
#include "Key.h"
#include "DS18B20.h"
#include "XPT2406.h"

sbit Motor=P1^0;
//通用参数（主状态标志位——选择菜单功能，识别矩阵键盘按键，识别独立键盘按键，次数）
unsigned char Main_Flag,Mar_KeyNum,Key_KeyNum,PWD_Count;
//开始输入密码参数
unsigned int PassWord;
//多功能菜单选择参数
unsigned char Fun_Temp;
//实时温度监测参数
float T;
//风扇控速参数
unsigned char Compare,Fan_Speed;
//实时监测光照参数
unsigned int Sun_GZ;
//计算机参数
unsigned int Num1,Num2,Cal_Temp,Result,Cal_Count,Start_Flag,Com_Flag,Com_TempFlag;
//BMI参数
unsigned int Weight,BMI_Temp,Hight;
unsigned char BMI_Count,BMI_Flag;

//锁屏函数，当在菜单选择界面，监测到K1按下时回到开始密码界面
void Exit0()
{
	Key_KeyNum=KeyNum();
	if(Key_KeyNum==1)
	{
		LCD_ShowString(1,1,"                ");//清屏
		LCD_ShowString(2,1,"                ");
		Main_Flag=0;//
	}
}
//退出函数，当功能页面，监测到K1按下时回到菜单选择界面
void Exit1()
{
	Key_KeyNum=KeyNum();
	if(Key_KeyNum==1)
	{
		LCD_ShowString(1,1,"                ");
		LCD_ShowString(2,1,"                ");
		
		Main_Flag=6;
	}
}
//开始输入密码
void Start_PassWord()
{
	LCD_ShowString(1,1,"Scanf PWD:");
	Mar_KeyNum=MatrixKey_Key();  //获取矩阵键盘键码值
	if(Mar_KeyNum)//当矩阵键盘有按下时进入
	{
		//输入密码共4位，每次只能输入一个数字(0-9)，矩阵键盘K1-K10
		if(PWD_Count<4&&Mar_KeyNum>=1&&Mar_KeyNum<=10)
		{
			PassWord=PassWord*10+Mar_KeyNum%10;
			PWD_Count++;
		}
		//按下矩阵键盘K11，输入的密码会清零
		else if(Mar_KeyNum==11)
		{
			PassWord=0;
			PWD_Count=0;
		}
		//按下矩阵键盘K12，确定密码(初始化：1234)
		else if(Mar_KeyNum==12)
		{
			//密码正确，进入菜单页面
			if(PassWord==1234)
			{
				PassWord=0;
				PWD_Count=0;
				Main_Flag=6;
				LCD_ShowString(1,1,"                ");
				LCD_ShowString(2,1,"                ");
			}
			else
			{
				//密码错误，重新输入
				PassWord=0;
				PWD_Count=0;
				LCD_ShowString(2,1,"PWD Mistack!");
			}
		}
	}
	LCD_ShowNum(1,11,PassWord,4);//实时显示输入的值
}

//多功能菜单选择
void Function_Select()
{
	LCD_ShowString(1,1,"Fan Temp Sun Cal");//显示功能选择
	LCD_ShowString(2,1,"BMI  Pick(1-5):");
	Exit0();//实时监测，是否要退出到锁屏页面
	Mar_KeyNum=MatrixKey_Key();
	if(Mar_KeyNum)
	{
		if(Mar_KeyNum>=1&&Mar_KeyNum<=5)//只有5个功能选项，所以只能是1-5
		{
			Fun_Temp=Mar_KeyNum;//临时选项值
			LCD_ShowNum(2,16,Fun_Temp,1);
		}
		//当按下K12确定时，把临时选项值赋给Main_Flag(主状态标志位),进入相应功能页面
		else if(Mar_KeyNum==12)
		{
			if(Fun_Temp)
			{
				Main_Flag=Fun_Temp;
				LCD_ShowString(1,1,"                ");
				LCD_ShowString(2,1,"                ");				
			}
		}
	}
}
//BMI的计算
void BMI()
{
	if(BMI_Flag==0)
	{
		LCD_ShowString(1,1,"BMI");
		LCD_ShowString(1,10,"H:");
		LCD_ShowString(2,10,"W:");
		LCD_ShowString(1,15,"CM");
		LCD_ShowString(2,15,"KG");
		Mar_KeyNum=MatrixKey_Key();
		Exit1();//实时监测，是否要退出到菜单选择页面
		if(Mar_KeyNum)
		{
			//输入数字最多3位，每次只能输入一个数字(0-9)，矩阵键盘K1-K10
			if(BMI_Count<3&&Mar_KeyNum>=1&&Mar_KeyNum<=10)
			{
				BMI_Temp=BMI_Temp*10+Mar_KeyNum%10;
				BMI_Count++;
			}
			else if(Mar_KeyNum==11)//清零
			{
				BMI_Temp=0;
				BMI_Count=0;
			}
			//确定按键，当矩阵键盘按下K12时
			else if(Mar_KeyNum==12)
			{
				//当身高为0，Temp值给身高
				if(Hight==0)
				{
					Hight=BMI_Temp;
					LCD_ShowNum(1,12,Hight,3);
					BMI_Temp=0;
					BMI_Count=0;
				}
				//当身高为0，Temp值给身高
				else if(Weight==0)
				{
					Weight=BMI_Temp;
					BMI_Flag=1;
					LCD_ShowString(1,1,"                ");
					LCD_ShowString(2,1,"                ");	
					LCD_ShowString(1,1,"BMI:");	
					LCD_ShowString(2,1,"Result:");
				}
			}
		}
		if(Hight==0)
		{
			LCD_ShowNum(1,12,BMI_Temp,3);
		}
		else if(Weight==0)
		{
			LCD_ShowNum(2,12,BMI_Temp,3);
		}
	}
	if(BMI_Flag)
	{	
		double H=(double)(Hight)/100;
		double Result=Weight/(H*H);
		Exit1();
		LCD_ShowNum(1,5,Result,2);
		LCD_ShowChar(1,7,'.');
		LCD_ShowNum(1,8,(int)(Result*100)%100,2);
		if(Result>18.5&&Result<24.9)
		{
			LCD_ShowString(2,8,"Normal");
		}
		else if(Result<18.5)
		{
			LCD_ShowString(2,8,"Thin!");
		}
		else
		{
			LCD_ShowString(2,8,"Chubby!!");
		}
		Mar_KeyNum=MatrixKey_Key();
		if(Mar_KeyNum==11)
		{
			LCD_ShowString(1,1,"                ");
			LCD_ShowString(2,1,"                ");	
			Hight=0;
			Weight=0;
			BMI_Temp=0;
			BMI_Count=0;
			BMI_Flag=0;
		}
	}
}
//计算器
void Cal()
{
	Exit1();
	LCD_ShowString(1,1,"NUM1 NUM2 Result");
	Mar_KeyNum=MatrixKey_Key();
	if(Mar_KeyNum)
	{
		if(Cal_Count<3&&Mar_KeyNum>=1&&Mar_KeyNum<=10)
		{
			Cal_Temp=Cal_Temp*10+Mar_KeyNum%10;
			Start_Flag=1;
			Cal_Count++;
		}
		else if(Mar_KeyNum==11)
		{
			Cal_Temp=0;
			Cal_Count=0;
			if(Com_Flag&&Num1&&Num2)
			{
				LCD_ShowString(2,1,"                ");	
				Num1=0,Num2=0,Cal_Temp=0,Cal_Count=0,Start_Flag=0,Com_TempFlag=0,Com_Flag=0;
			}
		}
		else if(Mar_KeyNum==12)
		{
			if(Num1==0)
			{
				Num1=Cal_Temp;
				LCD_ShowNum(2,1,Num1,3);				
				Start_Flag=0;
			}
			else if(Num1&&Com_Flag==0)
			{
				Com_Flag=Com_TempFlag;
				Cal_Temp=0,Cal_Count=0;
			}
			else if(Com_Flag&&Num2==0)
			{
				Num2=Cal_Temp;
				LCD_ShowNum(2,7,Num2,3);
			}
			if(Com_Flag&&Num1&&Num2)
			{
				LCD_ShowChar(2,10,'=');
				if(Com_Flag==13){LCD_ShowNum(2,11,(Num1+Num2),4);}
				if(Com_Flag==14){LCD_ShowSignedNum(2,11,(Num1-Num2),3);}
				if(Com_Flag==15){LCD_ShowNum(2,11,(Num1*Num2),6);}
				if(Com_Flag==16){LCD_ShowNum(2,11,(Num1/Num2),3);}
			}
		}
		else if(Num1&&Mar_KeyNum>=13&&Mar_KeyNum<=16)
		{
			if(Mar_KeyNum==13){Com_TempFlag=13;LCD_ShowChar(2,5,'+');}
			if(Mar_KeyNum==14){Com_TempFlag=14;LCD_ShowChar(2,5,'-');}
			if(Mar_KeyNum==15){Com_TempFlag=15;LCD_ShowChar(2,5,'*');}//由于51单片机乘法运算最高位16位处理，所以如果计算结果超过65536将会从1开始算起
			if(Mar_KeyNum==16){Com_TempFlag=16;LCD_ShowChar(2,5,'/');}
		}
	}
	if(Start_Flag&&Num1==0)
	{
		LCD_ShowNum(2,1,Cal_Temp,3);
	}
	if(Com_Flag&&Num2==0&&Start_Flag)
	{
		LCD_ShowNum(2,7,Cal_Temp,3);
	}
}
//实时监测光照，判断白天黑夜
void Sun()
{
	LCD_ShowString(1,1,"RG:");
	Sun_GZ=XPT2406_ReadAD(0XA4);
	LCD_ShowNum(1,4,Sun_GZ,3);
	while(Main_Flag==3&&Sun_GZ<=8)
	{
		Exit1();
		Sun_GZ=XPT2406_ReadAD(0XA4);
		LCD_ShowNum(1,4,Sun_GZ,3);
		LCD_ShowString(2,1,"now is night!");
	}
	LCD_ShowString(2,1,"                ");	
	while(Main_Flag==3&&Sun_GZ>8)
	{
		Exit1();
		Sun_GZ=XPT2406_ReadAD(0XA4);
		LCD_ShowNum(1,4,Sun_GZ,3);
		LCD_ShowString(2,1,"now is day!");
	}
}
//实时监测温度
void Temp()
{
	LCD_ShowString(1,1,"T:");
	while(Main_Flag==2)
	{
		Exit1();
		DS18B20_ConverT();
		T=DS18B20_ReadT();
		if(T<0)
		{
			LCD_ShowChar(1,3,'-');
			T=-T;
		}
		else
		{
			LCD_ShowChar(1,3,'+');
		}
		LCD_ShowNum(1,4,T,3);
		LCD_ShowChar(1,7,'.');
		LCD_ShowNum(1,8,(unsigned long)(T*100)%100,2);
	}
}
//调速风扇
void Fan()
{
	Timer1_Init();
	Exit1();
	Mar_KeyNum=MatrixKey_Key();
	if(Mar_KeyNum==10||(Mar_KeyNum>=1&&Mar_KeyNum<=3))
	{
		if(Mar_KeyNum==10){Fan_Speed=0,Compare=0;}
		if(Mar_KeyNum==1){Fan_Speed=1,Compare=50;}
		if(Mar_KeyNum==2){Fan_Speed=2,Compare=75;}
		if(Mar_KeyNum==3){Fan_Speed=3,Compare=100;}
	}
	LCD_ShowString(1,1,"Fan Speed:");
	LCD_ShowNum(1,11,Fan_Speed,1);
}


void main()
{
	LCD_Init();
	Timer0_Init();
	
	while(1)
	{
		if(Main_Flag==0)
		{
			Start_PassWord();
		}
		else if(Main_Flag==1)
		{
			Fan();
		}
		else if(Main_Flag==2)
		{
			Temp();
		}
		else if(Main_Flag==3)
		{
			Sun();
		}
		else if(Main_Flag==4)
		{
			Cal();
		}
		else if(Main_Flag==5)
		{
			BMI();
		}	
		else if(Main_Flag==6)
		{
			Fan_Speed=0,Compare=0,ET1 = 0;//当从风扇功能出来时，风速调为0，关闭定时器1
			Num1=0,Num2=0,Cal_Temp=0,Cal_Count=0,Start_Flag=0,Com_Flag=0;//当从计算机功能出来时，参数初始化为0
			Weight=0,BMI_Temp=0,Hight=0,BMI_Count=0,BMI_Flag=0;
			Function_Select();
		}
	}
}

void Timer0_Routine() interrupt 1
{
	static unsigned char T0Count1;
	static unsigned char T0Count2;
	T0Count1++;
	T0Count2++;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	
	if(T0Count1>=20)
	{
		MartrixKey_Loop();
	}
	if(T0Count2>=60)
	{
		Key_Loop();
	}
	
}

void Timer1_Routine() interrupt 3  //中断函数
{
	static unsigned int T1Count;
	T1Count++;
	T1Count%=100;
	TL1 = 0x9C;				//设置定时初始值
	TH1 = 0xFF;				//设置定时初始值
	if(T1Count<Compare)     
	{
		Motor=1;           
	}
	else
	{
		Motor=0;
	}
}