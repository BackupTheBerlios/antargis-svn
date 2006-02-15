/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_border.cc
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

#include "ag_border.h"
#include "ag_painter.h"
#include "ag_theme.h"
#include "ag_texturecache.h"

AGBorder::AGBorder(const std::string &pTheme)
{
  if((mEnable=getTheme()->hasSurface(pTheme+".image")))
    {
      //      mSurface=getTextureManager()->makeTexture(getTheme()->getSurface(pTheme+".image"));
      mTexture=&getTextureCache()->get(getTheme()->getSurfaceName(pTheme+".image"));
      mWidth=mTexture->width()/3;
    }
  else
    {
      mWidth=0;
      mTexture=0;
    }
}

void AGBorder::draw(AGPainter &p)
{
  if(mTexture==0 || !mEnable)
    return;
  int w=mTexture->width()/3;
  int w2=w*2;

  AGRect d=p.getRect();
  float x2=d.x1();
  float y2=d.y1();

  // corners
  p.blit(*mTexture,AGRect(0,0,w,w),AGRect(0,0,w,w));
 
  p.blit(*mTexture,AGRect(0,y2-w,w,w),AGRect(0,w2,w,w));
 
  p.blit(*mTexture,AGRect(x2-w,0,w,w),AGRect(w2,0,w,w));
 
  p.blit(*mTexture,AGRect(x2-w,y2-w,w,w),AGRect(w2,w2,w,w));

  // borders

  // hor
  p.tile(*mTexture,AGRect(w,0,d.w()-w2,w),AGRect(w,0,w,w));
  p.tile(*mTexture,AGRect(w,y2-w,d.w()-w2,w),AGRect(w,w2,w,w));
  // ver
  p.tile(*mTexture,AGRect(0,w,w,d.h()-w2),AGRect(0,w,w,w));
  p.tile(*mTexture,AGRect(x2-w,w,w,d.h()-w2),AGRect(w2,w,w,w));

  // skip interior
  
}

size_t AGBorder::getWidth() const
{
  return mWidth;
}

void AGBorder::useTextures()
{
  if(mTexture)
    const_cast<AGTexture*>(mTexture)->useTexture();
}
