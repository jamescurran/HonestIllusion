//
//  Iterator of the Week
//
// A regular (if not actually weekly) series of Open-source discussion 
// projects.
//
//	#2 - POSITIONiterator -- An STL-style iterator templted to handle 
//		any MFC container that uses a POSITION object.
//
// Copyright 2002, James M. Curran   jamescurran@mvps.org
//
// May be used freely.
//
// Comments, suggestions, modifications, improvements welcome....
// 
// Also, while this was independantly created, it trots over similar ground 
// as earlier works by 
// -David Lorde (http://www.codeguru.com/cpp_mfc/MFCIterator.shtml), and
// -Kevin Kostrzewa (C/C++ User Journal Sep 1999:"STL-Style Iterators for MFC")
// so you may want to compare this with those.
//
//
//
//  This is largely based on last week's (OK, it was actually more
//  like two+ weeks ago)iterator, ViewIter.  I'm just going to comment
//	on the bits that are different, so you may want to review that one. (It's 
//	also available here on the newsgroup)
//
//  One of the things you'll note about ViewIter if you were to try to 
// templatize it, is that it calls two functions that are specific to the 
// task of getting a CView* out of a CDocument*, namely GetFirstViewPosition()
// and GetNextView().  Every POSITION-based MFC collection has two similar, 
// but nevertheless unique, functions.  To turn ViewIter into a template, we 
// need a way of getting these function names into the template.  
//
// The first thought you might have is to try to pass pointers to these 
// functions in as template parameters (it was the first thought I had).  
// Even had the result worked (which it didn't), it would have been too messy
// to be practical.
//
// A better method is to create a class for each collection, which gives these
// functions common names.  Ideally, thet would follow the pattern:
//
//	template<typename out, typename in>
//	class Helper
//	{	
//		static POSITION first(const in* app)
//		{	....	}
//	
//		static out next(const in* app, POSITION& pos)
//		{	....	}
//	};
//
//	Of course, as we'll have to define each such class manually, there's not
//	much use in that template, but it helps to visualize the interface.
//	The implementations of each of these functions is just a single functions
//	call.  
//
//	For example:
//

#include <iterator>
#include <utility>
#include <afxwin.h>
#include <afxtempl.h>

struct ViewHelper
{
	static POSITION first(const CDocument* app)
	{	return app->GetFirstViewPosition(); }

	static CView* next(const CDocument* app, POSITION& pos)
	{	return app->GetNextView(pos); }

};


// Here is the actual template.  It's basically the ViewIter from last time
// converted into a template, with some additions/correction provided by
// Igor Tandetnik and Werner Salomon.

template <typename out, typename in, typename helper>
class POSITIONiterator : public std::iterator<std::forward_iterator_tag, out>
{
// Here's I'm sticking with it being a forward_iterator. ViewIter was also 
// declared as a forward_iterator, although it was really closer to being the
// less flexible input_iterator.  I decided to scale it up to be a real 
// forward_iterator, but I not really sure if those changes are safe.  We may
// have to revisit this point again.
private:
	const in*	m_src;
	POSITION	m_pos;
	POSITION	m_lastpos;
	out			m_item;

//	First thing you should note is that I've added a new data member.  Yep, 
//	there was a bug in ViewIter. The POSITION goes to NULL when it *reaches* 
//	the final item.  We need it to be NULL when it *passes* the final item.
//	Hence, lastpos which holds the previous m_pos value. When it goes to null,
//	we're done.
//
//	A more subtle point to note, is the types of the source and item members.
//	m_src is held as a pointer to the container, and m_item is the actual item
//	that it contains.  This is a slight change to the interface of ViewIter
//	where the container (a CDocuement) was passed to the ctor as a pointer 
//	(because you were more likely to have a CDocument* than a CDocument obj).
//	However, for CLists and CMaps, you're more likely to have an object.
//	Further, the fact that the objects contained in a CDocument were pointers
//	to CView object is incidental.  CLists and CMaps can hold object as well as
//	pointers, so we'll have to adapt.


public:
	POSITIONiterator(const in& src)
		: m_src(&src), 
		  m_pos(helper::first(&src)), 
		  m_lastpos(NULL), 
		  m_item(0)
	{ 
		if (m_pos)
			operator++();
	}

// Things to note here: 
// We pass the src container in by const reference, but store it as a pointer. 
// We call the static first() function from our helper class to initialize
//  the m_pos member, and
// We initialize the m_item to 0.  I'm a bit torn about this. Half of me says
//	we should explicitly initialize each member, and Half says, since we don't
//	know exact what type it is, we should let it just default initialize.
// Finally, we only move to the first item, if m_pos is not NULL (ie, there is
//	a first item). (Thanks, Igor)


