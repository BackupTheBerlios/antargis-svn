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

class AGPaintTarget
{
 public:
  virtual void blit(const AGTexture &pSource,const AGRect &pDest){}
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc){}
  virtual void tile(const AGTexture &pSource){}
  virtual void tile(const AGTexture &pSource,const AGRect &pDest){}
  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc){}

  virtual void blit(const AGSurface &pSource,const AGRect &pDest){}
  virtual void blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc){}
  virtual void tile(const AGSurface &pSource){}
  virtual void tile(const AGSurface &pSource,const AGRect &pDest){}
  virtual void tile(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc){}

  virtual void drawRect(const AGRect &pRect,const AGColor &c){}
  
  virtual AGRect getRect() const
  {
    return AGRect(0,0,0,0);
  }
  
  virtual void drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
  {
  }
  virtual void drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
  {
  }
  virtual void drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2)
  {
  }

  virtual void renderText (const AGRect &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn)
  {
  }

  
  virtual void putPixel(int x,int y,const AGColor &c)
  {
  }
  virtual AGColor getPixel(int x,int y) const
  {
    return AGColor();
  }
};


#endif

