/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_texturecache.h
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

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#ifndef __TEXTURE_CACHE_H__
#define __TEXTURE_CACHE_H__

#include "ag_surface.h"
#include "ag_debug.h"
#include <ag_fs.h>

#include <string>
#include <map>

class AGRect2;

class AGTextureCache
  {
    AGTextureCache();
  public:
    const AGTexture &get(const AGFilename &pTexture,int downScaleExp=1);
    const AGTexture &get3D(const AGFilename &pTexture,int downScaleExp=1,int downScaleZ=1);

    const AGTexture &get(const AGFilename &pTexture,const AGRect2 &pSub);
  private:
    std::map<AGFilename,AGTexture*> mTextures;
    
    friend AGTextureCache *getTextureCache();
  };

AGTextureCache *getTextureCache();


#endif
