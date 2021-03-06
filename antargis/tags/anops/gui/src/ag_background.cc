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

/** sets a uniform background of color pColor
    @param pColor an rgba-color. so you can use transparent backgrounds,too.
*/
AGBackground::AGBackground(const AGColor &pColor):mTextureFlag(false)
{
  mColor=true;
  mColors[0]=pColor;
  mColors[1]=pColor;
  mColors[2]=pColor;
  mColors[3]=pColor;
  mBorder=0;
}


/**
   This constructor set a themed background.
   @param pThemeName a typical string is "button.background.normal". Then the entry in the theme.xml file should look something like:
<pre>
 <button>
   <background>
     <normal>
       <color name='gradientColor1' color='#12141c'/>
       <color name='gradientColor2' color='#12141c'/>
       <color name='gradientColor3' color='#12141c'/>
       <color name='gradientColor4' color='#12141c'/>
     </normal>
   </background>
  </button>
</pre>
*/

AGBackground::AGBackground(const AGString &pThemeName):mTextureFlag(false)
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
  else if(theme->hasColor(pThemeName+"."+"gradientColor1"))
    {
      mColor=true;
      mColors[0]=theme->getColor(pThemeName+"."+"gradientColor1");
      mColors[1]=theme->getColor(pThemeName+"."+"gradientColor2");
      mColors[2]=theme->getColor(pThemeName+"."+"gradientColor3");
      mColors[3]=theme->getColor(pThemeName+"."+"gradientColor4");
    }

  mBorder=theme->getInt(pThemeName+"."+"border");
}


/// draws the background on painter in the given rectangle
void AGBackground::draw(const AGRect2 &r,AGPainter &p)
{
  if(mTextureFlag)
    {
      p.tile(mTexture,r.shrink(mBorder));
    }
  else if(mColor)
    p.drawGradient(r.shrink(mBorder),mColors[0],mColors[1],mColors[2],mColors[3]);
 
}

/// this is a help function, so that once made textures aren't automatically discarded.
void AGBackground::useTextures()
{
  mTexture.useTexture();
}
