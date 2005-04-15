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
  mTexture(pSurface),mTile(pTile)
{
  CTRACE;
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

void AGImage::draw(const AGRect &r)
{
  //cdebug(mTexture.surface()<<endl);
  //mTexture.toScreen(100,100);
  //  AGSurface screen(getScreen());

  // do always center for now
  bool center=true;

  if(mTile)
    getScreen().tile(mTexture,r.project(getRect()),mSrcRect);
  else if(center)
    {
      
      AGRect mr=getRect();

      mr.x+=(width()-mTexture.width())/2;
      mr.y+=(height()-mTexture.height())/2;

      mr=r.project(mr);
      getScreen().blit(mTexture,mr);
    }
  else
    getScreen().blit(mTexture,r.project(getRect()));
}

void AGImage::setSurface(AGSurface pSurface)
{
  mTexture=AGTexture(pSurface);
}

/*
void AGImage::setSourceRect(const AGRect &pRect)
{
  mSrcRect=pRect;
}
*/
