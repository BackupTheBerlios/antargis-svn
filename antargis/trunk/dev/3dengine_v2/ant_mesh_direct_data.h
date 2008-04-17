#ifndef ANT_MESH_DIRECT_DATA_H
#define ANT_MESH_DIRECT_DATA_H

#include "ant_static_mesh_data.h"
#include "ant_mesh_render_data.h"

class AntMeshDirectData:public AntMeshRenderData
  {
    AntStaticMeshData *mMeshData;

public:
  AntMeshDirectData(AntStaticMeshData *pMeshData);

  void render(const AntRenderMode &pMode);
  };

#endif
