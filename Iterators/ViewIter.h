//
//  Iterator of the Week
//
// A regular (if not actually weekly) series of Open-source discussion 
// projects.
//
//  If this get's managled by your newsreader, a clean
// copy get by found at:
// http://www.noveltheory.com/Iterators/ViewIter.h

// ViewIter -- An STL-style iterator over the CViews of
// a particular CDocument.
//
// Usage example:
//  void SomeFunc(CView* vv);
//
//	CDocument* dd = GetDocument();
//	std::for_each(ViewIter(dd), ViewIter(), SomeFunc);
//
//
// Copyright 2002, James M. Curran   jamescurran@mvps.org
//
// May be used freely.
//
// Comments, suggestions, modifications, improvements welcome....
//

class ViewIter : public std::iterator<std::forward_iterator_tag, CView*>

// First thing to note, is that while it functions somewhat like a 
// pointer, a ViewIter is an object, so be careful not to declare it as 
// ViewIter* iter;   also, you'd have to say iter.memfunc() instead of
// iter->memfunc() -- except that ViewIter has no member functions (except 
// operators) to call.
//
// The next important detail is the iterator<> base class.
//  forward_iterator_tag says what kind of iterator this is (basically
//  it can only move forward in the list of CViews), and CView* is the 
//  datatype of the thing that *iter will return.  iterator<>  defines a 
//  number of typedefs which are used by the standard algorithms.  We could 
//  define these ourselves, but it's best to let the standard library work 
//  for us.	
{
private:
	POSITION	m_pos;
	CDocument*	m_src;
	CView*		m_curr;

// Like many collections in MFC, enumerating CView is done via a POSITION 
// object, so our iterator will need one of them.  Since we will be calling
// member functions of CDocument, we'll have to keep a pointer to one of 
// them around.  And, though it may not be obvious right now, we'll also 
// need to store a CView pointer.  We'll get to why in a minute.


public:
	ViewIter(): m_src(NULL), m_pos(NULL), m_curr(NULL)
	{ }
// Next up, the constructors.  We'll start with the simpler one, the 
// default ctor.  No parameters and everything set to NULL.  This pretty 
// much makes it a "dead object" as we aren't going to define a way to 
// initialize a ViewIter object after it's constructed.  So, what's it good 
// for? Well, for a standard container we'd have begin() and end() member 
// functions to provide that range iterators.  CDocument doesn't provide 
// them, so we have to make do.  The beginning iterator is given by 
// ViewIter(pDoc); by convention in these cases, a default constructed iter 
// --ViewIter() -- creates a "passed-the-end" iterator.


	ViewIter(CDocument* src): m_src(src), m_pos(src->GetFirstViewPosition())
	{ 
		operator++();	
	}
// For the important ctor, we just store the CDoument pointer, then 
// initialize the POSITION object by calling GetFirstViewPosition().  Note, 
// however, that that function doesn't actually get the first position; it 
// must be followed by a call to GetNextView(), but that's what we do in 
// oper++, so we'll just call that.

	ViewIter& operator++()
	{	
		m_curr = m_src->GetNextView(m_pos);
		return *this;
	}

// oper++ is the heart of the class. Now, here it's important to note the 
// difference between the way MFC handles iterating through collections, 
// and the way STL does.  Basically, in STL, getting the current and moving 
// to the next are two distinct operations, while under MFC, they are just 
// one.  This complicates simulating an STL interface, since we'll have to 
// separate them. This is why we needed to store a copy of the current 
// CView -- so we'll have it available later when we call oper*

// Another note things to note, is that this implements the prefix oper++ 
// (++iter), and nowhere do we define the postfix operator (iter++).
// Technically, to be a true forward iterator, we should define the postfix
// operator, but it is rarely if ever used in the standard algorithms.  
// More importantly, it is expected to return a copy of the iterator in the 
// not-yet-incremented state, and I didn't want to be bother thinking about 
// if that would work.  Just eliminating the postincrement made life much 
// easier.
	
	CView* operator*()
	{	return m_curr;		}

// And thus, oper*. Note, coded this way, they maintain the correct 
// behavior of the STL operators:  two oper++s in a row never return the 
// first object; two oper*s in a row return the same object each time.

	bool operator==(const ViewIter& rhs)
	{	return m_pos == rhs.m_pos;	}

// If two POSITION object have the same value, it's a good bet they are 
// iterating the same path.  More importantly, if a POSITION object equal 
// NULL, it's reached the end of it's range, which is what we are really 
// checking for when we compare iterators 99.99% of the time.  So, if the 
// m_pos member of a ViewIter object pretending to be the "past-the-end" 
// indicator equals NULL, than oper== will return true, when it is compared 
// to a "live" ViewIter which has reached the end.  And, sure enough, 
// that's what we set m_pos to in the default ctor!	

	bool operator!=(const ViewIter& rhs)
	{	return !operator==(rhs);	}

// OK, actually, people really don't care when iterators are equal -- they 
// care when they are not equal...
	
};

// NOTE: This is part of a work-in-progress.  It may eventually lead to a
// print article, a series of print articles, and/or a book. Or, it may
// not.  By participating in the public discussion of these articles, you
// consent to allowing your contribution to be used in any subsequent work
// derived from this without compensation.  I will make a good-faith
// effort to acknowledge those that have contributed.
//
