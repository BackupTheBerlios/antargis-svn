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

class AGPainter
{
 public:
  AGPainter();
  AGPainter(const AGPainter &p);
  AGPainter(AGPaintTarget &pTarget);
  
  virtual ~AGPainter();

  virtual void putPixel(const AGPoint &p,const AGColor &c);
  virtual AGColor getPixel(int x,int y);

  virtual void blit(const AGTexture &pSource,const AGRect &pDest);
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);
  virtual void tile(const AGTexture &pSource);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);


  virtual void blit(const AGSurface &pSource,const AGRect &pDest);
  virtual void blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc);
  virtual void tile(const AGSurface &pSource);
  virtual void tile(const AGSurface &pSource,const AGRect &pDest);
  virtual void tile(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc);
  
  void transform(const AGRect &r);

  void renderText(const std::string &pText,const AGPoint &p,const AGFont &f);

  AGRect getRect() const;

  void drawGradient(const AGRect &r,const AGColor &c0,const AGColor &c1,const AGColor &c2,const AGColor &c3);
  void drawBorder(const AGRect& rect,int width, const AGColor& c1, const AGColor& c2);
  virtual void drawRect(const AGRect &pRect,const AGColor &c);

 private:

  AGPoint move(const AGPoint &p) const;
  AGRect move(const AGRect &r) const;

  bool inRect(const AGPoint &p) const;
  std::pair<AGRect,AGRect> clip(const AGRect &from,const AGRect &to) const;


  std::list<AGRect> mClips;
  
  AGRect mRect;

  AGPaintTarget &mTarget;
};

/*
class AGSurfacePainter
{
 public:
  AGSurfacePainter(AGPaintTarget<AGSurface> &pSurface);
  virtual ~AGSurfacePainter();

  virtual void putPixel(const AGPoint &p,const AGColor &c);
  //  virtual AGColor getPixel(int x,int y);

  virtual void blit(const AGTexture &pSource,const AGRect &pDest);
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);
  virtual void tile(const AGTexture &pSource);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);
  
  void transform(const AGRect &r);

  void renderText(const std::string &pText,const AGPoint &p,const AGFont &f);

  AGRect getRect() const;

  void drawGradient(const AGRect &r,const AGColor &c0,const AGColor &c1,const AGColor &c2,const AGColor &c3);
  void drawBorder(const AGRect& rect,int width, const AGColor& c1, const AGColor& c2);
  virtual void drawRect(const AGRect &pRect,const AGColor &c);

 private:

  AGPoint move(const AGPoint &p) const;
  AGRect move(const AGRect &r) const;

  bool inRect(const AGPoint &p) const;
  std::pair<AGRect,AGRect> clip(const AGRect &from,const AGRect &to) const;


  std::list<AGRect> mClips;
  
  AGRect mRect;

  AGPaintTarget<AGSurface> &mTarget;
};
*/

#endif
