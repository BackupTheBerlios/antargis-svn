#ifndef __GUI_SURFACE_H
#define __GUI_SURFACE_H

#ifdef USE_RUBY
#include "ruby.h"
#undef connect
#endif
#include "ag_gsurface.h"

class AGSDLScreen:public AGGScreen
{
 public:
  AGSDLScreen(SDL_Surface *s);

  virtual void blit(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest);
  virtual void tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc);

  virtual void flip();

  virtual AGRect getRect() const;

  virtual void drawRect(const AGRect &pRect,const AGColor &c);

  virtual void drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
  {
  }

  virtual void drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2);

  virtual void putPixel(int x,int y,const AGColor &c);

  virtual SDL_Surface *newSurface(int x,int y);

  virtual AGSurface loadSurface(const std::string &pFilename);

  virtual AGTexture displayFormat(SDL_Surface *s);

 private:
  SDL_Surface *s;
 public:
#ifdef USE_RUBY
  VALUE mRUBY;
#endif
};

#endif
