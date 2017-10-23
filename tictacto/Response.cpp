//---------------------------------------------------------------------------
// TicTacToe - Response.cpp
//
//
// Copyright 1997, James M. Curran.  May be freely used.
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "ttt.hpp"

extern string CompMove(string);

	ID	ResponseList::nextID = 0;

Response ResponseList::GetResponse(string pl)
{
		Response	resp;
		std::list<Response>::iterator	itor;
		itor = std::find(lst.begin(), lst.end(), pl);

		if (itor != lst.end())
		{
			(*itor).count++;
			resp = *itor;
		}
		else
		{
			string cp = CompMove(pl);
			Response resp1(pl, cp,++nextID);
			lst.push_back(resp1);
			resp = lst.back();
		}

		return(resp);
}

void ResponseList::AddDirect(string board)
{
	Response startR = Response(board, board, ++nextID);
	lst.push_back(startR);
}


Response ResponseList::GetResponse(int id)
{
		Response	resp;
		std::list<Response>::iterator	itor;
		itor = std::find(lst.begin(), lst.end(), id);

		if (itor != lst.end())
		{
			resp = *itor;
		}

		return(resp);
}
