/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_gsurface.cc
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

#include "ag_gsurface.h"
#include "ag_color.h"
#include "ag_debug.h"

#include <sge.h>

// AGPainter stubs

bool mGLMode=true;

void AGPainter::putPixel(int x,int y,const AGColor &c)
{
  STUB;
}

AGColor AGPainter::getPixel(int x,int y)
{
  return AGColor();
}

// AGSurfacePainter

AGSurfacePainter::AGSurfacePainter(AGSurface &pSurface):mSurface(pSurface)
{
  //  STUB;
}

void AGSurfacePainter::putPixel(int x,int y,const AGColor &c)
{
  sge_PutPixel(mSurface.s,x,y,mSurface.color(c));
}

AGColor AGSurfacePainter::getPixel(int x,int y)
{
  return AGColor(sge_GetPixel(mSurface.s,x,y),mSurface);
}

void AGSurfacePainter::blit(const AGSurface &pSource,const AGRect &pDest)
{
  SDL_Rect sr;
  sr.x=sr.y=0;
  sr.w=pDest.w;
  sr.h=pDest.h;
  SDL_BlitSurface(pSource.s,&sr,mSurface.s,const_cast<AGRect*>(&pDest));
  SDL_Flip(mSurface.s);
}
void AGSurfacePainter::blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  if(mGLMode)
    {
      int x,y;
      AGSurfacePainter o(const_cast<AGSurface&>(pSource));
      for(x=0;x<pSrc.w;x++)
	for(y=0;y<pSrc.h;y++)
	  {
	    putPixel(x+pDest.x,y+pDest.y,o.getPixel(x+pSrc.x,y+pSrc.y));
	  }
    }
  else
    {
      SDL_BlitSurface(pSource.s,const_cast<AGRect*>(&pSrc),mSurface.s,const_cast<AGRect*>(&pDest));
      SDL_Flip(mSurface.s);
    }
}
void AGSurfacePainter::tile(const AGSurface &pSource)
{
  STUB;
}
void AGSurfacePainter::tile(const AGSurface &pSource,const AGRect &pDest)
{
  STUB;
}
void AGSurfacePainter::tile(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  STUB;
}


AGTexturePainter::AGTexturePainter(AGTexture &pSurface):mSurface(pSurface)
{
  STUB;
}

void AGTexturePainter::putPixel(int x,int y,const AGColor &c)
{
  STUB;
}

AGColor AGTexturePainter::getPixel(int x,int y)
{
  STUB;

  return AGColor();
}

void AGTexturePainter::blit(const AGTexture &pSource,const AGRect &pDest)
{
  STUB;
}
void AGTexturePainter::tile(const AGTexture &pSource)
{
  STUB;
}
void AGTexturePainter::tile(const AGTexture &pSource,const AGRect &pDest)
{
  STUB;
}
void AGTexturePainter::tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  STUB;
}


AGTexture::AGTexture()
{
  s=0;
}
AGTexture::AGTexture(const AGSurface &pSurface)
{
  //  CTRACE;
  assert(pSurface.valid());
  *this=getScreen().displayFormat(pSurface.s);
}

AGTexture::AGTexture(const AGSurface &pSurface,int W,int H)
{
  assert(pSurface.valid());
  *this=getScreen().displayFormat(pSurface.s);
  w=W;
  h=H;
}

void AGTexture::testSave() const
{
  SDL_SaveBMP(s,"testSave.bmp");
}


int AGTexture::width() const
{
  return w;
}
int AGTexture::height() const
{
  return h;
}
AGTexture::AGTexture(SDL_Surface *ps,int W,int H):s(ps),w(W),h(H)
{
  //  CTRACE;
  //  cdebug(W<<"//"<<H);
}
AGColor AGTexture::getPixel(int x,int y) const
{
  Uint32 c=sge_GetPixel(s,x,y);
  Uint8 r,g,b,a;
  SDL_GetRGBA(c,s->format,&r,&g,&b,&a);
  return AGColor(r,g,b,a);
}


AGSurfaceCache::AGSurfaceCache()
{
}

AGSurface AGSurfaceCache::getSurface(const std::string &pFilename)
{
}

AGSurfaceCache *mSurfaceCache=0;
AGSurfaceCache *surfaceCache()
{
  if(!mSurfaceCache)
    mSurfaceCache=new AGSurfaceCache;
  return mSurfaceCache;
}
