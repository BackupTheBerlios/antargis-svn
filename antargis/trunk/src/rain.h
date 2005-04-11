/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * rain.h
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

#ifndef __RAIN_H__
#define __RAIN_H__

#include <list>
#include <ag_gsurface.h>

class Rain
{
  std::list<AGPoint> rainSprites;
  std::list<AGPoint> snowSprites;
  AGTexture rainTexture,snowTexture;
  float rainSpeed,snowSpeed;
  Uint32 last;
  int w,h;
  int mAmount;
  float mSnow;
  
  public:
  Rain(int W,int H,int amount);
  virtual ~Rain();
  void addRain(int amount,bool r=false);
  virtual void draw(const AGRect&);
  void move();
};

#endif
