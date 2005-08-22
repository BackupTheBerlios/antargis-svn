/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_rect.h
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "Antargis" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */

#ifndef __AN_RECT_H
#define __AN_RECT_H

#include <list>
#include "ant_geometry.h"

struct Rect2D
{
  int x,y,w,h;

  Rect2D();
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
