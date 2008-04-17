#ifndef ANT_MESH_RENDER_DATA_H
#define ANT_MESH_RENDER_DATA_H

#include "ant_render_mode.h"

class AntMeshRenderData
  {
public:
  virtual ~AntMeshRenderData();
  virtual void render(const AntRenderMode &pMode);
  };

#endif
