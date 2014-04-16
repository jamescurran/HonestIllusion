//
//  Iterator of the Week  (26-Nov-2002)
//
// A regular (if not actually weekly) series of Open-source discussion 
// projects.
//
//  If this gets mangled by your newsreader, a clean
// copy get by found at:
// http://www.noveltheory.com/Iterators/serialstring.h
//
//	#3 - SerialString -- An STL-style container adapter (with iterator) 
//  to retrieve the items from a list of variable length string.
//
// Copyright 2002, James M. Curran   jamescurran@mvps.org
//
// May be used freely.
//
// Comments, suggestions, modifications, improvements welcome....
// 
// Some Win32 APIs, such as PdhEnumObjects, return a collection of text
// as a series of NUL terminated string (with the complete list terminated
// by a second NUL.  For example:
//  "hello world\0greetings earthling\0\0"
//
//  These classes are designed for easily handling of such collections.

#if !defined(SERIALSTRING_H_INCLUDED)
#define SERIALSTRING_H_INCLUDED

#include <string.h>
#include <string>
#include <memory>
#include <iterator>

// Now, before we can define the iterator, we must define the container 
// that we will be iterator through.
// It was pointed out to me (by Craig Powers) that instead of using MFC's
// TCHARS, it would be better (and more Standard) to use a template.
template<typename CHAR>
class SerialString
{
// Note they only care about reading items out of this container, so
//all we're going to need it a pointer to the first item.
// -- revising this to allow writing such containers is left as an 
// exercise for the student.
	const CHAR*	pBegin;
	 bool		self_delete;

public:
// I had to revise the ctor a bit here. 
// Originally, I had a ctor which took an auto_ptr.  The problem is that 
// auto_ptr can't be used with an array. (I'd forgotten about that).
// But I still want to be able to use this with a dynamically allocated
// character buffer -- and without worrying about calling delete myself.
// So, I replace the auto_ptr ctor, with one -- declared explicit --- which
// takes an integer, allocates a buffer that size, and later deletes it.
// The self_delete boolean differentiates between internally and 
// externally allocated buffers.
	SerialString( const CHAR* p) : pBegin(p), self_delete(false)  {}
	explicit SerialString(int n) : pBegin(new CHAR[n] ), self_delete(true) {}

// Since this is useless unless filled, and it, by it's current design, 
// must be filled externally, but the buffer needing to be filled may be
// created internally, we gonna need a way to access that internal 
// buffer.  This is not the slickest way of dealing with the problem 
// but it'll work for now.  Unfortunately, this is (nearly) the only
// spot where we'd need non-const access.  ("Only" is a bit on a misnomer
// as this function drives a truck through const-correctness)
	CHAR*	data()			{ return const_cast<CHAR*>(pBegin);	}
	
// On the dtor, we delete the buffer only if we allocated it.
// VC6 insisted on a non-const buffer here, although I'm not sure
// if that is per the Standard.
	~SerialString() { if (self_delete)  delete[] data();  }

// Now, before we can continue defining the container (we're pretty
// much done with it anyway), we have to start on the iterator 
// (this is where all the flashy parts are anyway)

// Our goal here is to make the SerialString look as if it were an
// array of char*s, so when derefenced, an iterator should return a 
// char* pointing to the start of a string. After incrementing, it should
// return a char* pointing to the start of the next string.

	class iterator : public std::iterator<std::forward_iterator_tag, const CHAR*>
	{
// Like the container, we'll only need a simple char* for our data member.
		 const CHAR*	ptr;
	public:
		iterator( const CHAR* p) : ptr(p) {}

// First, let's look at derefenceing -- nothing fancy here. 
// I've included a member function function version in addition to 
// the standard oper*
		const  CHAR* operator*() const 
				{ return ptr; }
		const CHAR* Ptr() const 
				{return ptr; }

// When increamenting, you want to move the internal pointer to the
// start of the next string, which means we want to move it by the
// number of characters in the current string. 
// NOTE: std::char_traits<CHAR>::length is the ISO Standard C++
// way of saying either strlen() or wcslen() depending on the type
// of CHAR  (Thanks to Igor Tandetnik for that).
		iterator operator++()   
		{ ptr +=std::char_traits<CHAR>::length(ptr) + 1; return *this; }

		iterator operator++(int) 
		{ iterator t(ptr); ptr += std::char_traits<CHAR>::length(ptr) + 1; return t; }

// The next tricky item is the defining Equality between two 
//	SerialString::iterators.  Naturally, they are equal if their internal
// pointers are equal, but as we'll see in a moments, we're defining
// a generic "end" iterator as having a NULL internal pointer, so we'll
// need to return True, if one iterator is an End and the other really
// is at the end.
		static bool Equal(const iterator& lhs,  const iterator& rhs)
				{ 
					return	lhs.ptr==rhs.ptr  || 
								(lhs.ptr==0 && *rhs.ptr==CHAR('\0') ) || 
								(rhs.ptr==0 && *lhs.ptr == CHAR('\0') ); 
				}
	};

// OK,  we're done defining the iterator class, 
// now back to the container class

// Here, we define a begin() & end() just like the standard STL
// containers.   begin() just creates an iterator at of it's internal
// pointer, while end() creates a NULL iterator.  Why NULL and not the
// real end of the container?  Because we don't know where it is, and 
// finding it would be a lot of work (O(n)).  Basically, we'd have to 
//	scan through the whole text block (with nexted loops, due to the
// internal NULs), looking for the repeated 00s.  Then, after we found 
// it, more than likely, the user's code would scan through the text
// again, doing the real work.  So, I've basically just move the act of 
// looking for the double 00s to the users loop.
//
	iterator begin() const { return iterator(pBegin); }
	iterator end()    const { return iterator(0);   }
};

// OK, almost done --- now two quickie global inline 
// functions for == and !=
template<typename CHAR>
inline bool operator==(const SerialString<CHAR>::iterator& lhs,  
					               const SerialString<CHAR>::iterator& rhs)
{	
	return  SerialString<CHAR>::iterator::Equal(lhs, rhs);
}

template<typename CHAR>
inline bool operator!=(const SerialString<CHAR>::iterator& lhs,  
					             const SerialString<CHAR>::iterator& rhs)
{
	return ! SerialString<CHAR>::iterator::Equal(lhs,rhs); 
}

// And finally, we'll tack on a output formatter, that makes printing an 
// iterator equivalent to printing what it points to.
#include <ostream>
template<typename CHAR>
inline std::ostream& operator<<(std::ostream& os, 
								                SerialString<CHAR>::iterator it)
{	
	os << it.Ptr();   
	return os;
}

#endif // defined(SERIALSTRING_H_INCLUDED)
