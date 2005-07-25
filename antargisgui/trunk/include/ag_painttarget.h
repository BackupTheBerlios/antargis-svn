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
class AGTriangle;


#define MSTUB { throw int();}

class AGPaintTarget
{
 public:
  virtual void blitTri(const AGTexture &pSource,const AGTriangle &pSrc,const AGTriangle &pDest) MSTUB
  virtual void blit(const AGTexture &pSource,const AGRect &pDest) MSTUB
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc) MSTUB
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGColor &pColor) MSTUB
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc,const AGColor &pColor) MSTUB
  virtual void tile(const AGTexture &pSource) MSTUB
  virtual void tile(const AGTexture &pSource,const AGRect &pDest) MSTUB
  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc) MSTUB

  virtual void blit(const AGSurface &pSource,const AGRect &pDest) MSTUB
  virtual void blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc) MSTUB
  virtual void tile(const AGSurface &pSource) MSTUB
  virtual void tile(const AGSurface &pSource,const AGRect &pDest) MSTUB
  virtual void tile(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc) MSTUB

  virtual void drawRect(const AGRect &pRect,const AGColor &c) MSTUB
  virtual void drawLine(const AGPoint &p0,const AGPoint &p1,const AGColor &c) MSTUB
  
  virtual AGRect getRect() const
  {
    return AGRect(0,0,0,0);
  }
  
  virtual void drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr) MSTUB
  virtual void drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr) MSTUB
  virtual void drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2) MSTUB
  virtual void renderText (const AGRect &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn) MSTUB

  
  virtual void putPixel(int x,int y,const AGColor &c) MSTUB
  virtual AGColor getPixel(int x,int y) const MSTUB
};


#endif

