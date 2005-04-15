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
#include "ag_color.h"
#include "ag_debug.h"
#include "privates.h"
#include "sge.h"

#include "SDL_image.h"

#include <sstream>

#include <ag_fs.h>

SDL_Surface *AGCreate32BitSurface(size_t width,size_t height)
{
  /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
     as expected by OpenGL for textures */
  SDL_Surface *surface;
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

  surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
                                 rmask, gmask, bmask, amask);
  if(!surface)
    {
      std::cerr<<"CreateRGBSurface failed: "<<SDL_GetError()<<std::endl;
      //        throw UI::UIException();
    }
  return surface;
}





AGSurface::AGSurface(SDL_Surface *pS,int W,int H):s(pS)
{
  //  CTRACE;
}

AGSurface::AGSurface(SDL_Surface *pS):s(pS)
{
  //  CTRACE;
}

AGSurface::AGSurface()
{
  s=0;
  //    CTRACE;
}


AGSurface::AGSurface(const AGSurface &p)
{
  if(p.valid())
    {
      s=AGCreate32BitSurface(p.width(),p.height());//sge_CreateAlphaSurface(SDL_SWSURFACE,p.width(),p.height());
      assert(s);
      for(int x=0;x<p.width();x++)
	for(int y=0;y<p.height();y++)
	  sge_PutPixel(s,x,y,sge_GetPixel(p.s,x,y));
      //      SDL_BlitSurface(p.s,0,s,0);
    }
  else
    s=0;
  
  // FIXME: this gets called too often!!!
  //  CTRACE;
}

AGSurface::AGSurface(int w,int h)
{
  s=AGCreate32BitSurface(w,h);
      //  s=sge_CreateAlphaSurface(SDL_SWSURFACE,w,h);
  //  CTRACE;
}

AGSurface::~AGSurface()
{
  SDL_FreeSurface(s);
}

AGSurface &AGSurface::operator=(const AGSurface &p)
{
  CTRACE;
  if(s)
    SDL_FreeSurface(s);
  s=0;
  if(p.valid())
    {

      if(true)
	{
      //      s=sge_CreateAlphaSurface(SDL_SWSURFACE,p.width(),p.height());
      s=AGCreate32BitSurface(p.width(),p.height());
      
      Uint8 r,g,b,a;
      Uint32 c;
      for(int x=0;x<p.width();x++)
	for(int y=0;y<p.height();y++)
	  {
	    c=sge_GetPixel(p.s,x,y);
	    SDL_GetRGBA(c,p.s->format,&r,&g,&b,&a);

	    sge_PutPixel(s,x,y,SDL_MapRGBA(s->format,r,g,b,a));
	  }
      //      SDL_BlitSurface(p.s,0,s,0);
	}
      else
	{
	  //s=sge_CreateAlphaSurface(SDL_SWSURFACE,p.width(),p.height());
	  s=AGCreate32BitSurface(p.width(),p.height());
	  SDL_BlitSurface(p.s,0,s,0);
	  SDL_Flip(s);
	}
    }
  return *this;
}


/*
void AGSurface::putPixel(int x,int y,const AGColor &c)
{
  sge_PutPixel(s,x,y,color(c));
}*/



SDL_Surface *AGSurface::surface()
{
  CHECK_ZERO(s);
  return s;
}

 //int k=0;

