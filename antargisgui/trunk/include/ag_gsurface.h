#ifndef __GUI_GSURFACE_H
#define __GUI_GSURFACE_H

#include <string>
#include <SDL.h>
#include "ag_geometry.h"

class AGColor;

extern bool mGLMode;

// Generic classes - for OpenGL and "normal" SDL
class AGSurface
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

class AGTexture
{
 public:
  AGTexture();
  explicit AGTexture(const AGSurface &pSurface);
  explicit AGTexture(const AGSurface &pSurface,int W,int H);

  int width() const;
  int height() const;

  void testSave() const;

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

class AGPainter
{
 public:
  virtual void putPixel(int x,int y,const AGColor &c);
  virtual AGColor getPixel(int x,int y);
};

class AGSurfacePainter:public AGPainter
{
 public:
  AGSurfacePainter(AGSurface &pSurface);

  virtual void putPixel(int x,int y,const AGColor &c);
  virtual AGColor getPixel(int x,int y);

  virtual void blit(const AGSurface &pSource,const AGRect &pDest);
  virtual void blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc);
  virtual void tile(const AGSurface &pSource);
  virtual void tile(const AGSurface &pSource,const AGRect &pDest);
  virtual void tile(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc);

 private:
  AGSurface &mSurface;
};


class AGTexturePainter:public AGPainter
{
 public:
  AGTexturePainter(AGTexture &pSurface);

  virtual void putPixel(int x,int y,const AGColor &c);
  virtual AGColor getPixel(int x,int y);

  virtual void blit(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);

 private:
  AGTexture &mSurface;
};

class AGGScreen
{
 public:
  virtual void blit(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);

  virtual void flip();

  virtual void drawRect(const AGRect &pRect,const AGColor &c);
  
  virtual AGRect getRect() const;
  
  virtual void drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  virtual void drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2);
  
  virtual void putPixel(int x,int y,const AGColor &c);
  
  virtual SDL_Surface *newSurface(int x,int y);

  virtual AGTexture displayFormat(SDL_Surface *s);
  
  virtual AGSurface loadSurface(const std::string &pFilename);

  virtual AGTexture makeTexture(AGSurface &s);
};

AGGScreen &getScreen();
void setScreen(AGGScreen *s);


class AGSurfaceCache
{
 public:
  AGSurfaceCache();

  AGSurface getSurface(const std::string &pFilename);
};

AGSurfaceCache *surfaceCache();

#endif
