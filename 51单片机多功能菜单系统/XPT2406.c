#include <REGX52.H>

sbit XPT2406_DIN=P3^4;
sbit XPT2406_CS=P3^5;
sbit XPT2406_DCLK=P3^6;
sbit XPT2406_DOUT=P3^7;

unsigned int XPT2406_ReadAD(unsigned int Command)
{
	unsigned int Vaule=0;
	unsigned char i;
	XPT2406_CS=1;
	XPT2406_DCLK=0;
	XPT2406_CS=0;
	
	for(i=0;i<8;i++)
	{
		XPT2406_DIN=Command&(0x80>>i);
		XPT2406_DCLK=1;
		XPT2406_DCLK=0;
	}
	
	XPT2406_DCLK=1;
	XPT2406_DCLK=0;
	for(i=0;i<16;i++)
	{
		XPT2406_DCLK=1;
		XPT2406_DCLK=0;
		if(XPT2406_DOUT)
		{
			Vaule=Vaule|(0x8000>>i);
		}
	}
	
	XPT2406_CS=1;
	return Vaule>>4;	
}