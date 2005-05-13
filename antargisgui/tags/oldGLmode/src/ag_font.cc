/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_font.cc
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

#include "ag_font.h"
#include "ag_fontengine.h"

AGFont::AGFont(const std::string &pFile,int size):
  mName(pFile),mSize(size),mColor(0xFF,0xFF,0xFF)
{
  mAlpha=0xFF;
  mStyle=NORMAL;
}

AGFont::AGFont()
{
  mName="Arial.ttf";
  mSize=14;
  mColor=AGColor(0xFF,0xFF,0xFF);
  mAlpha=0xFF;
  mStyle=NORMAL;
}

void AGFont::setColor(const AGColor &pColor)
{
  mColor=pColor;
}
AGColor AGFont::getColor() const
{
  return mColor;
}

void AGFont::setAlpha(Uint8 pAlpha)
{
  mAlpha=pAlpha;
}
Uint8 AGFont::getAlpha() const
{
  return mAlpha;
}

void AGFont::setSize(Uint8 pSize)
{
  mSize=pSize;
}
Uint8 AGFont::getSize() const
{
  return mSize;
}

void AGFont::setStyle(const Style &s)
{
  mStyle=s;
}

AGFont::Style AGFont::getStyle() const
{
  return mStyle;
}

void AGFont::setName(const std::string &s)
{
  mName=s;
}
std::string AGFont::getName() const
{
  return mName;
}

int AGFont::getWidth(const std::string &mText) const
{
  return AGFontEngine::getWidth(*this,mText);
}
int AGFont::getHeight(const std::string &mText) const
{
  return AGFontEngine::getHeight(*this,mText);
}


bool operator<(const AGFont&f1,const AGFont &f2)
{
  return f1.mStyle<f2.mStyle || 
    (f1.mStyle==f2.mStyle && (f1.mName<f2.mName || 
			      (f1.mName==f2.mName && (f1.mSize<f2.mSize ||
						      (f1.mSize==f2.mSize && (f1.mAlpha<f2.mAlpha ||
									      (f1.mAlpha==f2.mAlpha && f1.mColor<f2.mColor)))))));
}
