//---------------------------------------------------------------------------
// TicTacToe - Ttt.hpp
//
//
// Copyright 1997, James M. Curran.  May be freely used.
//---------------------------------------------------------------------------
#if !defined(TTT_HPP_INCLUDED)
#define TTT_HPP_INCLUDED

typedef int	ID;
enum	Result {unknown, cont, win, lose, tie};

struct TTTMove
{
	string	board;
	ID		response[9];

	TTTMove() {};

	TTTMove(string b): board(b) {};
	TTTMove& operator=(string rhs) 
	{
		board = rhs;
		memset(response, 0, sizeof(response));
		return(*this);
	}


};

inline bool operator==(TTTMove lhs, string rhs) 
{//	cout << "Comparing " << lhs.board.substr(1) << " with " << rhs.substr(1) << endl;
	return(lhs.board.substr(1) == rhs.substr(1));
}
inline bool operator==(string lhs, TTTMove rhs) {return(lhs.substr(1) == rhs.board.substr(1));}
inline bool operator==(TTTMove lhs, TTTMove rhs) {return (lhs.board.substr(1) == rhs.board.substr(1));}

inline bool operator<(TTTMove lhs, string rhs) {return (lhs.board.substr(1) < rhs.substr(1));}
inline bool operator<(TTTMove lhs, TTTMove rhs) {return (lhs.board.substr(1) < rhs.board.substr(1));}


class ResponseList;

class Response
{
public:
	ID		Id;
	string	PlayerMove;
	string	CmprMove;
	Result	result;
	int		count;
	friend	ResponseList;


public:
	Response() {PlayerMove.empty(); CmprMove.empty();  Id=(0); count=0; result = unknown;}
	Response(Response const &r) : PlayerMove(r.PlayerMove), CmprMove(r.CmprMove),  Id(r.Id), count(r.count), result(r.result) {}

	Response(string pl, string cp, int id): PlayerMove(pl), CmprMove(cp),  Id(id), count(1) 
	{
		switch (cp[0])
		{
		case '?':
			result = cont;
			break;

		case 'W':
			result = win;
			break;

		case 'L':
			result = lose;
			break;

		case 'T':
			result = tie;
			break;
		}
	}


friend bool operator==(Response  const &lhs, string rhs) ;
friend bool operator==(string lhs, Response const &rhs);

friend bool operator!=(Response  const & lhs, string rhs) ;
friend bool operator!=(string lhs, Response const & rhs) ;

friend bool operator<(Response  const & lhs, string rhs) ;
friend bool operator<(string lhs, Response  const & rhs) ;


};

inline bool operator==(Response const &lhs, Response const &rhs) {return(lhs.PlayerMove == rhs.PlayerMove );}
inline bool operator==(Response const &lhs, string const rhs) {return(lhs.PlayerMove == rhs);}
inline bool operator==(string const lhs, Response  const &rhs) {return(lhs == rhs.PlayerMove);}

inline bool operator==(Response const &lhs, int const rhs) {return(lhs.Id == rhs);}

inline bool operator!=(Response const & lhs, Response const & rhs) {return(lhs.PlayerMove != rhs.PlayerMove);}
inline bool operator!=(Response const & lhs, string const rhs) {return(lhs.PlayerMove != rhs);}
inline bool operator!=(string const lhs, Response const & rhs) {return(lhs != rhs.PlayerMove);}

inline bool operator<(Response const&lhs, Response const &rhs) {return(lhs.PlayerMove < rhs.PlayerMove);}
inline bool operator<(Response const & lhs, string const rhs) {return(lhs.PlayerMove < rhs);}
inline bool operator<(string const lhs, Response const& rhs) {return(lhs < rhs.PlayerMove);}

inline bool operator>(Response const&lhs, Response const &rhs) {return(lhs.PlayerMove > rhs.PlayerMove);}


class ResponseList 
{
	static	ID		nextID;
	std::list<Response>	lst;
	

public:
	Response GetResponse(string pl);
	void AddDirect(string board);
	Response GetResponse(int id);
	std::list<Response>::iterator	begin() {return lst.begin();}
	std::list<Response>::iterator	end()   {return lst.end();}

};







/*
class TTTRow
{
private:
	union 
	{
		char	m_row[3];
		struct 
		{
			char	m_col1;
			char	m_col2;
			char	m_col3;
		};
	};

	TTTRow() {};

	static	char	chars[4];
	static  string  disp(char x) {return (string(" ")+chars[x-'0']+" ");};

public:
	TTTRow(char ptr[]) : m_col1(ptr[0]), m_col2(ptr[1]), m_col3(ptr[2]) {};
	char operator[](int pos) {return(m_row[pos]);};
	friend ostream& operator<<(ostream& os,TTTRow tttr)
	{
		os << disp(tttr.m_col1) <<"|" << disp(tttr.m_col2) << "|" << disp(tttr.m_col3);
		return(os);
	}
};


class TTTBoard
{
private:
	TTTRow	m_row1;
	TTTRow	m_row2;
	TTTRow	m_row3;

public:
	TTTBoard(char ptr[]) : m_row1(&ptr[0]), m_row2(&ptr[3]), m_row3(&ptr[6]){};
	TTTRow &operator[](int pos) {return((&m_row1)[pos]);};
	friend ostream& operator<<(ostream& os,TTTBoard tttb)
	{
		os << tttb.m_row1 << endl;
		os << "---+---+---" << endl;
		os << tttb.m_row2 << endl;
		os << "---+---+---" << endl;
		os << tttb.m_row3 << endl;
		return(os);
	};
};
*/

#endif //!defined(TTT_HPP_INCLUDED)
