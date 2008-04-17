#ifndef ANT_TEXTURE_H
#define ANT_TEXTURE_H

#include "ant_gl.h"
#include "ag_string.h"

class AntTexture
  {
public:
  // Create a 3d-texture
  AntTexture(size_t w,size_t h,size_t d);
  // Create a 2d-texture
  AntTexture(size_t w,size_t h);
  // copy texture-ref
  AntTexture(const AntTexture &pTexture);
  // create texture from given file (cached)
  AntTexture(const AGString &pFilename,size_t pDepth=1);
  
  ~AntTexture();

  AntTexture dup() const;
  
  void bind(int unit);
private:
  friend class AntTextureData;
  
  AntTexture(AntTextureData*pTexture);

  AntTextureData *mTexture;
  };

#endif
