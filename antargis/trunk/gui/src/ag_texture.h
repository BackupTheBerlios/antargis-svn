/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_texture.h
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


#ifndef AG_TEXTURE_H
#define AG_TEXTURE_H

#include "ag_surface.h"
#include "ag_gltexture.h"
#include "ag_painttarget.h"

class AGTexture:public AGPaintTarget
{
 public:
  AGTexture();
  AGTexture(const AGSurface &pSurface,bool p3d=false);
  AGTexture(const AGTexture &t);         // shallow copy, uses same tex-id or sdl-surface
  AGTexture(int W,int H);
  ~AGTexture();

  AGTexture copy() const; // returns a real copy

  int width() const;
  int height() const;

  AGRect getRect() const;

  AGColor getPixel(int x,int y) const;

  float getSurfaceWidth() const;
  float getSurfaceHeight() const;

  float getTextureWidth() const;
  float getTextureHeight() const;

  float getTW() const;
  float getTH() const;

  bool hasTexture() const;
  bool textureUsed() const;

  void clearTexture();
  void clearTextureUsed();

  void useTexture();
 
  virtual void beginPaint();
  virtual void endPaint();

  virtual void putPixel(int x,int y,const AGColor &c);
  virtual void fillRect(const AGRect &pRect,const AGColor &c);
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc,const AGColor &pColor);
  virtual void drawLine(const AGPoint &p0,const AGPoint &p1,const AGColor &c);
  virtual void drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  virtual void drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);


  void setWrapping(bool pWrap);
  void setFilter(GLuint mag,GLuint min);
  void setClamp(GLuint s,GLuint t,GLuint r);

  AGGLTexture *glTexture();
  AGInternalSurface *sdlTexture();

  bool is3d() const;

 private:

  void bindTexture();

  AGGLTexture *mTexture;
  bool mTextureUsed;
  bool mPainting;

  bool m3d;
  int w,h;

  AGInternalSurface *s;
  AGInternalSurface *mSDLTexture;
  

  friend class AGTexturePainter;
  friend class AGSDLScreen;
  friend class AGGLScreen;
};


#endif
