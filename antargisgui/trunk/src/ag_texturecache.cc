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

#include "ag_texturecache.h"
#include "ag_png.h"
#include "ag_kill.h"

AGTextureCache *mTextureCache=0;
AGTextureCache *getTextureCache()
{
  if(!mTextureCache)
    mTextureCache=new AGTextureCache;
  return mTextureCache;
}

AGTextureCache::AGTextureCache()
{
  CTRACE;
  getInstanceKiller()->reg(createKiller(this));
}
const AGTexture &AGTextureCache::get
  (const std::string &pTexture)
  {

    std::map<std::string,AGTexture>::iterator i=mTextures.find(pTexture);
    if(i==mTextures.end())
      {
        // load
        std::string c=loadFile(pTexture);

        AGSurface ms=getSurfaceManager()->fromSDL(fromPNG(c));
        mTextures[pTexture]=getTextureManager()->makeTexture(ms);;
      }
    return mTextures[pTexture];
  }
