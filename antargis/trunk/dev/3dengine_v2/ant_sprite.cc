#include "ant_sprite.h"
#include "ag_debug.h"

AntSprite::AntSprite(const AGVector3 &pPosition,const AntQuaternion &pRotation,AntMeshRenderData *pData,AntTexture *pTexture):
  AntRenderable(pPosition,pRotation),
  mData(pData),
  mTexture(pTexture)
    {

    }

void AntSprite::renderObjectSpace(const AntRenderMode &pMode)
  {
    CTRACE;
    cdebug("MUH");
    //mTexture->bind(0);
    mData->render(pMode);
  }
