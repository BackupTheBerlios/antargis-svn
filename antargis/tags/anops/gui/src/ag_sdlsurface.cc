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
#include "ag_texture.h"
#include "ag_debug.h"
#include "ag_fs.h"
#include "ag_draw.h"
#include "ag_fontengine.h"

#include "SDL_image.h"
#include "ag_sgeexport.h"
#include <math.h>

SDL_Surface *AGCreate32BitSurface(size_t width,size_t height);

AGSDLScreen::AGSDLScreen(SDL_Surface *S):s(S)
{
  sge_Update_OFF();
}

void AGSDLScreen::flip()
{
  SDL_Flip(s);
}

AGRect2 AGSDLScreen::getRect() const
{
  return AGRect2(0,0,s->w,s->h);
}

void AGSDLScreen::fillRect(const AGRect2 &pRect,const AGColor &c)
{
  for(int x=(int)pRect.x0();x<(int)pRect.x1();x++)
    for(int y=(int)pRect.y0();y<(int)pRect.y1();y++)
      putPixel(x,y,c);
  return;
  sge_FilledRectAlpha(s,
		      (int)pRect.x(),
		      (int)pRect.y(),
		      (int)(pRect.x()+pRect.w()-1),
		      (int)(pRect.y()+pRect.h()-1),
		      c.mapRGB(s->format),c.a);
}
void AGSDLScreen::blit(const AGTexture &pSource,const AGRect2 &pDest,const AGRect2 &pSrc)
{
  SDL_Rect sr=pSrc.sdl();
  SDL_Rect dr=pDest.sdl();

  //  cdebug(pDest<<"  "<<pSrc);

  SDL_Surface *source=const_cast<AGTexture&>(pSource).sdlTexture()->surface;

  //  cdebug(source->w<<"  "<<source->h);

  //  cdebug(sr.x<<" "<<sr.y<<" "<<sr.w<<" "<<sr.h);
  //  cdebug(dr.x<<" "<<dr.y<<" "<<dr.w<<" "<<dr.h);

  SDL_BlitSurface(source,&sr,s,&dr);
}



AGScreen *mAGGScreen=0;

bool videoInited()
{
  return mAGGScreen;
}

AGScreen & AGEXPORT getScreen()
{
  assert(mAGGScreen);
  return *mAGGScreen;
}


void setScreen(AGScreen *s)
{
  TRACE;
  cdebug("s:"<<s);
  mAGGScreen=s;
}


void AGSDLScreen::drawBorder(const AGRect2& rect,int W, const AGColor& c1, const AGColor& c2)
{
}
void AGSDLScreen::putPixel(int x,int y,const AGColor &c)
{
  sge_PutPixelAlpha(s,x,y,c.mapRGB(s->format),c.a);
}


void AGSDLScreen::drawGradientAlpha(const AGRect2& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  AGDrawGradientAlpha(s,rect,ul,ur,dl,dr);

}
void AGSDLScreen::drawGradient(const AGRect2& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr)
{
  AGDrawGradient(s,rect,ul,ur,dl,dr);
}

void AGSDLScreen::drawLine(const AGVector2 &pp0,const AGVector2 &pp1,const AGColor &c)
{
  AGVector2 p0,p1;

  p0=pp0;
  p1=pp1;

  float dx=p1[0]-p0[0];
  float dy=p1[1]-p0[1];

  if(fabs(dx)>fabs(dy))
    {
      if(dx<0)
	{
	  p0=pp1;
	  p1=pp0;
	}

      dx=p1[0]-p0[0];
      dy=p1[1]-p0[1];

      float y=p0[1];
      float e=0;
      float de=((float)dy)/dx;
      for(float x=p0[0];x<=p1[0];x++)
	{
	  sge_PutPixel(s,int(x),int(y),c.mapRGB(s->format));
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

      dx=p1[0]-p0[0];
      dy=p1[1]-p0[1];

      float x=p0[0];
      float e=0;
      float de=((float)dx)/dy;
      for(float y=p0[1];y<=p1[1];y++)
	{
	  sge_PutPixel(s,int(x),int(y),c.mapRGB(s->format));
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

size_t AGSDLScreen::getWidth() const
{
  return s->w;
}
size_t AGSDLScreen::getHeight() const
{
  return s->h;
}

void AGSDLScreen::clip(const AGRect2 &r)
{
  //  cdebug(r);
  SDL_Rect sr=r.sdl();
  SDL_SetClipRect(s,&sr);
}
void AGSDLScreen::unclip()
{
  SDL_Rect sr=getRect().sdl();
  SDL_SetClipRect(s,&sr);
}

AGSurface AGSDLScreen::screenshot()
{
  AGInternalSurface *surface=new AGInternalSurface;
  surface->surface=s;
  return AGSurface(surface).copy();
}
/*
extern "C"
BOOL APIENTRY DllMain (HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
	return true;
}
*/
