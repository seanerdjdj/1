#include <REGX52.H>
unsigned char Mar_KeyNumer;

unsigned char MatrixKey_Key()
{
	unsigned char Temp=0;
	Temp=Mar_KeyNumer;
	Mar_KeyNumer=0;
	return Temp;
}

unsigned char Mar_GetKeyState()
{
	unsigned char KeyNumer=0;
	P1=0xff;
	P1_3=0;
	if(P1_7==0){KeyNumer=1;}
	if(P1_6==0){KeyNumer=5;}
	if(P1_5==0){KeyNumer=9;}
	if(P1_4==0){KeyNumer=13;}
	
	P1=0xff;
	P1_2=0;
	if(P1_7==0){KeyNumer=2;}
	if(P1_6==0){KeyNumer=6;}
	if(P1_5==0){KeyNumer=10;}
	if(P1_4==0){KeyNumer=14;}
	
	P1=0xff;
	P1_1=0;
	if(P1_7==0){KeyNumer=3;}
	if(P1_6==0){KeyNumer=7;}
	if(P1_5==0){KeyNumer=11;}
	if(P1_4==0){KeyNumer=15;}
	
	P1=0xff;
	P1_0=0;
	if(P1_7==0){KeyNumer=4;}
	if(P1_6==0){KeyNumer=8;}
	if(P1_5==0){KeyNumer=12;}
	if(P1_4==0){KeyNumer=16;}	
	
	return KeyNumer;
}

void MartrixKey_Loop()
{
	static unsigned char NowState,LastState;
	LastState=NowState;
	NowState=Mar_GetKeyState();
	if(LastState==1&&NowState==0){Mar_KeyNumer=1;}
	if(LastState==2&&NowState==0){Mar_KeyNumer=2;}
	if(LastState==3&&NowState==0){Mar_KeyNumer=3;}	
	if(LastState==4&&NowState==0){Mar_KeyNumer=4;}	
	if(LastState==5&&NowState==0){Mar_KeyNumer=5;}	
	if(LastState==6&&NowState==0){Mar_KeyNumer=6;}	
	if(LastState==7&&NowState==0){Mar_KeyNumer=7;}	
	if(LastState==8&&NowState==0){Mar_KeyNumer=8;}	
	if(LastState==9&&NowState==0){Mar_KeyNumer=9;}	
	if(LastState==10&&NowState==0){Mar_KeyNumer=10;}	
	if(LastState==11&&NowState==0){Mar_KeyNumer=11;}	
	if(LastState==12&&NowState==0){Mar_KeyNumer=12;}	
	if(LastState==13&&NowState==0){Mar_KeyNumer=13;}	
	if(LastState==14&&NowState==0){Mar_KeyNumer=14;}	
	if(LastState==15&&NowState==0){Mar_KeyNumer=15;}	
	if(LastState==16&&NowState==0){Mar_KeyNumer=16;}	
}

