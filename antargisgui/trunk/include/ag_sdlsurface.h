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

#ifndef __GUI_SURFACE_H
#define __GUI_SURFACE_H

#include <string>
#include <SDL.h>
#include "ag_geometry.h"

class AGColor;

class AGSurface
{
 public:
  AGSurface();
  AGSurface(int w,int h);
  AGSurface(SDL_Surface *s);

  void toScreen(int x,int y);

  void blit(const AGTexture &pSource,const AGRect &pDest);
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);
  void tile(const AGTexture &pSource);
  void tile(const AGTexture &pSource,const AGRect &pDest);
  void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);
  SDL_Surface *surface();

  Uint32 color(const AGColor &c) const;

  int width() const;
  int height() const;

  AGRect getRect() const;
 private:
  SDL_Surface *s;
};

AGSurface loadAGSurface(const std::string &pFilename);
AGSurface getScreen();

#endif
