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
#include <SDL_opengl.h>
//#include <GL/gl.h>
//#include <GL/glext.h>
#include <GL/glu.h>
#include "ag_geometry.h"
#include "ag_painttarget.h"

class AGColor;
class AGSurface;

//extern bool mGLMode;


// Generic classes - for OpenGL and "normal" SDL
class AGSurface:public AGPaintTarget
{
 public:  
  AGSurface();
  AGSurface(int w,int h);
  AGSurface(const AGSurface &p);

  virtual ~AGSurface();

  int width() const;
  int height() const;

  SDL_Surface *surface();
  AGRect getRect() const;

  AGSurface getSubSurface(const AGRect &r) const;

  AGSurface *clone() const;

  bool valid() const;

  AGSurface &operator=(const AGSurface &p);

  virtual void blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc);
  virtual void blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc,const AGColor &pColor);

  void drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  void drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  void drawBorder(const AGRect& rect,int width, const AGColor& c1, const AGColor& c2);

  virtual void putPixel(int x,int y,const AGColor &c);
  virtual AGColor getPixel(int x,int y) const;

 private:

  // used by AGDraw
  Uint32 color(const AGColor &c) const;

  //  explicit AGSurface(SDL_Surface *s,int w,int h);
  explicit AGSurface(SDL_Surface *s);

  SDL_Surface *s;

  friend class AGSDLScreen;
  friend class AGGLScreen;
  friend class AGFontEngine; // needed because TTF_* function return Surfaces
  friend class AGDraw;
  friend class AGColor;
  friend class AGSurfacePainter;
  friend class AGTexture;
  friend class AGSurfaceManager;
};

// FIXME: make AGTexture a AGPaintTarget, too

class AGTexture
{
 public:
  AGTexture();
  AGTexture(const AGTexture &t);
  //  explicit AGTexture(const AGSurface &pSurface);
  //  explicit AGTexture(const AGSurface &pSurface,int W,int H);
  ~AGTexture();

  int width() const;
  int height() const;

  void testSave() const;

  AGRect getRect() const;

  AGColor getPixel(int x,int y) const;

  float getTW() const;
  float getTH() const;

  bool hasTexture() const;
  bool textureUsed() const;
  void clearTexture();
  void clearTextureUsed();
  void setTextureID(GLuint id);
  GLuint getTextureID();

  SDL_Surface *surface();

 private:

  void init();

  GLuint mTextureID;
  bool mHasTexture;
  bool mTextureUsed;

  AGTexture(SDL_Surface *s,int W,int H);
  SDL_Surface *s;
  int w,h;
  float mTW,mTH;
  friend class AGTexturePainter;
  friend class AGSDLScreen;
  friend class AGGLScreen;
};

// this is virtually a pure virtual class ;-)
// because swig directors have problems with this otherwise

class AGScreen:public AGPaintTarget
{
 public:
  virtual void begin(); // call before start drawing
  virtual void flip();

  virtual size_t getWidth() const;
  virtual size_t getHeight() const;

 private:
  virtual AGTexture makeTexture(const AGSurface &s);
  virtual void deleteTexture(AGTexture &t);

  friend class AGTextureManager;
};

AGScreen &getScreen();
void setScreen(AGScreen *s);

class AGSurfaceManager
{
 public:
  ~AGSurfaceManager();
  AGSurface loadSurface(const std::string &pFilename);
  AGSurface fromSDL(SDL_Surface *s); // eats s

  // AGSurfaces are not owned - but SDL_Surfaces - so beware!!
  void registerSurface(AGSurface *pSurface);
  void deregisterSurface(AGSurface *pSurface);

  void cleanup();

  void enableGC();
  void disableGC();
 private:
  AGSurfaceManager();

  std::set<AGSurface*> mSurfaces;
  std::set<SDL_Surface*> mRealSurfaces;
  friend AGSurfaceManager *getSurfaceManager();

  bool mAutoGC;

};
AGSurfaceManager *getSurfaceManager();



class AGTextureManager
{
 public:
  ~AGTextureManager();

  void registerTexture(AGTexture *pTexture);
  void deregisterTexture(AGTexture *pTexture);

  // cleanup deleted all unreferenced Surfaces
  void cleanup();

  // checkUnused deletes Textures from GL, which weren't used in last frame
  void checkUnused();

  AGTexture makeTexture(const AGSurface &s);

 private:
  AGTextureManager();
#ifdef SLOW_TEXTUREMANAGER
  std::set<AGTexture*> mTextures;
#else
  std::list<AGTexture*> mTextures;
  std::list<AGTexture*> mDelTextures;
#endif



  std::set<GLuint> mTextureIDs;
  std::set<SDL_Surface*> mSurfaces;
  friend AGTextureManager *getTextureManager();
};
AGTextureManager *getTextureManager();

//void setTrap();

#endif
