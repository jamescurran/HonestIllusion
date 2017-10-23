//---------------------------------------------------------------------------
// TicTacToe - TTTGrid.h
//
//
// Copyright 1997, James M. Curran.  May be freely used.
//---------------------------------------------------------------------------
#if !defined(TTTGRID_H_INCLUDED)
#define TTTGRID_H_INCLUDED

class TTTGrid
{
private:
	string	Str;

public:
	TTTGrid(string	str): Str(str) {}
	char  operator() (int x, int y) { return (Str[x*3 + y +1]);}
	char &at(int x, int y)	{return (Str.at(x*3 +y +1));}
	char &at(int x)			{return (Str.at(x));}
	string AsString() {return(Str); }

	void MarkWin()  {Str[0] = 'W';}
	void MarkLose() {Str[0] = 'L';}
	void MarkTie()  {Str[0] = 'T';}

};

#endif //!defined(TTTGRID_H_INCLUDED)