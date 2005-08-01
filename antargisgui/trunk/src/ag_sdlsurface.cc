/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_sdlsurface.cc
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

#include "ag_sdlsurface.h"
#include "ag_debug.h"
#include "ag_fs.h"

#include "SDL_image.h"
#include <sge.h>

SDL_Surface *AGCreate32BitSurface(size_t width,size_t height);


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

AGScreen *mAGGScreen=0;

AGScreen &getScreen()
{
  return *mAGGScreen;
}

void setScreen(AGScreen *s)
{
  mAGGScreen=s;
}


void AGSDLScreen::drawBorder(const AGRect& rect,int W, const AGColor& c1, const AGColor& c2)
{
}
void AGSDLScreen::putPixel(int x,int y,const AGColor &c)
{
  sge_Update_OFF();

  sge_PutPixel(s,x,y,c.mapRGB(s->format));
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
  return AGSurface(s);//,s->w,s->h);
}

AGTexture AGSDLScreen::displayFormat(SDL_Surface *s)
{
  return AGTexture(SDL_DisplayFormatAlpha(s),s->w,s->h);
}


void AGSDLScreen::drawGradientAlpha(const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  STUB;
}

void AGSDLScreen::drawLine(const AGPoint &pp0,const AGPoint &pp1,const AGColor &c)
{
  AGPoint p0,p1;

  p0=pp0;
  p1=pp1;

  int dx=p1.x-p0.x;
  int dy=p1.y-p0.y;

  sge_Update_OFF();
  if(abs(dx)>abs(dy))
    {
      if(dx<0)
	{
	  p0=pp1;
	  p1=pp0;
	}

      dx=p1.x-p0.x;
      dy=p1.y-p0.y;

      int y=p0.y;
      float e=0;
      float de=((float)dy)/dx;
      for(int x=p0.x;x<=p1.x;x++)
	{
	  sge_PutPixel(s,x,y,c.mapRGB(s->format));
	  e+=de;
	  if(e>0.5)
	    {
	      e-=1.0;
	      y++;
	    }
	  else if(e<-0.5)
	    {
	      e+=1.0;
	      y--;
	    }
	}
    }
  else
    {
      if(dy<0)
	{
	  p0=pp1;
	  p1=pp0;
	}

      dx=p1.x-p0.x;
      dy=p1.y-p0.y;

      int x=p0.x;
      float e=0;
      float de=((float)dx)/dy;
      for(int y=p0.y;y<=p1.y;y++)
	{
	  sge_PutPixel(s,x,y,c.mapRGB(s->format));
	  e+=de;
	  if(e>0.5)
	    {
	      e-=1.0;
	      x++;
	    }
	  else if(e<-0.5)
	    {
	      e+=1.0;
	      x--;
	    }
	}
    }
}

AGTexture AGSDLScreen::makeTexture(const AGSurface &s)
{
  return AGTexture(const_cast<AGSurface&>(s).surface(),s.width(),s.height());
}
