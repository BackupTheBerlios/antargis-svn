#include "ag_algebra.h"
#include "ag_debug.h"
#include "ag_gauss.h"
#include "ag_stringstream.h"



AGMatrixN::AGMatrixN(size_t w,size_t h):m(w*h,0),
mW(w),mH(h)
{
}

AGMatrixN::AGMatrixN(const AGMatrixN &n):m(n.m),
mW(n.mW),
mH(n.mH)
{
}

void AGMatrixN::set(size_t x,size_t y,float v)
  {
    assert(x<mW);
    assert(y<mH);
    m[index(x,y)]=v;
  }
float AGMatrixN::get(size_t x,size_t y) const
{
  assert(x<mW);
  assert(y<mH);
  return m[index(x,y)];
}

AGMatrixN &AGMatrixN::operator*=(const AGMatrixN &p)
{
  assert(mW==p.mH);
  AGMatrixN n(p.mW,mH);
  for(size_t x=0;x<p.mW;x++)
    for(size_t y=0;y<mH;y++)
      {
        float v=0;
        for(size_t k=0;k<mW;k++)
          v+=get(k,y)*p.get(x,k);
        n.set(x,y,v);
      }
  *this=n;
  return *this;

}
AGMatrixN AGMatrixN::operator*(const AGMatrixN &p) const
{
  assert(mW==p.mH);
  AGMatrixN n(p.mW,mH);
  for(size_t x=0;x<p.mW;x++)
    for(size_t y=0;y<mH;y++)
      {
        float v=0;
        for(size_t k=0;k<mW;k++)
          v+=get(k,y)*p.get(x,k);
        n.set(x,y,v);
      }
  return n;
}

AGMatrixN AGMatrixN::operator-(const AGMatrixN &p) const
{
  assert(mW==p.mW);
  assert(mH==p.mH);
  AGMatrixN n(mW,mH);

  for(size_t x=0;x<mW;x++)
    for(size_t y=0;y<mH;y++)
      n.set(x,y,get(x,y)-p.get(x,y));
  return n;
}

float AGMatrixN::scalar() const
{
  float s=0;
  float v;
  for(size_t x=0;x<mW;x++)
    for(size_t y=0;y<mH;y++)
      {
        v=get(x,y);
        s+=v*v;
      }
  return sqrt(s);
}


AGMatrixN AGMatrixN::makeQuadratic() const
{
  size_t nw=std::max(mW,mH);
  AGMatrixN a(nw,nw);
  a.copyFrom(*this);
  return a;
}

/// a very simple way to compute a pseudo-inverse
/// using svd may be better - no matter ;-)
/// for details go to http://en.wikipedia.org/wiki/Pseudoinverse
AGMatrixN AGMatrixN::pseudoInverse() const
{
  AGMatrixN t=transposed();
  if(mW<mH)
    return (t*(*this)).inverse()*t;
  else
    return t*((*this)*t).inverse();
}

AGMatrixN AGMatrixN::transposed() const
{
  AGMatrixN a(mH,mW);
  for(size_t x=0;x<mW;x++)
    for(size_t y=0;y<mH;y++)
      a.set(y,x,get(x,y));
  return a;
}



AGMatrixN AGMatrixN::inverse() const
{
  size_t nw=std::max(mW,mH);
  AGMatrixN a(nw,nw),b(nw,nw);

  a.makeUnitMatrix();
  b.copyFrom(*this);
  gauss(a,b,nw);
  return a;
}

void AGMatrixN::makeUnitMatrix()
  {
    for(size_t x=0;x<mW;x++)
      for(size_t y=0;y<mH;y++)
        set(x,y,(x==y?1:0));
  }

void AGMatrixN::copyFrom(const AGMatrixN &p)
  {
    for(size_t x=0;x<mW && x<p.mW;x++)
      for(size_t y=0;y<mH && y<p.mH;y++)
        set(x,y,p.get(x,y));
  }

void AGMatrixN::output() const
{
  for(size_t y=0;y<mH;y++)
    {
      for(size_t x=0;x<mW;x++)
        {
          std::cout<<get(x,y)<<"\t";
        }
      std::cout<<"\n";
    }

}

AGString AGMatrixN::toString() const
{
  AGStringStream s;
  for(size_t y=0;y<mH;y++)
    {
      for(size_t x=0;x<mW;x++)
        {
          s<<get(x,y)<<"\t";
        }
      s<<"\n";
    }
  return s.str();
}

void AGMatrixN::swapRows(size_t a,size_t b)
  {
    if(a==b)
      return;
    assert(a<mH);
    assert(b<mH);
    for(size_t x=0;x<mW;x++)
      {
        float t=get(x,a);
        set(x,a,get(x,b));
        set(x,b,t);
      }
  }
void AGMatrixN::swapCols(size_t a,size_t b)
  {
    if(a==b)
      return;
    assert(a<mW);
    assert(b<mW);
    for(size_t y=0;y<mH;y++)
      {
        float t=get(a,y);
        set(a,y,get(b,y));
        set(b,y,t);
      }
  }
