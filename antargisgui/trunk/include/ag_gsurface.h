/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_gsurface.h
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

#ifndef __GUI_GSURFACE_H
#define __GUI_GSURFACE_H

#include <string>
#include <SDL.h>
#include "ag_geometry.h"

class AGColor;

extern bool mGLMode;

// Generic classes - for OpenGL and "normal" SDL
class AGSurface:public AGPaintTarget
{
 public:  
  explicit AGSurface(SDL_Surface *s,int w,int h);
  explicit AGSurface(SDL_Surface *s);

  AGSurface();
  AGSurface(int w,int h);
  AGSurface(const AGSurface &p);

  virtual ~AGSurface();

  int width() const;
  int height() const;

  SDL_Surface *surface();
  AGRect getRect() const;

  AGSurface getSubSurface(const AGRect &r) const;

  bool valid() const
  {
    return s;
  }

  AGSurface &operator=(const AGSurface &p);

 protected:

  // used by AGDraw
  Uint32 color(const AGColor &c) const;


  SDL_Surface *s;
  //  int w,h; // original size - as surface always gets converted to (2^n)*(2^n) in gl mode
  //  bool mChanged;
  friend class AGSDLScreen;
  friend class AGGLScreen;
  friend class AGFontEngine;
  friend class AGDraw;
  friend class AGColor;
  friend class AGSurfacePainter;
  friend class AGTexture;
};

// FIXME: make AGTexture a AGPaintTarget, too

class AGTexture
{
 public:
  AGTexture();
  explicit AGTexture(const AGSurface &pSurface);
  explicit AGTexture(const AGSurface &pSurface,int W,int H);

  int width() const;
  int height() const;

  void testSave() const;

  AGRect getRect() const;

  virtual AGColor getPixel(int x,int y) const;

 private:

  AGTexture(SDL_Surface *s,int W,int H);
  SDL_Surface *s;
  int w,h;
  friend class AGTexturePainter;
  friend class AGSDLScreen;
  friend class AGGLScreen;
};

// this is virtually a pure virtual class ;-)
// because swig directors have problems with this otherwise

class AGScreen:public AGPaintTarget
{
 public:
  virtual void flip();

  virtual SDL_Surface *newSurface(int x,int y);

  virtual AGTexture displayFormat(SDL_Surface *s);
  
  virtual AGSurface loadSurface(const std::string &pFilename);

  virtual AGTexture makeTexture(AGSurface &s);
};

AGScreen &getScreen();
void setScreen(AGScreen *s);

class AGSurfaceCache
{
 public:
  AGSurfaceCache();

  AGSurface getSurface(const std::string &pFilename);
};

AGSurfaceCache *surfaceCache();

#endif
