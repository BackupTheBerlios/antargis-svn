/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_painter.h
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

#ifndef __AG_PAINTER_H
#define __AG_PAINTER_H

#include <ag_color.h>
#include <ag_surface.h>
#include <ag_geometry.h>
#include <ag_font.h>
#include <ag_painttarget.h>


#include <list>

class AGTriangle2;
class AGRect2;

struct AGProjection
{
  AGMatrix3 a;
  AGRect2 clip;

  AGProjection(const AGRect2 &pClip);

  AGPoint project(const AGPoint &p) const;
  bool pointOk(const AGPoint &p) const;

  AGRect project(const AGRect &p) const;
  std::pair<AGRect,AGRect> clipRect(AGRect target,AGRect src) const;
  AGLine2 clipLine(AGLine2 l) const;

  void translate(const AGVector2 &v);
  void setClip(const AGRect2&p);

  AGRect2 getRect() const;
};

class AGPainter
{
 public:
  AGPainter();
  AGPainter(const AGPainter &p);
  AGPainter(AGPaintTarget &pTarget);
  
  ~AGPainter();

  void putPixel(const AGPoint &p,const AGColor &c);
  AGColor getPixel(int x,int y);

  void drawCircle(const AGPoint &p,float rad,const AGColor &c);

  void blitTri(const AGTexture &pSource,const AGTriangle2 &pSrc,const AGTriangle2 &pDest);

  void blit(const AGTexture &pSource,const AGRect &pDest);
  void blit(const AGTexture &pSource,const AGRect &pDest,const AGColor &pColor);
  void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);
  void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc,const AGColor &pColor);
  void tile(const AGTexture &pSource);
  void tile(const AGTexture &pSource,const AGRect &pDest);
  void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);

  void blit(const AGSurface &pSource,const AGRect &pDest);
  void blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc);
  void tile(const AGSurface &pSource);
  void tile(const AGSurface &pSource,const AGRect &pDest);
  void tile(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc);
  
  void renderText(const std::string &pText,const AGPoint &p,const AGFont &f);

  AGRect getRect() const;

  void drawGradient(const AGRect &r,const AGColor &c0,const AGColor &c1,const AGColor &c2,const AGColor &c3);
  void drawBorder(const AGRect& rect,int width, const AGColor& c1, const AGColor& c2);
  void fillRect(const AGRect &pRect,const AGColor &c);
  void drawLine(const AGPoint &p0,const AGPoint &p1,const AGColor &c);

  void pushMatrix();
  void popMatrix();
  void translate(const AGVector2 &v);
  void scale(const AGVector2 &v);
  void clip(const AGRect2 &r);
  void transform(const AGRect2 &r);

  AGPoint project(const AGPoint &p) const;
  bool pointOk(const AGPoint &p) const;
  

 private:
  std::list<AGProjection> ps;

  AGProjection mCurrent;

  AGPaintTarget *mTarget;
};

#endif
