#include "ant_texture_internal.h"
#include "ag_vdebug.h"

std::map<AGString,AntTextureData*> AntTextureData::mLoaded;


struct AntTextureUnitState
  {
    GLint dim,id;
    AntTextureUnitState()
      {
        GLboolean b;
        glGetBooleanv(GL_TEXTURE_BINDING_1D,&b);
        if(b==GL_TRUE)
          {
            dim=GL_TEXTURE_1D;
            glGetIntegerv(GL_TEXTURE_1D,&id);
          }
        else
          {
            glGetBooleanv(GL_TEXTURE_BINDING_2D,&b);
            if(b==GL_TRUE)
              {
                dim=GL_TEXTURE_2D;
                glGetIntegerv(GL_TEXTURE_2D,&id);
              }
            else
              {
                glGetBooleanv(GL_TEXTURE_BINDING_3D,&b);
                if(b==GL_TRUE)
                  {
                    dim=GL_TEXTURE_3D;
                    glGetIntegerv(GL_TEXTURE_3D,&id);
                  }
                else
                  {
                    dim=GL_TEXTURE_2D; // make this default
                    //throw Exception("ERROR: Unknown Texture-Unit state!");
                  }
              }
          }
      }
    ~AntTextureUnitState()
      {
        glBindTexture(dim,id);
      }
  };


AntTextureData::AntTextureData(const AGSurface &pSurface,size_t pDepth):
  mSurface(pSurface),mDepth(pDepth)
  {
    AntTextureUnitState saved;
    GLint format=GL_RGBA;

    size_t d=pDepth;
    size_t w=pSurface.width();
    size_t h=pSurface.height()/d;

    bool threed=(d>1);
    glGenTextures(1,&mID);

    if(threed)
      {
        mDim=3;
        glBindTexture(GL_TEXTURE_2D,mID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, 0);
      }
    else
      {
        glBindTexture(GL_TEXTURE_3D,mID);
        glTexImage3D(GL_TEXTURE_3D, 0, format, w, h, d, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, 0);
        mDim=2;
      }



    SDL_Surface *surface=pSurface.s->surface;

    switch(surface->format->BytesPerPixel)
      {
    case 3:
      format = GL_RGB;break;
    case 4:
      format = GL_RGBA;break;
    default:
      throw Exception("Surface-format not supported for texturing!");
      }

    assertGL;


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    assertGL;
    glPixelStorei(GL_UNPACK_ROW_LENGTH,
        surface->pitch / surface->format->BytesPerPixel);

    AGVector2 offset(0,0);

    assertGL;
    if(threed)
      {
        assert(glIsEnabled(GL_TEXTURE_3D));
        int mw=std::min(surface->w,surface->h);
        int mh=std::max(surface->w,surface->h)/mw;

        // FIXME: use glTexImage instead of glTexSubImage, because it makes problems
        // on MacOSX 10.5.1 (at least on my macbook pro - godrin)

        glTexImage3D(GL_TEXTURE_3D, 0, format, w, h, d, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, surface->pixels);
        //glTexSubImage3D(GL_TEXTURE_3D,0,int(offset[0]),int(offset[1]),0,
        //    mw,mw,mh,format,GL_UNSIGNED_BYTE,surface->pixels);
      }
    else
      {
        assertGL;

        glTexSubImage2D(GL_TEXTURE_2D, 0, int(offset[0]), int(offset[1]),
            surface->w, surface->h, format, GL_UNSIGNED_BYTE,
            surface->pixels);
        cdebug("ok");
      }
  }
AntTextureData::AntTextureData(size_t w,size_t h,size_t d)
  {
    AntTextureUnitState saved;
    GLint format=GL_RGBA;

    bool threed=(d>1);
    glGenTextures(1,&mID);

    if(threed)
      {
        mDim=3;
        glBindTexture(GL_TEXTURE_2D,mID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, 0);
      }
    else
      {
        mDim=2;
        glBindTexture(GL_TEXTURE_3D,mID);
        glTexImage3D(GL_TEXTURE_3D, 0, format, w, h, d, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, 0);
      }
  }
AntTextureData::~AntTextureData()
  {
    assert(mUsers.size()==0);
    glDeleteTextures(1,&mID);
    
    
    // FIXME: this may be slow
    for(std::map<AGString,AntTextureData*>::iterator i=mLoaded.begin();i!=mLoaded.end();i++)
      if(i->second==this)
        {
          mLoaded.erase(i);
          break;
        }
        
  }

void AntTextureData::registerUser(AntTexture *pUser)
  {
    mUsers.insert(pUser);
  }

void AntTextureData::removeUser(AntTextureData *pData,AntTexture *pUser)
  {
    pData->mUsers.erase(pUser);
    if(pData->mUsers.size()==0)
      delete pData;
  }


AntTextureData *AntTextureData::clone()
  {
  }

size_t AntTextureData::getUnitCount()
  {
    GLint id;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS,&id);    
    checkGL("getting count of texture units failed!");
    return id;
  }

size_t AntTextureData::getActiveUnit()
  {
    GLint id;
    glGetIntegerv(GL_ACTIVE_TEXTURE,&id);
    checkGL("getting active Texture-unit failed!");
    return id;
  }

/// returns true on success and 0 on failure (unit not valid)
void AntTextureData::bind(int unit)
  {
    glActiveTexture(unit);
    /*if(getActiveUnit()!=unit)
      {
        if(getUnitCount()<=unit)
          {
            throw Exception("Invalid");
          }
        else
          {
            glActiveTexture(unit);
          }
      }*/

    if(mID==0)
      throw Exception("Invalid mID in AntTextureData::bind()");

    switch(mDim)
      {
    case 1:
      glBindTexture(GL_TEXTURE_1D,mID);
      break;
    case 2:
      glBindTexture(GL_TEXTURE_2D,mID);
      cdebug("bind 2d:"<<mID);
      break;
    case 3:
      glBindTexture(GL_TEXTURE_3D,mID);
      break;
    default:
      throw Exception("Invalid mDim in AntTextureData!");
      }
  }

AntTextureData *AntTextureData::load(const AGString &pFilename,size_t pDepth)
  {
    if(mLoaded.find(pFilename)==mLoaded.end())
      mLoaded.insert(std::make_pair(pFilename,new AntTextureData(AGSurface::load(pFilename),pDepth)));
    return mLoaded[pFilename];
  }

