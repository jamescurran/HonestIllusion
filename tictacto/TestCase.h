//---------------------------------------------------------------------------
// TicTacToe - TestCase.cpp
//
//
// Copyright 1997, James M. Curran.  May be freely used.
//---------------------------------------------------------------------------
#if !defined(TESTCASE_H_INCLUDED)
#define TESTCASE_H_INCLUDED

class TestCase
{
private:
	int		X;
	int		Oh;
	int		point;
	int		pos;

public:
	TestCase() : X(0),Oh(0),point(99),pos(0) {};
	bool IsWin()	{return ( (Oh==2) && (point != 99) );}
	bool IsBlock()	{return ( ( X==2) && (point != 99) );}
	bool IsLose()   {return (   X==3);}
	int	 Point()	{return (point);}

	void Clear() { X=(0); Oh=(0); point=(99); pos=(0); };

	void Test(char ch)
	{		
			switch (ch)
			{
			     case BLANK:
					point = pos;
					break;

				 case XXXXX:
					X++;
					break;

				 case OOOOO:
					Oh++;
					break;
			}
			pos++;
	}
};

#endif //!defined(TESTCASE_H_INCLUDED)