AGSurface AGSurface::getSubSurface(const AGRect &sr) const
{
  CHECK_ZERO(s);
  CTRACE;
  SDL_Surface *ns=getScreen().newSurface(sr.w,sr.h);//sge_CreateAlphaSurface(SDL_SWSURFACE,sr.w,sr.h);

  AGRect dr(0,0,sr.w,sr.h);
  Uint32 c;
  Uint8 r,g,b,a;

  SDL_SaveBMP(s,"sub1.bmp");

  cdebug(sr);
  
  for(int x=0;x<sr.w;x++)
    for(int y=0;y<sr.h;y++)
      {
	c=sge_GetPixel(s,x+sr.x,y+sr.y);
	SDL_GetRGBA(c,s->format,&r,&g,&b,&a);
	c=SDL_MapRGBA(ns->format,r,g,b,a);
	
	sge_PutPixel(ns,x,y,c);
	}
  //  SDL_BlitSurface(s,const_cast<AGRect*>(&sr),ns,&dr);

  SDL_SaveBMP(ns,"sub2.bmp");

  return AGSurface(ns,sr.w,sr.h);
}

 /*
void AGSurface::toScreen(int x,int y)
{
  if(s==ScreenSurface)
    {
      std::cerr<<"Cannot blit from screen to screen!"<<std::endl;
      return;
    }
  SDL_Rect sr,dr;
  sr.x=sr.y=0;
  dr.w=sr.w=s->w;
  dr.h=sr.h=s->h;
  dr.x=x;
  dr.y=y;
  SDL_BlitSurface(s,&sr,ScreenSurface,&dr);
}
 */
  /*
AGSurface getScreen()
{
  return AGSurface(ScreenSurface);
  }*/

   /*
AGSurface loadAGSurface(const std::string &pFilename)
{
  return AGSurface(IMG_Load(pFilename.c_str()));
  }*/

   
Uint32 AGSurface::color(const AGColor &c) const
{
  CHECK_ZERO(s);
  return c.mapRGB(s->format);
}

AGRect AGSurface::getRect() const
{
  CHECK_ZERO(s);
  if(s)
    return AGRect(0,0,s->w,s->h);
  else
    return AGRect(0,0,0,0);
}


int AGSurface::width() const
{
  CHECK_ZERO(s);
  return s->w;
}
int AGSurface::height() const
{
  CHECK_ZERO(s);
  return s->h;
}


////////////////////////////////////////////////////////////////////////
// AGScreen
////////////////////////////////////////////////////////////////////////

AGSDLScreen::AGSDLScreen(SDL_Surface *S):s(S)
{
}





void AGSDLScreen::flip()
{
  SDL_Flip(s);
}

AGRect AGSDLScreen::getRect() const
{
  return AGRect(0,0,s->w,s->h);
}

void AGSDLScreen::drawRect(const AGRect &pRect,const AGColor &c)
{
  sge_FilledRectAlpha(s,pRect.x,pRect.y,pRect.x+pRect.w-1,pRect.y+pRect.h-1,c.mapRGB(s->format),c.a);
}

void AGSDLScreen::blit(const AGTexture &pSource,const AGRect &pDest)
{
  //cdebug(pDest.toString()<<endl);
  SDL_Rect sr;
  sr.x=0;
  sr.y=0;
  sr.w=pDest.w;
  sr.h=pDest.h;
  //  SDL_BlitSurface(s,const_cast<AGRect*>(&pDest),pSource.s,&sr);
  SDL_BlitSurface(pSource.s,&sr,s,const_cast<AGRect*>(&pDest));
}

void AGSDLScreen::tile(const AGTexture &pSource)
{
  tile(pSource,getRect());
  /*
  int x,y;
  if(pSource.s->w==0 || pSource.s->h==0)
    return;
  for(y=0;y<s->h;y+=pSource.s->h)
    for(x=0;x<s->w;x+=pSource.s->w)
      {
	SDL_Rect sr,dr;
	sr.x=sr.y=0;
	sr.w=dr.w=std::min(pSource.s->w,s->w-x);
	sr.w=dr.w=std::min(pSource.s->h,s->h-y);
	dr.x=x;
	dr.y=y;
	SDL_BlitSurface(pSource.s,&sr,s,&dr);
      }
  */
}

void AGSDLScreen::tile(const AGTexture &pSource,const AGRect &pDest)
{

  //  sge_FilledRect(s,pDest.x,pDest.y,pDest.x+pDest.w-1,pDest.y+pDest.h-1,0);
  //  return;
  int x,y;
  if(pSource.s->w==0 || pSource.s->h==0)
    return;
  for(y=pDest.y;y<pDest.y+pDest.h;y+=pSource.s->h)
    for(x=pDest.x;x<pDest.x+pDest.w;x+=pSource.s->w)
      {
	SDL_Rect sr,dr;
	sr.x=sr.y=0;
	sr.w=dr.w=std::min(pSource.s->w,pDest.w-(x-pDest.x));
	sr.h=dr.h=std::min(pSource.s->h,pDest.h-(y-pDest.y));
	dr.x=x;
	dr.y=y;
	SDL_BlitSurface(pSource.s,&sr,s,&dr);
      }
    
}


