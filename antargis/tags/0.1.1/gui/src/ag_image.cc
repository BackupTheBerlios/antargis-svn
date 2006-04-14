/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_image.cc
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

#include "ag_image.h"
#include "ag_debug.h"

AGImage::AGImage(AGWidget *pParent,const AGRect2 &r,AGSurface pSurface,bool pTile,const AGRect2 &pRect):
  AGWidget(pParent,r),
  mTexture(pSurface),mTile(pTile)
{
  mCenter=true;
  //  CTRACE;
  if(pRect!=pSurface.getRect() && pRect.w()!=0 && pRect.h()!=0)
    {
      mSrcRect=pRect;

      setHeight(pRect.h());
      setWidth(pRect.w());
    }
}
AGImage::AGImage(AGWidget *pParent,const AGRect2 &r,AGTexture pTexture,bool pTile,const AGRect2 &pRect):
  AGWidget(pParent,r),
  mTexture(pTexture),mTile(pTile)
{
  mCenter=true;
  //  CTRACE;
  if(pRect!=pTexture.getRect() && pRect.w()!=0 && pRect.h()!=0)
    {
      mSrcRect=pRect;

      setHeight(pRect.h());
      setWidth(pRect.w());
    }
}



AGImage::~AGImage()
{
}

void AGImage::draw(AGPainter &p)
{
  // do always center for now
  bool center=true;


  if(mTile)
    {
      if(mSrcRect.w()==0 || mSrcRect.h()==0)
	mSrcRect=mTexture.getRect();
      p.tile(mTexture,getRect().origin(),mSrcRect);
    }
  else if(center)
    {
      
      AGRect2 mr=getRect().origin();

      if(mCenter)
	mr+=AGVector2((width()-mTexture.width())/2,(height()-mTexture.height())/2);

      p.blit(mTexture,mr);
    }
  else
    p.blit(mTexture,getRect().origin());
}

void AGImage::setSurface(const AGSurface &pSurface)
{
  AGTexture t(pSurface);
  mTexture=t;
  queryRedraw();
}
void AGImage::setTexture(const AGTexture &pTexture)
{
  mTexture=pTexture;
  queryRedraw();
}

void AGImage::useTextures()
{
  mTexture.useTexture();
}

void AGImage::setCenter(bool c)
{
  mCenter=c;
}
