#ifndef ANT_EXAMPLE_CUBE_H
#define ANT_EXAMPLE_CUBE_H

#include "ant_scene_node.h"

class AntExampleCube:public AntSceneNode
  {
public:
  virtual void render(const AntRenderMode &pMode); 
  };


#endif
