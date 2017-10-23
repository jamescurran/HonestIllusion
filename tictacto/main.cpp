//---------------------------------------------------------------------------
// TicTacToe - Main.cpp
//
//
// Copyright 1994-1997, James M. Curran.  May be freely used.
//---------------------------------------------------------------------------
#include "stdafx.h"



#include "ttt.hpp"


	int	uniq=0;
	int	Win =0;
	int Lose=0;
	int Tie =0;



ResponseList	moves;
std::vector<TTTMove>	sheets;	

void TryAllMoves(TTTMove m)
{
	string	test = m.board;
	for (int i =0; i<9; i++)
	{
		string	answer;

		if (m.board[i+1] != BLANK)
			m.response[i] = 0;
		else
		{
			test = m.board;
			test[i+1] = XXXXX;
			Response resp = moves.GetResponse(test);
			m.response[i] = resp.Id;

// process result
			if (resp.count == 1)
			{
				cout << "Before:" << test << " after:"<< resp.CmprMove ;
				switch (resp.result)
				{
				case cont:
					break;

				case win:
					Win++;
					break;

				case lose:
					Lose++;
					break;

				case tie:
					Tie++;
					break;
				}

				uniq++;
				cout << "--Move #" << resp.Id << " Wins:" << Win << " Loses:" << Lose << " Ties:" << Tie << endl;
				if (resp.result == cont)
					TryAllMoves(resp.CmprMove);
			}
			else
				cout << "Move:" <<  test << " Seen " << resp.count -1 << " times before as Move #" << resp.Id << endl;
// end process result

		}
	}
	sheets.push_back(m);
}

string GetHREFfromID(int ID, char ch)
{
	if (ID == 0)
	{
		return(string("<img src=\"") + ch + ".gif\" border=\"0\">");
	}

	Response resp = moves.GetResponse(ID);

	return("<a href=\""+resp.PlayerMove.substr(1)+".htm\"><img src=\"b.gif\" border=\"0\"></a>");
}

string GenerateIntermPage(TTTMove m)
{
	string html;
	string line;
	string LF = "\n";

	line = GetHREFfromID(m.response[0], m.board[1]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(m.response[1], m.board[2]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(m.response[2], m.board[3]);
	html+= line + LF;

	html+= "<br><img src=\"h.gif\"><br>" +LF;

	line = GetHREFfromID(m.response[3], m.board[4]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(m.response[4], m.board[5]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(m.response[5], m.board[6]);
	html+= line + LF;

	html+= "<br><img src=\"h.gif\"><br>" +LF;

	line = GetHREFfromID(m.response[6], m.board[7]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(m.response[7], m.board[8]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(m.response[8], m.board[9]);
	html+= line + LF;

	return(html);
}


string GenerateFinalPage(Response R)
{
	string html;
	string line;
	string LF = "\n";



	line = GetHREFfromID(0, R.CmprMove[1]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(0, R.CmprMove[2]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(0, R.CmprMove[3]);
	html+= line + LF;

	html+= "<br><img src=\"h.gif\"><br>" +LF;

	line = GetHREFfromID(0, R.CmprMove[4]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(0, R.CmprMove[5]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(0, R.CmprMove[6]);
	html+= line + LF;

	html+= "<br><img src=\"h.gif\"><br>" +LF;

	line = GetHREFfromID(0, R.CmprMove[7]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(0, R.CmprMove[8]);
	html+= line + LF;

	html+= "<img src=\"v.gif\">" + LF;

	line = GetHREFfromID(0, R.CmprMove[9]);
	html+= line + LF;

	switch(R.result)
	{
		case cont:
		case unknown:
			break;

		case win:
			html+="<BR><P FONT=4><B> I WIN !</B>" +LF;
			break;

		case lose:
			html+="<BR><P FONT=4><B> You win!</B>" +LF;
			break;

		case tie:
			html+="<BR><P FONT=4><B>It's a Tie!</B>" +LF;
			break;
	}

	html+="&nbsp;&nbsp;<A HREF=\"bbbbbbbbb.htm\">Play Again?</A>" +LF;
	html+="&nbsp;&nbsp;<A HREF=\"default.htm\" TARGET=\"_top\">Home</A>" +LF;

	return(html);
}

void GenerateHTML()
{
		std::list<Response>::iterator	itorR= moves.begin();
		std::vector<TTTMove>::iterator	itorM;

		ofstream		htmlfile;
		string			line;
		char			buff[20];

		while (itorR != moves.end())
		{
			Response	R = *itorR;
			TTTMove		m;
			itorM = std::find(sheets.begin(),sheets.end(), R.CmprMove);

			string filename = "html\\"+R.PlayerMove.substr(1)+".htm";
			htmlfile.open(filename.c_str());
			
			_itoa(R.Id, buff, 10);
			string sID(buff);

			htmlfile << "<HTML><HEAD><TITLE>TicTacToe Move #"+sID+"</TITLE></HEAD><BODY BGCOLOR=\"#FFFFFF\">" << endl;
//			htmlfile << endl << "HTML for " << R.PlayerMove.substr(1) << ".HTM" << endl;

			if (itorM != sheets.end())
			{
				m = *itorM;

				htmlfile << GenerateIntermPage(m);
			}
			else
			{
				htmlfile << GenerateFinalPage(R);
			}

			htmlfile << "</BODY></HTML>" << endl;
			htmlfile.close();

			itorR++;
		}

}
void main()
{
	TryAllMoves("?"+string(9, BLANK));

	GenerateHTML();
}
