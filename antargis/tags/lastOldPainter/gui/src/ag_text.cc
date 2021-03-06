/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_text.cc
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


#include "ag_text.h"
#include "ag_debug.h"
#include "ag_surface.h"
#include "ag_fontengine.h"
#include "ag_geometry.h"

#include <iostream>


using namespace std;

AGText::AGText(AGWidget *pParent,const AGRect &r,const std::string &pText,const AGFont &pFont):
  AGWidget(pParent,r),
  mText(pText),mFont(pFont)
{
  mFixedSize=true;
}

AGText::AGText(AGWidget *pParent,const AGPoint &p,const std::string &pText,const AGFont &pFont):
  AGWidget(pParent,AGRect(p.x,p.y,pFont.getWidth(pText),pFont.getHeight(pText))),
  mText(pText),mFont(pFont)
{
  mFixedSize=false;
}

void AGText::draw(AGPainter &p)
{
  int x=(width()-mFont.getWidth(mText))/2;
  int y=(height()-mFont.getHeight(mText))/2;
  p.renderText(mText,AGPoint(x,y),mFont);
}

void AGText::setDeriveRect()
{
  setRect(AGRect(getRect().x,getRect().y,mFont.getWidth(mText),mFont.getHeight(mText)));
}

void AGText::setText(const std::string &pText)
{
  mText=pText;
  if(!mFixedSize)
    {
      setWidth(mFont.getWidth(pText));
      setHeight(mFont.getHeight(pText));
    }
}
std::string AGText::getText() const
{
  return mText;
}

AGText &toAGText(AGWidget &w)
{
  return dynamic_cast<AGText&>(w);
}

void AGText::setFont(const AGFont &f)
{
  mFont=f;
}
