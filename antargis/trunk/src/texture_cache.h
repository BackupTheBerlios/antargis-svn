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
    TextureCache();
  public:
    const AGTexture &get
    (const std::string &pTexture);
  private:
    std::map<std::string,AGTexture> mTextures;

    friend TextureCache *getTextureCache();
  };

TextureCache *getTextureCache();


#endif
