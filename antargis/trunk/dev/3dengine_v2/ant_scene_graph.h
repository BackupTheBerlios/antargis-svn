#ifndef ANT_SCENE_GRAPH_H
#define ANT_SCENE_GRAPH_H

#include <set>

#include "ant_scene_node.h"
#include "ant_camera.h"

class AntSceneGraph
  {
    std::set<AntSceneNode*> mAllNodes;
public:
  virtual ~AntSceneGraph();
  
  virtual void add(AntSceneNode *pNode);
  virtual void erase(AntSceneNode *pNode);
  
  const std::set<AntSceneNode*> getAllNodes() const;
  
  virtual void render(const AntCamera &pCamera,const AntRenderMode &pRenderMode);

  };

#endif
