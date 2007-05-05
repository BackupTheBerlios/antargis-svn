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

#ifndef __GUI_SURFACE_H
#define __GUI_SURFACE_H

#include <string>
#include <set>
#include <list>
#include <SDL.h>
#include <GLee.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include "ag_geometry.h"
#include "ag_painttarget.h"
#include "ag_gltexture.h"
#include <ag_plugin.h>

class AGColor;
class AGSurface;
class AGGLTexture;

void beginRender();
void endRender();
bool isRendering();

/**
   AGInternalSurface links plain sdl-surfaces (in main-memory) and textures (both gl-textures and
   sdl-screenlike-surfaces)
*/
struct AGInternalSurface
{
  AGInternalSurface  *sdlTexture;
  AGGLTexture *glTexture;
  SDL_Surface *surface;
  size_t version;

  AGInternalSurface();
  ~AGInternalSurface();
};


/**
   AGDecryptor provides functionaliy for DRM-plugged images and music-files.
   You have to derive from this class and give an instance when loading a surface.
   When you have done so saving is disabled afterwards.
 */
class AGDecryptor
{
 public:
  virtual ~AGDecryptor()
    {
    }
  virtual std::string decrypt(const std::string &s,const std::string &n)
    {
      return s;
    }
};

/// Generic classes - for OpenGL and "normal" SDL
class AGSurface:public AGPaintTarget
{
 public:  
  AGSurface();
  AGSurface(int w,int h);
  AGSurface(const AGSurface &p); // shallow copy only

  virtual ~AGSurface();

  AGSurface copy() const;

  int width() const;
  int height() const;

  AGRect2 getRect() const;

  AGSurface getSubSurface(const AGRect2 &r) const;

  bool valid() const;

  AGSurface scale(int w,int h) const;
  AGSurface grayed() const;

  AGSurface &operator=(const AGSurface &p);

  virtual void blit(const AGSurface &pSource,const AGRect2 &pDest,const AGRect2 &pSrc,const AGColor &pColor);

  void drawGradient(const AGRect2& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  void drawGradientAlpha(const AGRect2& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  void drawBorder(const AGRect2& rect,int width, const AGColor& c1, const AGColor& c2);

  virtual void putPixel(int x,int y,const AGColor &c);
  virtual AGColor getPixel(int x,int y) const;

  virtual void fillRect(const AGRect2 &pr,const AGColor &c);
  virtual void drawLine(const AGVector2 &p0,const AGVector2 &p1,const AGColor &c);


  void save(const std::string &pName) const;

  static AGSurface load(const std::string &pName);
  static AGSurface loadDRM(const std::string &pName);

  AGInternalSurface *surface() const;

  static void setDecryptor(AGDecryptor *pDecryptor);

  AGVector2 shrink2Fit(int alphaThresh=20);

 private:

  AGSurface(AGInternalSurface *i);

  Uint32 color(const AGColor &c) const;

  AGInternalSurface *s;

  friend class AGFontEngine;
  friend class AGSDLScreen;
  size_t mVersion;

  static AGDecryptor *mDecryptor;  
};

void AGFreeSurface(SDL_Surface *s);

#endif
