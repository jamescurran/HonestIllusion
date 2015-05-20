// ReverveCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

unsigned char Reverse(int source);
int ReverseInt(unsigned int source);
void BuildTable();

unsigned char *xlat;

int _tmain(int argc, _TCHAR* argv[])
{
	BuildTable();
	printf("%x\n", ReverseInt(0xAA330011)); 

}


void BuildTable()
{
	xlat = new unsigned char[256];
	for(int i=0; i < 256; ++i)
	{
		xlat[i] = Reverse(i);
	}
}

unsigned char Reverse(int source)
{
	int shiftRight = 8;
	int tmpValue = source;
	int result = 0;
	while (shiftRight-- > 0)
	{
		tmpValue <<= 1;
		int currentBit = tmpValue & 0x0100;
		result = (result | currentBit) >>1;
	}
	return (unsigned char)result;
}

union intbyte {
unsigned int asInt;
unsigned char asBytes[4];
};

int ReverseInt(unsigned int source)
{
	intbyte a, b;
	a.asInt =source;
	b.asBytes[0] = xlat[a.asBytes[3]];
	b.asBytes[1] = xlat[a.asBytes[2]];
	b.asBytes[2] = xlat[a.asBytes[1]];
	b.asBytes[3] = xlat[a.asBytes[0]];
	return b.asInt;
}
