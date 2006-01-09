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

AGImage::AGImage(AGWidget *pParent,const AGPoint &p,AGSurface pSurface,bool pTile,const AGRect &pRect):
  AGWidget(pParent,pSurface.getRect()+p),
  mTexture(getTextureManager()->makeTexture(pSurface)),mTile(pTile)
{
  //  CTRACE;
  if(pRect!=pSurface.getRect() && pRect.w!=0 && pRect.h!=0)
    {
      mSrcRect=pRect;
      /*
	mTexture=pSurface.getSubSurface(pRect);*/
  //  if(mSrcRect.w==0 || mSrcRect.h==0)
  //    mSrcRect=mTexture.getRect();

    //  if(getRect()!=pRect)
    //    {
      setHeight(pRect.h);
      setWidth(pRect.w);
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
      if(mSrcRect.w==0 || mSrcRect.h==0)
	mSrcRect=mTexture.getRect();
      p.tile(mTexture,getRect().origin(),mSrcRect);
    }
  else if(center)
    {
      
      AGRect mr=getRect().origin();

      mr.x+=(width()-mTexture.width())/2;
      mr.y+=(height()-mTexture.height())/2;

      p.blit(mTexture,mr);
    }
  else
    p.blit(mTexture,getRect().origin());
}

void AGImage::setSurface(const AGSurface &pSurface)
{
  mTexture=getTextureManager()->makeTexture(pSurface);
}
void AGImage::setTexture(const AGTexture &pTexture)
{
  mTexture=pTexture;
}
