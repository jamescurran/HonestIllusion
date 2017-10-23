//---------------------------------------------------------------------------
// TicTacToe - Tic.cpp
//
//
// Copyright 1997, James M. Curran.  May be freely used.
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "TestCase.h"
#include "TTTGrid.h"

//XOmXOmXmOmm

// 
string CompMove(string cells) 
{
	int		i = 0;
	int		j = 0;
	int		point = 0;
	char	ch;

	TTTGrid		board(cells);
	TestCase	tc;

	// Test for Lose

   // Check rows for 2/3
   for(i = 0; i < 3; i++) 
   {
		tc.Clear();
		for(j = 0; j < 3; j++) 
		{
			ch = board(i, j);
			tc.Test(ch);
		}

		if (tc.IsLose() )
		{
			board.MarkLose();
			return board.AsString();
		}

   }	


   // Check Columns for 2/3
   for(j = 0; j < 3; j++) 
   {
	   tc.Clear();
		for(i = 0; i < 3; i++) 
		{
			ch = board(i, j);
			tc.Test(ch);
		}

		if (tc.IsLose() )
		{
			board.MarkLose();
			return board.AsString();
		}

   }	

	// Check Diagonal 1 for 2/3
	tc.Clear();

	for( i = 0; i < 3; i++) 
	{
		ch = board(i,i);
		tc.Test(ch);
	}

	if (tc.IsLose() )
	{
		board.MarkLose();
		return board.AsString();
	}

	// Check diagonal 2 for 2/3
	tc.Clear();

	for( i = 0; i < 3; i++) 
	{
		ch = board(i,2 -i);
		tc.Test(ch);
	}

	if (tc.IsLose() )
	{
		board.MarkLose();
		return board.AsString();
	}



	// Check for Win
   // Check rows 
   for(i = 0; i < 3; i++) 
   {
		tc.Clear();
		for(j = 0; j < 3; j++) 
		{
			ch = board(i, j);
			tc.Test(ch);
		}

		if (tc.IsWin() )
		{
			board.at(i, tc.Point()) = OOOOO;
			board.MarkWin();
			return board.AsString();
		}


   }	


   // Check Columns for 2/3
   for(j = 0; j < 3; j++) 
   {
	   tc.Clear();
		for(i = 0; i < 3; i++) 
		{
			ch = board(i, j);
			tc.Test(ch);
		}

		if (tc.IsWin() )
		{
			board.at(tc.Point(), j) = OOOOO;
			board.MarkWin();
			return board.AsString();
		}

   }	

	// Check Diagonal 1 for 2/3
	tc.Clear();

	for( i = 0; i < 3; i++) 
	{
		ch = board(i,i);
		tc.Test(ch);
	}

	if (tc.IsWin() )
	{
		board.at(tc.Point(), tc.Point()) = OOOOO;
		board.MarkWin();
		return board.AsString();
	}

	// Check diagonal 2 for 2/3
	tc.Clear();

	for( i = 0; i < 3; i++) 
	{
		ch = board(i,2 -i);
		tc.Test(ch);
	}

	if (tc.IsWin() )
	{
		board.at(tc.Point(), 2 - tc.Point()) = OOOOO;
		board.MarkWin();
		return board.AsString();
	}


	// check for block potenial
   // Check rows 
   for(i = 0; i < 3; i++) 
   {
		tc.Clear();
		for(j = 0; j < 3; j++) 
		{
			ch = board(i, j);
			tc.Test(ch);
		}


		if (tc.IsBlock() )
		{
			board.at(i, tc.Point()) = OOOOO;
			return board.AsString();
		}

   }	


   // Check Columns for 2/3
   for(j = 0; j < 3; j++) 
   {
	   tc.Clear();
		for(i = 0; i < 3; i++) 
		{
			ch = board(i, j);
			tc.Test(ch);
		}

		if (tc.IsBlock() )
		{
			board.at(tc.Point(), j) = OOOOO;
			return board.AsString();
		}

   }	

	// Check Diagonal 1 for 2/3
	tc.Clear();

	for( i = 0; i < 3; i++) 
	{
		ch = board(i,i);
		tc.Test(ch);
	}

	if (tc.IsBlock() )
	{
		board.at(tc.Point(), tc.Point()) = OOOOO;
		return board.AsString();
	}

	// Check diagonal 2 for 2/3
	tc.Clear();

	for( i = 0; i < 3; i++) 
	{
		ch = board(i,2 -i);
		tc.Test(ch);
	}

	if (tc.IsBlock() )
	{
		board.at(tc.Point(), 2 - tc.Point()) = OOOOO;
		return board.AsString();
	}

	point = 15;
	if (board(1,1) == BLANK)
	{
		point = 5;
		board.at(point) = OOOOO;
		return board.AsString();
	}



   // check weakness
   if( board(1,2) == XXXXX ) 
   {
		if( board(2,2) == BLANK) 
		{
			point = 9;
		} 
		else if(board(2,1) == BLANK) 
		{
			point = 8;
		}
   } 
   else if( board(2,1) == XXXXX ) 
   {
       if( board(2,2) == BLANK) 
	   {
            point = 9;
       } else 
			if(board(1,2) == BLANK) 
			{
				point = 6;
			}
	}

	if( point != 15) 
	{
		board.at(point) = OOOOO;
		return board.AsString();
	}


   // Check Corner weakness
   point = 15;
   if( board(0,0) == XXXXX) 
   {
       if( board(2,2) == BLANK) 
	   {
           point = 9;
       }
   } 
   else if( board(0,2) == XXXXX) 
		{
			if( board(2,0) == BLANK) 
			{
				point = 7;
			}
		} 
   else if( board(2,0) == XXXXX) 
		{
			if( board(0, 2)  == BLANK) 
			{
				point = 3;
			}
		} 
   else if( board(2,2)  == XXXXX) 
		{
			if( board(0,0)  == BLANK) 
			{
				point = 1;
			}
		}

	if( point != 15) 
	{
		board.at(point) = OOOOO;
		return board.AsString();
	}

   //Otherwise, check the corners
	point = 15;
	if( cells[1] == BLANK) 
	{
		point = 1;
	} 
   else if( cells[3] == BLANK) 
		{
			point = 3;
		} 
   else if( cells[7] == BLANK) 
		{
			point = 7;
		} 
   else if( cells[9] == BLANK) 
		{
			point = 9;
		}

	if( point != 15) 
	{
		board.at(point) = OOOOO;
		return board.AsString();
	}

	//If center is open, then go there
	if( board(1,1) == BLANK)
	{
		point = 5;
		board.at(point) = OOOOO;
		return board.AsString();
	}


   //Otherwise, move next to opponent
   point = 15;
   for(i = 2; i < 10; i++) 
   {
        char	cur = cells[i];
        char	prev = cells[i-1];
        char	next = cells[i+1];

		if( cur == XXXXX ) 
		{
			if(prev == BLANK) 
			{
				point = i-1;
			} 
			else 
				if( next == BLANK ) 
				{
					point = i+1; 
				}
		}
   }

	if( point != 15) 
	{
		board.at(point) = OOOOO;
		return board.AsString();
	}

	board.MarkTie();
	return board.AsString();
}


