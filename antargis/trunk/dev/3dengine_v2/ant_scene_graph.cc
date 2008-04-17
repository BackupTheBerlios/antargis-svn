#include "ant_scene_graph.h"

AntSceneGraph::~AntSceneGraph()
  {

  }

void AntSceneGraph::add(AntSceneNode *pNode)
  {
    mAllNodes.insert(pNode); 
  }
void AntSceneGraph::erase(AntSceneNode *pNode)
  {
    mAllNodes.erase(pNode); 
  }

const std::set<AntSceneNode*> &AntSceneGraph::getAllNodes() const
{
  return mAllNodes;
}

void AntSceneGraph::render(const AntCamera &pCamera,const AntRenderMode &pRenderMode)
  {
    
  }
