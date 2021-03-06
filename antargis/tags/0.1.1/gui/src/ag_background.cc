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


AGBackground::AGBackground(const AGColor &pColor):mTextureFlag(false)
{
  mColor=true;
  mColors[0]=pColor;
  mColors[1]=pColor;
  mColors[2]=pColor;
  mColors[3]=pColor;
  mBorder=0;
}


AGBackground::AGBackground(std::string pThemeName):mTextureFlag(false)
{
  //  CTRACE;
  AGTheme *theme=getTheme();
  mColor=false;
  if(theme->hasSurface(pThemeName+".image"))
    {
      //      CTRACE;
      cdebug(pThemeName+".image");
      mTexture=AGTexture(theme->getSurface(pThemeName+".image"));
      mTextureFlag=true;
    }
  else if(theme->hasColor(pThemeName+"."+std::string("gradientColor1")))
    {
      mColor=true;
      mColors[0]=theme->getColor(pThemeName+"."+std::string("gradientColor1"));
      mColors[1]=theme->getColor(pThemeName+"."+std::string("gradientColor2"));
      mColors[2]=theme->getColor(pThemeName+"."+std::string("gradientColor3"));
      mColors[3]=theme->getColor(pThemeName+"."+std::string("gradientColor4"));
    }

  mBorder=theme->getInt(pThemeName+"."+std::string("border"));
}

void AGBackground::draw(const AGRect2 &r,AGPainter &p)
{
  if(mTextureFlag)
    {
      p.tile(mTexture,r.shrink(mBorder));
    }
  else if(mColor)
    p.drawGradient(r.shrink(mBorder),mColors[0],mColors[1],mColors[2],mColors[3]);
 
}

void AGBackground::useTextures()
{
  mTexture.useTexture();
}
