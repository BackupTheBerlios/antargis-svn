//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "texture_cache.h"

TextureCache *mTextureCache=0;
TextureCache *getTextureCache()
{
  if(!mTextureCache)
    mTextureCache=new TextureCache;
  return mTextureCache;
}

