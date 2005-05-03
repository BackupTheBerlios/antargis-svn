/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_background.cc
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

#include "ag_background.h"
#include "ag_draw.h"
#include "ag_theme.h"
#include "ag_debug.h"
#include "ag_painter.h"

AGBackground::AGBackground(std::string pThemeName):mSurfaceFlag(false)
{
  //  CTRACE;
  AGTheme *theme=getTheme();
  mColor=false;
  if(theme->hasSurface(pThemeName+".image"))
    {
      //      CTRACE;
      cdebug(pThemeName+".image");
      mSurface=AGTexture(theme->getSurface(pThemeName+".image"));
      mSurfaceFlag=true;
    }
  else if(theme->hasColor(pThemeName+"."+std::string("gradientColor1")))
    {
      mColor=true;
      mColors[0]=theme->getColor(pThemeName+"."+std::string("gradientColor1"));
      mColors[1]=theme->getColor(pThemeName+"."+std::string("gradientColor2"));
      mColors[2]=theme->getColor(pThemeName+"."+std::string("gradientColor3"));
      mColors[3]=theme->getColor(pThemeName+"."+std::string("gradientColor4"));
    }

  //  cdebug(pThemeName<<":"<<mSurfaceFlag);
  mBorder=theme->getInt(pThemeName+"."+std::string("border"));
}

void AGBackground::draw(AGPainter &p)
{
  if(mSurfaceFlag)
    p.tile(mSurface,p.getRect().shrink(mBorder));
  else if(mColor)
    p.drawGradient(p.getRect().shrink(mBorder),mColors[0],mColors[1],mColors[2],mColors[3]);
 
}
