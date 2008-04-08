#ifndef ANT_SCENE_NODE_H
#define ANT_SCENE_NODE_H

#include "ant_render_mode.h"

class AntSceneNode
  {
public:
  virtual void render(const AntRenderMode &pMode); 
  };

#endif
