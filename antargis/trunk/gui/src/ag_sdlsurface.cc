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
#include <sge.h>
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
  SDL_BlitSurface(const_cast<AGTexture&>(pSource).sdlTexture()->surface,&sr,s,&dr);
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

/*
void AGSDLScreen::renderText (const AGRect2 &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn)
{
  if(!AGFontEngine::renderText(this,pClipRect,BaseLineX,BaseLineY,pText,ParamIn))
    cdebug("SOME ERROR");

    }*/


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
/*
AGTexture AGSDLScreen::makeTexture(const AGSurface &s)
{
  SDL_Surface *ns=SDL_DisplayFormatAlpha(const_cast<AGSurface&>(s).surface());
  return AGTexture(ns,s.width(),s.height());
  }*/

size_t AGSDLScreen::getWidth() const
{
  return s->w;
}
size_t AGSDLScreen::getHeight() const
{
  return s->h;
}
