#ifndef __AN_RECT_H
#define __AN_RECT_H

#include <list>
#include "ant_geometry.h"

struct Rect2D
{
  int x,y,w,h;

  Rect2D(int X,int Y,int W,int H);
  Rect2D(const AntRect &r);

  bool splittable() const;
  std::list<Rect2D> split() const;

  bool contains(const Pos2D &p) const;

  bool intersects(const Rect2D &r) const;

  bool contains(const Rect2D &r) const;

  Pos2D get00() const;
  Pos2D get10() const;
  Pos2D get01() const;
  Pos2D get11() const;
};

#endif
