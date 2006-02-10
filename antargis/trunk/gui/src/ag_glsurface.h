/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_glsurface.h
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

#ifndef __GUI_GLSURFACE_H
#define __GUI_GLSURFACE_H

#include <string>
#include <map>
#include <list>
#include <SDL.h>
#include "ag_geometry.h"
#include "ag_screen.h"
#include <GL/gl.h>

#define NEW_TEXTURES

class AGColor;

typedef GLuint TextureID;

class AGGLScreen:public AGScreen
{
 public:
  AGGLScreen(int W,int H);

  virtual ~AGGLScreen();

  virtual void blitTri(const AGTexture &pSource,const AGTriangle2 &pSrc,const AGTriangle2 &pDest);

  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);
  virtual void blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc,const AGColor &pColor);

  //  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);

  virtual void fillRect(const AGRect &pRect,const AGColor &c);
  virtual void drawLine(const AGPoint &p0,const AGPoint &p1,const AGColor &c);

  virtual AGRect getRect() const;

  virtual void drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  //  void renderText (const AGRect &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn);

  virtual void drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  virtual void drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2);
  virtual void putPixel(int x,int y,const AGColor &c);
  /*
  virtual AGTexture makeTexture(const AGSurface &s);
  virtual AGTexture makeTexture3D(const AGSurface &s);
  virtual void deleteTexture(AGTexture &t);
  */
  virtual void begin(); // call before start drawing
  void flip();
  bool inScreen(const AGRect &r) const;
  //  bool inScreen(const AGRect2 &r) const;

  virtual size_t getWidth() const;
  virtual size_t getHeight() const;

  void clip(const AGRect &r);
  void unclip();

 private:
  AGRect getRect(SDL_Surface *s);
  void checkUnusedTextures();

  int w,h;

  friend class AGTexture;
};

bool opengl();

#endif
