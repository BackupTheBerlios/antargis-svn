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

AGBorder::AGBorder(const std::string &pTheme)
{
  if((mEnable=getTheme()->hasSurface(pTheme+".image")))
    {
      mSurface=AGTexture(getTheme()->getSurface(pTheme+".image"));
    }
}

void AGBorder::draw(AGPainter &p)
{
  if(!mEnable)
    return;
  int w=mSurface.width()/3;
  int w2=w*2;

  AGRect d=p.getRect();
  int x2=d.x+d.w-1;
  int y2=d.y+d.h-1;

  // corners
  p.blit(mSurface,AGRect(0,0,w,w),AGRect(0,0,w,w));
 
  p.blit(mSurface,AGRect(0,y2-w+1,w,w),AGRect(0,w2,w,w));
 
  p.blit(mSurface,AGRect(x2-w+1,0,w,w),AGRect(w2,0,w,w));
 
  p.blit(mSurface,AGRect(x2-w+1,y2-w+1,w,w),AGRect(w2,w2,w,w));

  // borders

  // hor
  p.tile(mSurface,AGRect(w,0,d.w-w2,w),AGRect(w,0,w,w));
  p.tile(mSurface,AGRect(w,y2-w+1,d.w-w2,w),AGRect(w,w2,w,w));
  // ver
  p.tile(mSurface,AGRect(0,w,w,d.h-w2),AGRect(0,w,w,w));
  p.tile(mSurface,AGRect(x2-w+1,w,w,d.h-w2),AGRect(w2,w,w,w));

  // skrip interior

  
}
