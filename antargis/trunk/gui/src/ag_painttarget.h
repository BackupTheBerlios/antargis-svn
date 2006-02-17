/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_painttarget.h
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

#ifndef __AG_PAINTTARGET_H
#define __AG_PAINTTARGET_H

#include <ag_geometry.h>
#include <ag_color.h>
#include <ag_font.h>

class AGSurface;
class AGTexture;

#define MSTUB { throw int();}

class AGPaintTarget
{
 public:
  virtual ~AGPaintTarget()
    {
    }
  virtual void blitTri(const AGTexture &pSource,const AGTriangle2 &pSrc,const AGTriangle2 &pDest) MSTUB

  virtual void blit(const AGTexture &pSource,const AGRect2 &pDest,const AGRect2 &pSrc,const AGColor &pColor) MSTUB
  virtual void blit(const AGTexture &pSource,const AGRect2 &pDest,const AGRect2 &pSrc) MSTUB
  virtual void blit(const AGSurface &pSource,const AGRect2 &pDest,const AGRect2 &pSrc) MSTUB

  virtual void drawLine(const AGVector2 &p0,const AGVector2 &p1,const AGColor &c) MSTUB
  virtual void fillRect(const AGRect2 &pr,const AGColor &c) MSTUB
      
  virtual AGRect2 getRect() const
  {
    return AGRect2(0,0,0,0);
  }
  
  virtual void putPixel(int x,int y,const AGColor &c) MSTUB
  virtual AGColor getPixel(int x,int y) const MSTUB

  virtual void beginPaint()
  {
  }
  virtual void endPaint()
  {
  }
};


#endif

