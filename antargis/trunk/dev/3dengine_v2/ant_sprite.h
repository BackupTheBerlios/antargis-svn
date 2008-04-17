#ifndef ANT_SPRITE_H
#define ANT_SPRITE_H

#include "ant_renderable.h"
#include "ant_mesh_render_data.h"
#include "ant_texture.h"

class AntSprite:public AntRenderable
  {
    AntMeshRenderData *mData;
    AntTexture *mTexture;
public:
  AntSprite(const AGVector3 &pPosition,const AntQuaternion &pRotation,AntMeshRenderData *pData,AntTexture *pTexture);
  
  void renderObjectSpace(const AntRenderMode &pMode);

  };

#endif
