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
