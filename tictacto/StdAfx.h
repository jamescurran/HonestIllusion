//---------------------------------------------------------------------------
// TicTacToe - StdAfx.hpp
//
//
// Copyright 1997, James M. Curran.  May be freely used.
//---------------------------------------------------------------------------
#if !defined(AFX_STDAFX_H__1F1BD12A_CEBA_11D0_9963_00AA0020E483__INCLUDED_)
#define AFX_STDAFX_H__1F1BD12A_CEBA_11D0_9963_00AA0020E483__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning(disable:4786)
#include <iostream>
#include <fstream>
//#include <afx.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#include <string>
#include <list>
#include <algorithm>
#include <vector>
#include <map>


using	std::string;
using	std::cout;
using	std::cin;
using	std::endl;
using	std::basic_ofstream<char,struct std::char_traits<char> >;
using	std::ofstream;

const char	XXXXX = 'X';
const char	OOOOO = 'O';
const char	BLANK = 'b';	


#endif // !defined(AFX_STDAFX_H__1F1BD12A_CEBA_11D0_9963_00AA0020E483__INCLUDED_)