	POSITIONiterator(): m_src(NULL), m_pos(NULL), m_lastpos(NULL), m_item(0)
	{ }


	bool operator==(const POSITIONiterator& rhs) const
	{	return m_lastpos == rhs.m_lastpos;	}

//	As I mentioned before, we hadn't reached the end until the lastpos is NULL.

	bool operator!=(const POSITIONiterator& rhs) const
	{	return !operator==(rhs);	}

	
	out operator*()
	{	return m_item;		}

	out* operator->()
	{	return &m_item;		}

	POSITIONiterator& operator++()
	{	
		m_lastpos = m_pos;
		if (m_pos)
			m_item = helper::next(m_src, m_pos);

		return *this;
	}
//	This is a bit more complicated than before, because now we realize we have 
//	to step passed that last item.   But here, as in the ctor, calling the 
//	correct next() function via the helper class is simple.

	POSITIONiterator operator++( int )  // postfix ++
	{
		POSITIONiterator old( *this );
		operator++();
		return( old );
	}
};
//
// OK, that's it.  To create the ViewIter class we had last week (with the bugs
//	fixed), all we need is the helper class we defined above as this typedef:

typedef	POSITIONiterator<CView*, CDocument, ViewHelper>
			ViewIter;

//-----------------------------------------------------------
// From there extending it to, say, get a list of CDocuments* from a 
// CDocTemplate; or a list of CDocTemplates* from a CWinApp, is just a matter
// of a simple helper class, and a typedef.

struct DocHelper
{
	static POSITION first(const CDocTemplate* app)
	{	return app->GetFirstDocPosition(); }

	static CDocument* next(const CDocTemplate* app, POSITION& pos)
	{	return app->GetNextDoc(pos); }
};

typedef	POSITIONiterator<CDocument*, CDocTemplate, DocHelper>
			DocIter;

//
//
//
struct DocTemplateHelper
{
	static POSITION first(const CWinApp* app)
	{	return app->GetFirstDocTemplatePosition(); }

	static CDocTemplate* next(const CWinApp* app, POSITION& pos)
	{	return app->GetNextDocTemplate(pos); }
};


typedef	POSITIONiterator<CDocTemplate*, CWinApp, DocTemplateHelper>
			DocTemplateIter;

//-----------------------------------------------------------
//	The general purpose container classes are just as easy, as with this
// set to get a list of voids* from a CPtrList
	
struct PtrListHelper
{
	static POSITION first(const CPtrList* lst)
	{	return lst->GetHeadPosition();	}

	static void* next(const CPtrList* lst, POSITION& pos)
	{	return lst->GetNext(pos);		}
};

typedef POSITIONiterator<void*, CPtrList, PtrListHelper>
		PtrListIter;


//-----------------------------------------------------------
// Similarly, the templated MFC containers can be done; we'll just need a
//	templated helper function.

template <typename TYPE, typename TYPE_ARG = TYPE>
struct ListHelper
{
	static POSITION first(const CList<TYPE, TYPE_ARG>* app)
	{	return app->GetHeadPosition();	}

	static TYPE next(const CList<TYPE, TYPE_ARG>* app, POSITION& pos)
	{	return app->GetNext(pos); }

};

// Unfortunately, we cannot create a templated typedef (yet...), so we must
// write a typedef for each container instaniation we will be using.

typedef POSITIONiterator<int, CList<int, int>, ListHelper<int> >
		LintIter;

// Should a future C++ Standard allow templated typedef, here's the definition
// we'd need..

//template <typename out, typename out_arg = out>
//	typedef POSITIONiterator<out, CList<out, out_arg>, ListHelper<out, out_arg> >
//			ListIter;

// NOTE: This is part of a work-in-progress.  It may eventually lead to a
// print article, a series of print articles, and/or a book. Or, it may
// not.  By participating in the public discussion of these articles, you
// consent to allowing your contribution to be used in any subsequent work
// derived from this without compensation.  I will make a good-faith
// effort to acknowledge those that have contributed.
//

