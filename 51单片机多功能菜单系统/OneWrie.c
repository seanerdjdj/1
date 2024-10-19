#include <REGX52.H>

sbit OneWire_DQ=P3^7;
/**
	*@brief  OneWire初始化
	*@param  无
	*@retval 返回DQ总线的值
*/
unsigned char OneWire_Init()
{
	unsigned char i;
	unsigned char AckBit;
	EA=0;                      //关闭定时器全部中断
	OneWire_DQ=1;              //先把总线拉高
	OneWire_DQ=0;	           //再把总线拉低
	i = 247;while (--i);       //Delay500us
	OneWire_DQ=1;              //释放总线
	i = 32;while (--i);        //Delay70us
	AckBit=OneWire_DQ;
	i = 247;while (--i);       //Delay500us
	EA=1;                      //打开定时器中断
	return AckBit; 
}
/**
	*@brief  OneWire发送一位
	*@param  一位
	*@retval 无
*/
void OneWire_SendBit(unsigned char Bit)
{
	unsigned char i;
	EA=0;                    //关闭定时器全部中断
	OneWire_DQ=0;           //把总线拉低
	i = 4;while (--i);      //Delay10us
	OneWire_DQ=Bit;         //发送一位数据
	i = 24;while (--i);     //Delay50us
	OneWire_DQ=1;           //释放总线
	EA=1;                   //打开定时器中断
}
/**
	*@brief  OneWire接收一位
	*@param  无
	*@retval 返回一位
*/
unsigned char ReceiveBit()
{
	unsigned char i;
	unsigned char AckBit;
	EA=0;                  //关闭定时器全部中断
	OneWire_DQ=0;          //把总线拉低
	i = 2;while (--i);     //Delay5us
	OneWire_DQ=1;          //释放总线需要时间，所以释放完刚好到15us未尾
	i = 2;while (--i);     //Delay5us
	AckBit=OneWire_DQ;
	i = 24;while (--i);    //Delay50us
	EA=1;                  //打开定时器中断
	return AckBit;
	
}
/**
	*@brief  OneWire发送一个字节
	*@param  一个字节
	*@retval 无
*/
void OneWire_SendByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		OneWire_SendBit(Byte&(0x01<<i));//逐位进行发送
	}
}
/**
	*@brief  OneWire接收一个字节
	*@param  无
	*@retval 返回一个字节
*/
unsigned char OneWire_ReceiveByte()
{
	unsigned char i;
	unsigned char Byte=0x00;//初始化为0000 0000
	for(i=0;i<8;i++)
	{
		if(ReceiveBit())
		{
			Byte=Byte|(0x01<<i);//有1，则置1
		}
	}
	return Byte;
}