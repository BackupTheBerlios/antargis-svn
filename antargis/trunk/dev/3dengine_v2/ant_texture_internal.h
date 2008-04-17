#ifndef ANT_TEXTURE_INTERNAL_H
#define ANT_TEXTURE_INTERNAL_H

#include "ant_gl.h"
#include "ag_surface.h"

#include <stdexcept>
#include <set>
#include <map>

class AntTexture;

class AntTextureData
  {
    AGSurface mSurface;
    GLuint mID;
    size_t mDepth;
    size_t mDim;
    
    typedef std::runtime_error Exception;
public:
  AntTextureData(const AGSurface &pSurface,size_t pDepth=1);
  AntTextureData(size_t w,size_t h,size_t w);
  ~AntTextureData();

  AntTexture *makeTexture();
  AntTextureData *clone();
  
  void bind(int unit);
  static size_t getUnitCount();
  static size_t getActiveUnit();
private:
  void registerUser(AntTexture *pUser);
  static void removeUser(AntTextureData *pData,AntTexture *pUser);
  static AntTextureData *load(const AGString &pFilename,size_t pDepth=1);
  
  friend class AntTexture;
  std::set<AntTexture*> mUsers;
  
  static std::map<AGString,AntTextureData*> mLoaded;
  };

#endif
