//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __TEXTURE_CACHE_H__
#define __TEXTURE_CACHE_H__

#include "ag_gsurface.h"
#include "ag_debug.h"
#include "fs.h"

#include <string>
#include <map>
#include "mpng.h"

class TextureCache
{
    TextureCache()
    {
      CTRACE;
    }
  public:
    const AGTexture &get(const std::string &pTexture)
    {
      
      std::map<std::string,AGTexture>::iterator i=mTextures.find(pTexture);
      if(i==mTextures.end())
      {
        // load
        std::string c=loadFile(pTexture);

        AGSurface ms(fromPNG(c));
        AGTexture mTexture(ms);
        mTextures[pTexture]=mTexture;
      }
      return mTextures[pTexture];
    }
  private:
    std::map<std::string,AGTexture> mTextures;
    
    friend TextureCache *getTextureCache();
};

TextureCache *getTextureCache();


#endif
