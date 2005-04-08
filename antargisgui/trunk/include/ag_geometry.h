/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_geometry.h
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

#ifndef AG_GEOMETRY_H
#define AG_GEOMETRY_H

#include <SDL.h>
#include <string>
#include <iostream>

class AGPoint
{
 public:
  int x,y;
  AGPoint(int pX,int pY);
  AGPoint();

  AGPoint operator-(const AGPoint &p) const;
  AGPoint &operator+=(const AGPoint &p);
};

class AGRect:public SDL_Rect
{
 public:
  AGRect();
  AGRect(const AGRect &r);
  AGRect(const SDL_Rect &r);
  AGRect(int x,int y,int w,int h);
  AGRect(const std::string &s);

  /*
  int x() const;
  int y() const;
  int width() const;
  int height() const;
  */
  AGRect intersect(const AGRect &r) const;

  bool isNull() const; // is empty

  AGRect shrink(int amount) const;

  bool contains(const AGPoint &p) const;

  AGRect project(const AGRect &r) const;
  AGPoint project(const AGPoint &r) const;

  AGRect origin() const;
  
  AGPoint getPosition() const;

  std::string toString() const;

  //  SDL_Rect rect() const;

  bool operator==(const AGRect &r) const;
  bool operator!=(const AGRect &r) const;

  AGRect operator+(const AGPoint &p) const;

 private:
  //  SDL_Rect mr;
};

std::ostream &operator<<(std::ostream &o,const SDL_Rect &r);
std::ostream &operator<<(std::ostream &o,const AGPoint &p);


#endif
