#ifndef ANT_SIMPLE_SCENE_GRAPH_H
#define ANT_SIMPLE_SCENE_GRAPH_H

#include "ant_scene_graph.h"

class AntSimpleSceneGraph:public AntSceneGraph
  {
public:
    void render(const AntCamera &pCamera,const AntRenderMode &pRenderMode);
  };

#endif
