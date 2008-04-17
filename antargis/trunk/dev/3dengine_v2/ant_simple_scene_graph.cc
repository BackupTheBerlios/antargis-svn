#include "ant_simple_scene_graph.h"

void AntSimpleSceneGraph::render(const AntCamera &pCamera,const AntRenderMode &pRenderMode)
  {
    const std::set<AntSceneNode*> &allNodes=getAllNodes();
    
    for(std::set<AntSceneNode*>::const_iterator i=allNodes.begin();i!=allNodes.end();i++)
      {
        (*i)->render(pRenderMode);
      }
  }
