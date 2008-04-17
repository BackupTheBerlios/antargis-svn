#ifndef ANT_MESH_VA_DATA_H
#define ANT_MESH_VA_DATA_H

#include "ant_static_mesh_data.h"
#include "ant_mesh_render_data.h"

class AntMeshVAData:public AntMeshRenderData
  {
    AntStaticMeshData *mMeshData;
    size_t mMeshChangeLevel;

    unsigned int mVertexBuffer,mColorBuffer,mNormalBuffer,mTexBuffer,mIndexBuffer;

public:
  AntMeshVAData(AntStaticMeshData *pMeshData);

  void render(const AntRenderMode &pMode);
private:
  void checkForChanges();
  void createBuffers();
  void dropBuffers();
  bool has2DTextures() const;
  bool has3DTextures() const;
  bool vertexBuffersActive() const;

  bool mDynamic;

  };

#endif
