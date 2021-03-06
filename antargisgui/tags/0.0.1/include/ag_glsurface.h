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
#include "ag_gsurface.h"
#include <GL/gl.h>

class AGColor;

typedef GLuint TextureID;

class AGGLScreen:public AGGScreen
{
 public:
  AGGLScreen(int W,int H);

  virtual ~AGGLScreen();

  virtual void blit(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);

  virtual void drawRect(const AGRect &pRect,const AGColor &c);

  virtual AGRect getRect() const;

  virtual void drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  virtual void drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2);
  virtual void putPixel(int x,int y,const AGColor &c);

  virtual SDL_Surface *newSurface(int x,int y);

  virtual AGSurface loadSurface(const std::string &pFilename);

  virtual AGTexture displayFormat(SDL_Surface *s);

  // do paint
  void flip();
  bool inScreen(const AGRect &r) const;
 private:

  //  void paintTerrain();

  TextureID getID(SDL_Surface *s);
  AGRect getRect(SDL_Surface *s);
  void checkUnusedTextures();

  int w,h;

  std::map<TextureID,bool> mUsedTextures; // unused textures and count of frames, in which they weren't used
  std::map<TextureID,SDL_Surface*> mTextures;
  std::map<SDL_Surface*,TextureID> mTexturesInv;

  std::map<SDL_Surface*,AGRect> mSurfaceRect;

};

#endif
