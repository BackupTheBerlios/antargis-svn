/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_texturecache.cpp
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

#include "ag_texture.h"
#include "ag_texturecache.h"
#include "ag_png.h"
#include "ag_kill.h"
#include "ag_geometry.h"

AGTextureCache *mTextureCache=0;
AGTextureCache *getTextureCache()
{
  if(!mTextureCache)
    mTextureCache=new AGTextureCache;
  return mTextureCache;
}

AGTextureCache::AGTextureCache()
{
  getInstanceKiller()->reg(createKiller(this));
}

const AGTexture &AGTextureCache::get(const std::string &pTexture,const AGRect2 &pSub)
{
  std::string s=pTexture+":"+pSub.toString();

  std::map<std::string,AGTexture>::iterator i=mTextures.find(s);
  if(i==mTextures.end())
    {
      // load
      AGSurface ms=AGSurface::load(pTexture).getSubSurface(pSub);
      


      /*      while(downScaleExp>1 && ms.width()>16 && ms.height()>16)
	{
	  ms=ms.scale(ms.width()/2,ms.height()/2);
	  downScaleExp--;
	}
      */
      mTextures[s]=AGTexture(ms,false);
    }
  return mTextures[s];
}


const AGTexture &AGTextureCache::get(const std::string &pTexture,int downScaleExp)
  {
    std::map<std::string,AGTexture>::iterator i=mTextures.find(pTexture);
    if(i==mTextures.end())
      {
        // load
        AGSurface ms=AGSurface::load(pTexture);

	while(downScaleExp>1 && ms.width()>16 && ms.height()>16)
	  {
	    ms=ms.scale(ms.width()/2,ms.height()/2);
	    downScaleExp--;
	  }

        mTextures[pTexture]=AGTexture(ms,false);
      }
    return mTextures[pTexture];
  }


const AGTexture &AGTextureCache::get3D(const std::string &pTexture,int downScaleExp)
  {
    std::map<std::string,AGTexture>::iterator i=mTextures.find(pTexture);
    if(i==mTextures.end())
      {
        // load
        AGSurface ms=AGSurface::load(pTexture);

	while(downScaleExp>1 && ms.width()>16 && ms.height()>16)
	  {
	    ms=ms.scale(ms.width()/2,ms.height()/2);
	    downScaleExp--;
	  }

        mTextures[pTexture]=AGTexture(ms,true);
      }
    return mTextures[pTexture];
  }
