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

        AGSurface ms(fromPNG(c));
        AGTexture mTexture(ms);
        mTextures[pTexture]=mTexture;
      }
    return mTextures[pTexture];
  }
