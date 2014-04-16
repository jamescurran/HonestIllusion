#Iterator of the Week
These are from a series of articles showcasing different uses for STL style iterators in C++. All source code presented here is copyright 1998-2002 James M. Curran, but may be used freely. All comment and suggestion are welcome.
 
 0. Circular Iterator The common usage of an iterator is to have it start at the beginning and progress to the end. But what if you don’t want to get off the roller coaster? What if you want through the container over and over again, just like a circular buffer. For that, we need a “circular” iterator 
1. CView Iterator Two years after my initial foray we try this thing again.... Step through the CView object attached to a CDocument. 
 2. POSITION IteratorBased on the CView Iterator, now we have an STL-style iterator templated to handle  any MFC container that uses a POSITION object. 
 3. SerialString Container & Iterator Iterator through a series of variable length, NUL terminated strings. 

Articles describing these are on my website, [http://noveltheory.com/iterators](http://noveltheory.com/iterators)