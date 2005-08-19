/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * texture_cache.cpp
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

#include "texture_cache.h"

TextureCache *mTextureCache=0;
TextureCache *getTextureCache()
{
  if(!mTextureCache)
    mTextureCache=new TextureCache;
  return mTextureCache;
}

TextureCache::TextureCache()
{
  CTRACE;
}
const AGTexture &TextureCache::get
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