void AGSDLScreen::tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  int x,y;
  if(pSource.s->w==0 || pSource.s->h==0 || pSrc.w==0 || pSrc.h==0)
    return;

  if(pSrc.w>pSource.s->w || pSrc.h>pSource.s->h || pSrc.x+pSrc.w-1>pSource.s->w || pSrc.y+pSrc.h-1>pSource.s->h)
    {
      cdebug("wrong source-rect");
    }

  for(y=pDest.y;y<pDest.y+pDest.h;y+=pSrc.h)
    for(x=pDest.x;x<pDest.x+pDest.w;x+=pSrc.w)
      {
	SDL_Rect sr,dr;
	sr.x=pSrc.x;
	sr.y=pSrc.y;
	sr.w=dr.w=std::min(int(pSrc.w),pDest.w-(x-pDest.x));
	sr.h=dr.h=std::min(int(pSrc.h),pDest.h-(y-pDest.y));
	dr.x=x;
	dr.y=y;
	//	cdebug(sr<<":"<<dr);
	SDL_BlitSurface(pSource.s,&sr,s,&dr);
      }
  
}

AGGScreen *mAGGScreen=0;

AGGScreen &getScreen()
{
  return *mAGGScreen;
}

void setScreen(AGGScreen *s)
{
  mAGGScreen=s;
}


void AGSDLScreen::drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2)
{
}
void AGSDLScreen::putPixel(int x,int y,const AGColor &c)
{
}

SDL_Surface *AGSDLScreen::newSurface(int x,int y)
{
  return AGCreate32BitSurface(x,y);
  //  return sge_CreateAlphaSurface(SDL_SWSURFACE,x,y);
}


AGSurface AGSDLScreen::loadSurface(const std::string &pFilename)
{
  std::string file=loadFile(pFilename);
  

  //  CTRACE;
  //SDL_Surface *s=IMG_Load(pFilename.c_str());
  SDL_Surface *s=IMG_Load_RW(SDL_RWFromMem(const_cast<char*>(file.c_str()),file.length()),false);
  if(!s)
    cdebug(pFilename);
  assert(s);
  //  SDL_Surface *s=IMG_Load(pFilename.c_str());
  return AGSurface(s,s->w,s->h);
}

AGTexture AGSDLScreen::displayFormat(SDL_Surface *s)
{
  return AGTexture(SDL_DisplayFormatAlpha(s),s->w,s->h);
}



/////////////////////////////////////////////////////////////////////////
// AGGSurface
/////////////////////////////////////////////////////////////////////////

void AGGScreen::blit(const AGTexture &pSource,const AGRect &pDest)
{
}
void AGGScreen::tile(const AGTexture &pSource)
{
}
void AGGScreen::tile(const AGTexture &pSource,const AGRect &pDest)
{
}
void AGGScreen::tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
}

void AGGScreen::flip()
{
}

void AGGScreen::drawRect(const AGRect &pRect,const AGColor &c)
{
}

AGRect AGGScreen::getRect() const
{
  return AGRect(0,0,0,0);
}

void AGGScreen::drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
}
void AGGScreen::drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2)
{
}


void AGGScreen::putPixel(int x,int y,const AGColor &c)
{
}

SDL_Surface *AGGScreen::newSurface(int x,int y)
{
  return 0;
}

AGTexture AGGScreen::displayFormat(SDL_Surface *s)
{
  return AGTexture();
}
  
AGSurface AGGScreen::loadSurface(const std::string &pFilename)
{
  return AGSurface();
}

AGTexture AGGScreen::makeTexture(AGSurface &s)
{
  STUB;
  return AGTexture();
}
