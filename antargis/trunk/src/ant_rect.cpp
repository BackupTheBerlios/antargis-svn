#include "ant_rect.h"

Rect2D::Rect2D(int X,int Y,int W,int H):
  x(X),y(Y),w(W),h(H)
{
  
}

bool Rect2D::splittable() const
{
  return w>2 && h>2;
}
std::list<Rect2D> Rect2D::split() const
{
  std::list<Rect2D> l;
  int w2=w/2;
  int h2=h/2;
  int w2a=w-w2;
  int h2a=h-h2;

  l.push_back(Rect2D(x   ,y   ,w2,h2));
  l.push_back(Rect2D(x+w2,y   ,w2a,h2));
  l.push_back(Rect2D(x   ,y+h2,w2,h2a));
  l.push_back(Rect2D(x+w2,y+h2,w2a,h2a));

  return l;
}


bool Rect2D::contains(const Pos2D &p) const
{
  return p.x>=x && p.y>=y && p.x<x+w && p.y<y+h;
}

bool intersect(int a1,int a2,int b1,int b2)
{
  if(a1>=b1 && a1<=b2)
    return true;
  if(a2>=b1 && a2<=b2)
    return true;

  if(b1>=a1 && b1<=a2)
    return true;
  if(b2>=a1 && b2<=a2)
    return true;

  return false;
}

bool Rect2D::intersects(const Rect2D &r) const
{
  return intersect(x,x+w-1,r.x,r.x+r.w-1) && intersect(y,y+h-1,r.y,r.y+r.h-1);
}

bool Rect2D::contains(const Rect2D &r) const
{
  return contains(r.get00()) && contains(r.get01()) && contains(r.get10()) && contains(r.get11());
}


Pos2D Rect2D::get00() const
{
  return Pos2D(x,y);
}
Pos2D Rect2D::get10() const
{
  return Pos2D(x+w-1,y);
}
Pos2D Rect2D::get01() const
{
  return Pos2D(x,y+h-1);
}
Pos2D Rect2D::get11() const
{
  return Pos2D(x+w-1,y+h-1);
}

Rect2D::Rect2D(const AntRect &r)
{
  x=r.x;
  y=r.y;
  w=r.w;
  h=r.h;
}
