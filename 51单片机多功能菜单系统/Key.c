#include <REGX52.H>

unsigned char Key_KeyNumer;
/**
	*@brief     获取按键键码
	*@param     无
	*@retval    返回那个按键按下
*/
unsigned char KeyNum()
{
	unsigned char Temp=0;
	Temp=Key_KeyNumer;
	Key_KeyNumer=0;
	return Temp;
}
/**
    *@brief     当有按键按下时,为Key_Loop()函数更新当前状态
	*@param     无
	*@retval    返回按键码
*/
unsigned char Key_GetNumer()
{
	unsigned char KeyNumer=0;
	
	if(P3_1==0){KeyNumer=1;}
	if(P3_0==0){KeyNumer=2;}
	if(P3_2==0){KeyNumer=3;}
	if(P3_3==0){KeyNumer=4;}
	return KeyNumer;
}
/**
    *@brief     在定时器中断0中扫描按键状态
	*@param     无
	*@retval    无
*/
void Key_Loop()
{
	static unsigned char NowState,LastState;
	LastState=NowState;             //1毫秒前的状态         
	NowState=Key_GetNumer();        //当前状态
	//如果上个时间点按键按下，这个时间点未按下，则是松手瞬间，以此避免消抖和松手检测
	if(LastState==1&&NowState==0)
	{
		Key_KeyNumer=1;
	}
	if(LastState==2&&NowState==0)
	{
		Key_KeyNumer=2;
	}
	if(LastState==3&&NowState==0)
	{
		Key_KeyNumer=3;
	}
	if(LastState==4&&NowState==0)
	{
		Key_KeyNumer=4;
	}
}