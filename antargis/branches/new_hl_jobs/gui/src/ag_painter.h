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
#include <ag_base.h>


#include <list>

class AGTriangle2;
class AGRect2;

struct AGProjection
{
  AGMatrix3 a;
  AGRect2 clip;

  AGProjection(const AGRect2 &pClip);

  AGVector2 project(const AGVector2 &p) const;
  bool pointOk(const AGVector2 &p) const;

  AGRect2 project(const AGRect2 &p) const;

  std::pair<AGRect2,AGRect2> clipRect(AGRect2 target,AGRect2 src) const;
  AGRect2 clipRect(AGRect2 target) const;


  AGLine2 clipLine(AGLine2 l) const;

  void translate(const AGVector2 &v);
  void setClip(const AGRect2&p);

  AGRect2 getRect() const;
};

#ifdef SWIG
#undef AGEXPORT
#define AGEXPORT
#endif

class AGEXPORT AGPainter
{
 public:
  AGPainter();
  AGPainter(const AGPainter &p);
  AGPainter(AGPaintTarget &pTarget);
  
  ~AGPainter();

  void putPixel(const AGVector2 &p,const AGColor &c);
  AGColor getPixel(int x,int y);

  void drawCircle(const AGVector2 &p,float rad,const AGColor &c);

  void blitTri(const AGTexture &pSource,const AGTriangle2 &pSrc,const AGTriangle2 &pDest);

  void blit(const AGTexture &pSource,const AGRect2 &pDest);
  void blit(const AGTexture &pSource,const AGRect2 &pDest,const AGColor &pColor);
  void blit(const AGTexture &pSource,const AGRect2 &pDest,const AGRect2 &pSrc);
  void blit(const AGTexture &pSource,const AGRect2 &pDest,const AGRect2 &pSrc,const AGColor &pColor);
  void tile(const AGTexture &pSource);
  void tile(const AGTexture &pSource,const AGRect2 &pDest);
  void tile(const AGTexture &pSource,const AGRect2 &pDest,const AGRect2 &pSrc);

  void blit(const AGSurface &pSource,const AGRect2 &pDest);
  void blit(const AGSurface &pSource,const AGRect2 &pDest,const AGRect2 &pSrc);
  void tile(const AGSurface &pSource);
  void tile(const AGSurface &pSource,const AGRect2 &pDest);
  void tile(const AGSurface &pSource,const AGRect2 &pDest,const AGRect2 &pSrc);
  
  void renderText(const AGStringUtf8 &pText,const AGVector2 &p,const AGFont &f);

  AGRect2 getRect() const;

  void drawGradient(const AGRect2 &r,const AGColor &c0,const AGColor &c1,const AGColor &c2,const AGColor &c3);
  void drawBorder(const AGRect2& rect,int width, const AGColor& c1, const AGColor& c2);
  void fillRect(const AGRect2 &pRect,const AGColor &c);
  void drawLine(const AGVector2 &p0,const AGVector2 &p1,const AGColor &c);
  void drawPoint(const AGVector2 &p,const AGColor &c,float size);

  void fillRects(const std::vector<std::pair<AGRect2,AGVector4> > &pRects);

  void pushMatrix();
  void popMatrix();
  void translate(const AGVector2 &v);
  void scale(const AGVector2 &v);
  void clip(const AGRect2 &r);
  void transform(const AGRect2 &r);

  AGVector2 project(const AGVector2 &p) const;
  bool pointOk(const AGVector2 &p) const;

  AGPaintTarget *getTarget();

 private:
  std::list<AGProjection> ps;

  AGProjection mCurrent;

  gc_ptr<AGPaintTarget> mTarget;
};

#endif
