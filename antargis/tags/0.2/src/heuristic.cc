#include "heuristic.h"
#include <ag_debug.h>


HeuristicFunction::Output HeuristicFunction::operator()(const Input &input) 
{
  return (input.second-input.first).length()*1.5;
}


StoredHeuristicFunction::StoredHeuristicFunction(size_t res,float width):
  a(res*res*res*res),
  r(res),
  w(width)
{
}

void StoredHeuristicFunction::store(Input in,Output out)
{
  size_t i=getIndex(in);
  //  cdebug("index:"<<i);
  //  cdebug("value:"<<out);
  a[i]=out;
}

StoredHeuristicFunction::Output StoredHeuristicFunction::operator()(const Input &input)
{
  size_t i=getIndex(input);
  //  cdebug("index:"<<i);
  float v=a[i];
  //  cdebug("value:"<<v);

  return v*6;
}

size_t StoredHeuristicFunction::getIndex(const Input &in)
{
  size_t a,b,c,d;

  //  cdebug(in.first<<":::"<<in.second);

  a=in.first[0]*r/w;
  b=in.first[1]*r/w;
  c=in.second[0]*r/w;
  d=in.second[1]*r/w;

  a=std::min(a,r-1);
  b=std::min(b,r-1);
  c=std::min(c,r-1);
  d=std::min(d,r-1);

  assert(a<r);
  assert(b<r);
  assert(c<r);
  assert(d<r);

  size_t i=((a*r+b)*r+c)*r+d;

  //  cdebug(i);
  return i;
}


void StoredHeuristicFunction::display()
{
  for(int a=0;a<r;a++)
    for(int b=0;b<r;b++)
      for(int c=0;c<r;c++)
	for(int d=0;d<r;d++)
	  {
	    AGVector2 p0(a*w/r,b*w/r);
	    AGVector2 p1(c*w/r,d*w/r);

	    float value=operator()(std::make_pair(p0,p1));
	    if(value>0)
	      std::cout<<a<<","<<b<<","<<c<<","<<d<<":"<<value<<std::endl;
	  }
}
