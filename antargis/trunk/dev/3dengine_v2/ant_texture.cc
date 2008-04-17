#include "ant_texture.h"
#include "ant_texture_internal.h"
#include "ag_debug.h"


//////////////////////////////////////////////////////////////////////////
// AntTexture
//////////////////////////////////////////////////////////////////////////

// Create a 3d-texture
AntTexture::AntTexture(size_t w,size_t h,size_t d):
  mTexture(new AntTextureData(w,h,d))
  {
    mTexture->registerUser(this);
  }
// Create a 2d-texture
AntTexture::AntTexture(size_t w,size_t h):
  mTexture(new AntTextureData(w,h,1))
  {
    mTexture->registerUser(this);
  }
// copy texture-ref
AntTexture::AntTexture(const AntTexture &pTexture):
  mTexture(pTexture.mTexture)
  {
    mTexture->registerUser(this);
  }

AntTexture::AntTexture(const AGString &pFilename,size_t pDepth):
  mTexture(AntTextureData::load(pFilename,pDepth))
  {
    mTexture->registerUser(this);
  }

AntTexture::AntTexture(AntTextureData *pData):
  mTexture(pData)
  {
    mTexture->registerUser(this);
  }

AntTexture::~AntTexture()
  {
    AntTextureData::removeUser(mTexture,this);
  }


AntTexture AntTexture::dup() const
{
  return AntTexture(mTexture->clone());
}

void AntTexture::bind(int unit)
  {
    CTRACE;
    mTexture->bind(unit);
  }

