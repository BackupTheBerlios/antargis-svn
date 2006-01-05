/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_sdlsurface.h
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

#ifndef __GUI_SDLSURFACE_H
#define __GUI_SDLSURFACE_H

#include "ag_surface.h"

class AGSDLScreen:public AGScreen
{
 public:
  AGSDLScreen(SDL_Surface *s);

  virtual void blit(const AGTexture &pSource,const AGRect &pDest);
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);
  virtual void tile(const AGTexture &pSource);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);

  virtual void flip();

  virtual AGRect getRect() const;

  virtual void drawRect(const AGRect &pRect,const AGColor &c);
  virtual void drawLine(const AGPoint &p0,const AGPoint &p1,const AGColor &c);

  virtual void drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  virtual void drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  virtual void renderText (const AGRect &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn);

  virtual void drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2);

  virtual void putPixel(int x,int y,const AGColor &c);

  virtual SDL_Surface *newSurface(int x,int y);

  virtual AGSurface loadSurface(const std::string &pFilename);

  virtual AGTexture displayFormat(SDL_Surface *s);

  virtual AGTexture makeTexture(const AGSurface &s);

  virtual size_t getWidth() const;
  virtual size_t getHeight() const;
 private:
  SDL_Surface *s;
 public:
};

#endif
