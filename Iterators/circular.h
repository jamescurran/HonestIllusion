// circular_iter.h: interface for the circular_iter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_circular_iter_H__C4D40888_285F_11D2_9968_00AA0020E483__INCLUDED_)
#define AFX_circular_iter_H__C4D40888_285F_11D2_9968_00AA0020E483__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iterator>


template<typename It>
struct container
{
	typedef	It::value_type	value_type;
	typedef It				iterator;
};


template<typename T>
class const_circular_iter : public std::iterator<std::bidirectional_iterator_tag, T::value_type>  
{
protected:
		T::iterator	begin;
		T::iterator	end;
		T::iterator	iter;
public:
	const_circular_iter(T& t) : iter(t.begin()), begin(t.begin()), end(t.end()) {};
	const_circular_iter(T::iterator b, T::iterator e) : iter(b), begin(b), end(e) {};

	const_circular_iter<T>& operator++()
	{	
		++iter;

		if (iter == end)
			iter = begin;

		return(*this);
	}

	const_circular_iter<T> operator++(int)
	{
		const_circular_iter<T>	t=*this;
		++(*this);
		return(t);
	}

	const T::value_type operator*() const 
		{return (*iter);}

	bool operator==(const const_circular_iter<T>&	rhs) const
		{return (iter == rhs.iter);}

	bool operator!=(const const_circular_iter<T>&	rhs) const 
		{return ! operator==(rhs); }
};

template<typename T>
class term_const_circular_iter : public const_circular_iter<T>
{
private:
	int count;

public:
	term_const_circular_iter(T& t, int c = 0) :  const_circular_iter<T>(t), count(c) {};
	term_const_circular_iter(T::iterator b, T::iterator e, int c = 0) : const_circular_iter<T>(b,e), count(c) {};

	const T::value_type operator*() const 
	{return (*iter);}

//	T::value_type& operator*() 
//	{return (*iter);}

	term_const_circular_iter<T>& operator++()
	{	
		++iter;

		if (iter == end)
		{
			if (count !=0)
			{
				iter = begin;
				if (count > 0)
					count --;
			}
		}

		return(*this);
	}


	term_const_circular_iter<T> operator++(int)
	{
		term_const_circular_iter<T>	t=*this;
		++(*this);
		return(t);
	}

};





#if 1
template<typename T>
class circular_iter : public const_circular_iter<T>
{
public:
	circular_iter(T& t) :  const_circular_iter<T>(t) {};
	circular_iter(T::iterator b, T::iterator e) : const_circular_iter<T>(b,e) {};

	const T::value_type operator*() const 
	{return (*iter);}

	T::value_type& operator*() 
	{return (*iter);}
};





#else
template<typename T>
class circular_iter : public std::iterator<std::bidirectional_iterator_tag, T::value_type>  
{
private:
		T::iterator		begin;
		T::iterator		end;
		T::iterator		iter;
public:
	circular_iter(T& t) : iter(t.begin()), begin(t.begin()), end(t.end()) {};
	circular_iter(T::iterator b, T::iterator e) : iter(b), begin(b), end(e) {};

	circular_iter<T>& operator++()
	{	
		++iter;

		if (iter == end)
			iter = begin;

		return(*this);
	}

	circular_iter<T> operator++(int)
	{
		circular_iter<T>	t=*this;
		++(*this);
		return(t);
	}

	const T::value_type operator*() const 
	{return (*iter);}

	T::value_type& operator*() 
	{return (*iter);}

	bool operator==(const circular_iter<T>&	rhs) const
	{return (iter == rhs.iter)}

	bool operator!=(const circular_iter<T>&	rhs) const {return ! operator==(rhs); }
};
#endif

#endif // !defined(AFX_circular_iter_H__C4D40888_285F_11D2_9968_00AA0020E483__INCLUDED_)
