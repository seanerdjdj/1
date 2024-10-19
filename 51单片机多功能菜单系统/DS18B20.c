#include <REGX52.H>
#include "OneWrie.h"

#define DS18B20_SKIP_ROM           0xcc //跳过ROM
#define DS18B20_CONVERT_T          0x44 //温度变换
#define DS18B20_READ_SCRATCHPAD    0xbe //读暂存器
/**
	*@brief  DS18B20温度转化(数字形式)
	*@param  无
	*@retval 无
*/
void DS18B20_ConverT()
{
	OneWire_Init();//初始化
	OneWire_SendByte(DS18B20_SKIP_ROM);//跳过Rom
	OneWire_SendByte(DS18B20_CONVERT_T);//温度转化(数字形式)
}
/**
	*@brief  DS18B20温度转化(数字形式)
	*@param  无
	*@retval 返回温度
*/
float DS18B20_ReadT()
{
	unsigned char TLSB,TMSB;//TSLB低8位 TMSB高8位
	int Temp;//16位
	float T;
	OneWire_Init();//初始化
	OneWire_SendByte(DS18B20_SKIP_ROM);//跳过Rom
	OneWire_SendByte(DS18B20_READ_SCRATCHPAD);//读暂存器
	TLSB=OneWire_ReceiveByte();//连续读操作
	TMSB=OneWire_ReceiveByte();
	Temp=(TMSB<<8)|TLSB;//转化成16位
	T=Temp/16.0;
	return T;
}
