/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_surface.cc
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

#include <iostream>

#include "ag_surface.h"
#include "ag_texture.h"
#include "ag_color.h"
#include "ag_debug.h"
#include "ag_draw.h"
#include "privates.h"
#include "sge.h"
#include "ag_kill.h"
#include "ag_png.h"
#include "ag_glsurface.h"
#include "ag_surfacemanager.h"

#include "SDL_image.h"

#include <sstream>

#include <ag_fs.h>

///////////////////////////////////////////////////////////////////////
// Tools
///////////////////////////////////////////////////////////////////////

bool gRendering=false;
void beginRender()
{
  gRendering=true;
}
void endRender()
{
  gRendering=false;
}

bool isRendering()
{
  return gRendering;
}


void AGFreeSurface(SDL_Surface *s)
{
  cdebug(s);
  SDL_FreeSurface(s);
}

AGInternalSurface *AGCreate32BitSurface(size_t width,size_t height)
{
  /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
     as expected by OpenGL for textures */
  AGInternalSurface *surface=new AGInternalSurface;
  Uint32 rmask, gmask, bmask, amask;

  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN

  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else

  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  surface->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
					  rmask, gmask, bmask, amask);
  if(!surface->surface)
    {
      std::cerr<<"CreateRGBSurface failed: "<<SDL_GetError()<<std::endl;
      //        throw UI::UIException();
    }

  cdebug("CREATE:"<<surface->surface);

  return surface;
}



///////////////////////////////////////////////////////////////////////
// AGInternalSurface
///////////////////////////////////////////////////////////////////////

AGInternalSurface::AGInternalSurface()
{
  sdlTexture=0;
  glTexture=0;
  surface=0;
  getSurfaceManager()->registerMe(this);
}

AGInternalSurface::~AGInternalSurface()
{
  getSurfaceManager()->deregisterMe(this);
}



///////////////////////////////////////////////////////////////////////
// AGSurface
///////////////////////////////////////////////////////////////////////

AGSurface::AGSurface(AGInternalSurface *i):s(i)
{
  getSurfaceManager()->registerMe(this);
}


AGSurface::AGSurface():
  s(0)
{
  getSurfaceManager()->registerMe(this);
}
AGSurface::AGSurface(int w,int h):
  s(AGCreate32BitSurface(w,h))
{
  getSurfaceManager()->registerMe(this);
}
AGSurface::AGSurface(const AGSurface &p):
  s(p.s)
{
  getSurfaceManager()->registerMe(this);
}

bool AGSurface::valid() const
{
  return s && s->surface;
}

AGSurface AGSurface::copy() const
{
  AGInternalSurface *is=0;
  if(valid())
    {
      is=AGCreate32BitSurface(s->surface->w,s->surface->h);

      // copy pixel for pixel - as blitting doesn't seem to work
      for(int x=0;x<width();x++)
	for(int y=0;y<height();y++)
	  {
	    Uint32 c;
	    Uint8 r,g,b,a;
	    c=sge_GetPixel(s->surface,x,y);
	    SDL_GetRGBA(c,s->surface->format,&r,&g,&b,&a);

	    sge_PutPixel(is->surface,x,y,SDL_MapRGBA(is->surface->format,r,g,b,a));
	  }
    }
  AGSurface ns;
  ns.s=is;
  return ns;
}

AGSurface::~AGSurface()
{
  if(getSurfaceManager())
    getSurfaceManager()->deregisterMe(this);
}

void AGSurface::save(const std::string &pName) const
{
  assert(s);
  assert(s->surface);
  std::string png=toPNG(s->surface);
  saveFile(pName,png);
}


AGSurface &AGSurface::operator=(const AGSurface &p)
{
  s=p.s;
  return *this;
}


void AGSurface::putPixel(int x,int y,const AGColor &c)
{
  sge_PutPixel(s->surface,x,y,color(c));
}

AGColor AGSurface::getPixel(int x,int y) const
{
  return AGColor(sge_GetPixel(s->surface,x,y),*this);
}



AGSurface AGSurface::getSubSurface(const AGRect &sr) const
{
  CHECK_ZERO(s);
  CTRACE;

  AGInternalSurface *ns=AGCreate32BitSurface((int)sr.w(),(int)sr.h());

  AGRect dr(sr.origin());
  Uint32 c;
  Uint8 r,g,b,a;

  // copy 
  for(int x=0;x<sr.w();x++)
    for(int y=0;y<sr.h();y++)
      {
	c=sge_GetPixel(s->surface,(int)(x+sr.x()),(int)(y+sr.y()));
	SDL_GetRGBA(c,s->surface->format,&r,&g,&b,&a);
	c=SDL_MapRGBA(ns->surface->format,r,g,b,a);
	
	sge_PutPixel(ns->surface,x,y,c);
      }
  AGSurface nas;
  nas.s=ns;
  return nas;
}

   
Uint32 AGSurface::color(const AGColor &c) const
{
  CHECK_ZERO(s);
  return c.mapRGB(s->surface->format);
}

AGRect AGSurface::getRect() const
{
  CHECK_ZERO(s);
  if(s)
    return AGRect(0,0,s->surface->w,s->surface->h);
  else
    return AGRect(0,0,0,0);
}


int AGSurface::width() const
{
  CHECK_ZERO(s);
  return s->surface->w;
}
int AGSurface::height() const
{
  CHECK_ZERO(s);
  return s->surface->h;
}

void AGSurface::drawGradient(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  CHECK_ZERO(s);
  AGDrawGradientAlpha(s->surface,rect,ul,ur,dl,dr);
}

void AGSurface::drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  CHECK_ZERO(s);
  AGDrawGradientAlpha(s->surface,rect,ul,ur,dl,dr);
}
void AGSurface::drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2)
{
  CHECK_ZERO(s);
  AGDrawBorder(s->surface,rect,W,c1,c2);
}

void AGSurface::blit(const AGSurface &pSource,const AGRect &pDest,const AGRect &pSrc,const AGColor &pColor)
{
  int x,y,sx,sy,dx,dy;
  Uint32 c;
  Uint8 r,g,b,a;
  SDL_Surface *ss=const_cast<SDL_Surface*>(pSource.s->surface);
  SDL_Surface *ds=s->surface;

  for(y=0,sy=(int)pSrc.y(), dy=(int)pDest.y();y<pSrc.h() ; y++,sy++,dy++)
    for(x=0,sx=(int)pSrc.x(), dx=(int)pDest.x();x<pSrc.w() ; x++,sx++,dx++)
      {
	c=sge_GetPixel(ss,sx,sy);

	SDL_GetRGBA(c,ss->format,&r,&g,&b,&a);
	c=SDL_MapRGBA(ds->format,r,g,b,a);

	sge_PutPixel(ds,dx,dy,c);

      }
}

AGInternalSurface *AGSurface::surface() const
{
  return s;
}

AGSurface AGSurface::load(const std::string &pFilename)
{
  AGSurface n;
  n.s=new AGInternalSurface;
  std::string file=loadFile(pFilename);

  SDL_Surface *s=IMG_Load_RW(SDL_RWFromMem(const_cast<char*>(file.c_str()),file.length()),false);
  n.s->surface=s;
  return n;
}




