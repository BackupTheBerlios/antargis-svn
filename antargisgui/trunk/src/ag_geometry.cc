#include "ag_geometry.h"
#include <map>
#include <sstream>

using namespace std;


AGPoint::AGPoint(int pX,int pY):
  x(pX),y(pY)
{
}

AGPoint::AGPoint():
  x(0),y(0)
{
}

AGPoint AGPoint::operator-(const AGPoint &p) const
{
  return AGPoint(x-p.x,y-p.y);
}



// AGRect


AGRect::AGRect()
{
  SDL_Rect::x=0;
  SDL_Rect::y=0;
  SDL_Rect::w=0;
  SDL_Rect::h=0;
}


AGRect::AGRect(const AGRect &r):SDL_Rect(r)//mr(r.mr)
{
}
AGRect::AGRect(const SDL_Rect &r):SDL_Rect(r)//mr(r)
{
}
AGRect::AGRect(int px,int py,int pw,int ph)
{
  SDL_Rect::x=px;
  SDL_Rect::y=py;
  SDL_Rect::w=max(0,pw); // do not accept false values
  SDL_Rect::h=max(0,ph);
}
/*
int AGRect::x() const
{
  return SDL_Rect::x;
}
int AGRect::y() const
{
  return SDL_Rect::y;
}
int AGRect::width() const
{
  return SDL_Rect::w;
}
int AGRect::height() const
{
  return SDL_Rect::h;
  }*/
/*
SDL_Rect AGRect::rect() const
{
  return mr;
}
*/


AGRect AGRect::intersect(const AGRect &r) const
{
  int x2a,y2a,x2b,y2b;
  x2a=x+w;
  x2b=r.x+r.w;
  y2a=y+h;
  y2b=r.y+r.h;

  int x1=max(x,r.x);
  int y1=max(y,r.y);
  int x2=min(x2a,x2b);
  int y2=min(y2a,y2b);

  return AGRect(min(x1,x2),min(y1,y2),max(0,x2-x1),max(0,y2-y1));
}

bool AGRect::isNull() const
{
  return w==0 && h==0;
}

AGRect AGRect::shrink(int amount) const
{
  return AGRect(x+amount,y+amount,w-2*amount,h-2*amount);
}

bool AGRect::contains(const AGPoint &p) const
{
  return p.x>=x && p.y>=y && p.x<x+w && p.y<y+h;
}

AGRect AGRect::project(const AGRect &r) const
{
  return AGRect(x+r.x,y+r.y,r.w,r.h);
}
AGPoint AGRect::project(const AGPoint &r) const
{
  return AGPoint(x+r.x,y+r.y);
}


std::string AGRect::toString() const
{
  std::ostringstream os;
  os<<"("<<x<<","<<y<<","<<w<<","<<h<<")";
  return os.str();
}

AGPoint AGRect::getPosition() const
{
  return AGPoint(x,y);
}

AGRect AGRect::origin() const
{
  return AGRect(0,0,w,h);
}

bool AGRect::operator==(const AGRect &r) const
{
  return x==r.x && y==r.y && w==r.w && h==r.h;
}
bool AGRect::operator!=(const AGRect &r) const
{
  return !operator==(r);
}

ostream &operator<<(ostream &o,const SDL_Rect &r)
{
  o<<"("<<r.x<<","<<r.y<<","<<r.w<<","<<r.h<<")";
  return o;
}

AGRect AGRect::operator+(const AGPoint &p) const
{
  return AGRect(x+p.x,y+p.y,w,h);
}
