/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_painter.cc
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

#include "ag_painter.h"
#include "ag_debug.h"
#include "ag_draw.h"
#include "ag_fontengine.h"


AGPainter::AGPainter():mRect(getScreen().getRect())
{
}

AGPainter::AGPainter(const AGPainter &p):mClips(p.mClips),mRect(p.mRect)
{
}

AGPainter::~AGPainter()
{
}

AGPoint AGPainter::move(const AGPoint &p) const
{
  return AGPoint(p.x+mRect.x,p.y+mRect.y);
}

AGRect AGPainter::move(const AGRect &r) const
{
  return AGRect(r.x+mRect.x,r.y+mRect.y,r.w,r.h);
}

bool AGPainter::inRect(const AGPoint &p) const
{
  return mRect.contains(p);
}

std::pair<AGRect,AGRect> AGPainter::clip(const AGRect &from,const AGRect &to) const
{
  //  if(!inRect(to))
  //    return std::make_pair(AGRect(0,0,0,0),AGRect(0,0,0,0)); // fast out

  int tx0=to.x;
  int ty0=to.y;
  int tx1=to.x+to.w-1;
  int ty1=to.y+to.h-1;

  int sx0=from.x;
  int sy0=from.y;
  int sx1=from.x+from.w-1;
  int sy1=from.y+from.h-1;

  int x0=mRect.x;
  int y0=mRect.y;
  int x1=mRect.x+mRect.w-1;
  int y1=mRect.y+mRect.h-1;

  /*  cdebug("("<<x0<<";"<<y0<<"),("<<x1<<","<<y1<<")");
  cdebug("("<<sx0<<";"<<sy0<<"),("<<sx1<<","<<sy1<<")");
  cdebug("("<<tx0<<";"<<ty0<<"),("<<tx1<<","<<ty1<<")");*/
  if(tx0<x0)
    {
      sx0+=x0-tx0;
      tx0=x0;
    }
  if(ty0<y0)
    {
      sy0+=y0-ty0;
      ty0=y0;
    }
  if(tx1>x1)
    {
      sx1-=tx1-x1;
      tx1=x1;
    }
  if(ty1>y1)
    {
      sy1-=ty1-y1;
      ty1=y1;
    }
  /*  cdebug("("<<x0<<";"<<y0<<"),("<<x1<<","<<y1<<")");
  cdebug("("<<sx0<<";"<<sy0<<"),("<<sx1<<","<<sy1<<")");
  cdebug("("<<tx0<<";"<<ty0<<"),("<<tx1<<","<<ty1<<")");*/
  if(tx0>tx1 || ty0>ty1)
    return std::make_pair(AGRect(0,0,0,0),AGRect(0,0,0,0)); // fast out
    
  return std::make_pair(AGRect(sx0,sy0,sx1-sx0+1,sy1-sy0+1),AGRect(tx0,ty0,tx1-tx0+1,ty1-ty0+1));
}


void AGPainter::putPixel(const AGPoint &p,const AGColor &c)
{
  if(!inRect(p))
    return;
  AGPoint p2=move(p);
  getScreen().putPixel(p.x,p.y,c);
}

void AGPainter::blit(const AGTexture &pSource,const AGRect &pDest)
{
  //  CTRACE;

  AGRect s=pSource.getRect();
  // ASSUME: we don't want to scale
  //  blit(pSource,pDest,pSource.getRect());
  blit(pSource,AGRect(pDest.x,pDest.y,s.w,s.h),s);
}
void AGPainter::blit(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  //  CTRACE;
  AGRect dest=move(pDest);
  std::pair<AGRect,AGRect> rs=clip(pSrc,dest);

  /*  cdebug(mRect);
  cdebug(pSrc<<"//D:"<<pDest);
  cdebug(dest);
  cdebug(rs.first<<"///"<<rs.second);
  */
  if(rs.first.w>0)
    {
      getScreen().blit(pSource,rs.second,rs.first);
    }
}
void AGPainter::tile(const AGTexture &pSource)
{
  //  CTRACE;
  tile(pSource,getScreen().getRect());
}
void AGPainter::tile(const AGTexture &pSource,const AGRect &pDest)
{
  //  CTRACE;
  //  cdebug(mRect<<"       "<<pDest);
  //  tile(pSource,pDest,pSource.getRect());
  AGRect r=move(pDest);
  std::pair<AGRect,AGRect> rs=clip(r,r);
  //  cdebug(r);
  //  cdebug(rs.first);
  getScreen().tile(pSource,rs.second);
}
void AGPainter::tile(const AGTexture &pSource,const AGRect &pDest,const AGRect &pSrc)
{
  //  CTRACE;
  //  cdebug("FIXME - IMPLEMENT ME");

  AGRect r=move(pDest);
  std::pair<AGRect,AGRect> rs=clip(pSrc,r);
  
  getScreen().tile(pSource,rs.second,pSrc);
}
  
void AGPainter::transform(const AGRect &r)
{
  mClips.push_back(mRect);
  AGRect mnew=move(r);
  //  cdebug("r:"<<r);
  //  cdebug("mnew:"<<mnew);
  std::pair<AGRect,AGRect> p=clip(mnew,mnew);
  //  cdebug("P:"<<p.first<<"//"<<p.second);
  mRect=p.second;
  //  cdebug("mrect:"<<mRect);
}


AGRect AGPainter::getRect() const
{
  return AGRect(0,0,mRect.w,mRect.h);
}

void AGPainter::drawGradient(const AGRect &r,const AGColor &c0,const AGColor &c1,const AGColor &c2,const AGColor &c3)
{
  //  cdebug(mRect);
  //  cdebug(r);
  AGRect r2=move(r);

  
  std::pair<AGRect,AGRect> p=clip(r2,r2);
  //  cdebug(p.first);
  
  //FIXME: gradient should be clipped too / so colors must be changed
  AGDraw::drawGradient(&getScreen(),p.second,c0,c1,c2,c3);
}
void AGPainter::renderText(const std::string &pText,const AGPoint &p,const AGFont &f)
{
  // FIXME: clip text
  //  cdebug(mRect);
  //  cdebug(p);
  AGFontEngine::renderText (&getScreen(),mRect,mRect.x+p.x,mRect.y+p.y,pText,f);

}
void AGPainter::drawBorder(const AGRect& pRect,int width, const AGColor& c1, const AGColor& c2)
{
  AGRect r2=move(pRect);

  
  std::pair<AGRect,AGRect> p=clip(r2,r2);
  //  cdebug(p.first);
  
  AGDraw::drawBorder(&getScreen(),p.second,width,c1,c2);
}

void AGPainter::drawRect(const AGRect &pRect,const AGColor &c)
{
  AGRect r2=move(pRect);

  
  std::pair<AGRect,AGRect> p=clip(r2,r2);
  //  cdebug(p.first);
  getScreen().drawRect(p.second,c);
}
