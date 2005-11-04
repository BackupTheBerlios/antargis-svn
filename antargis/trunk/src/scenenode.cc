#include "scenenode.h"
#include "scene.h"
#include "ag_debug.h"

SceneNode::SceneNode()
    {
      mRubyObject=false;
      scene=0;
    }
SceneNode::~SceneNode()
{
  mRubyObject=false;
  if(scene)
    scene->removeNode(this);
}
void SceneNode::setScene(Scene *s)
{
  scene=s;
}
  void SceneNode::drawShadow()
  {
  }
  void SceneNode::drawDepth()
  {
  }
  void SceneNode::draw()
  {
  }
  void SceneNode::advance(float time)
  {
  }
  size_t SceneNode::getTriangles() const
  {
    return 0;
  }
  AGVector4 SceneNode::lineHit(const AGLine3 &pLine) const
  {
    AGVector4 mfalse(0,0,0,0);
    return mfalse;
  }

  void SceneNode::sort(const AGVector4 &pCamera)
  {
  }
  void SceneNode::mapChanged()
  {
  }

  bool SceneNode::operator==(const SceneNode &n) const
  {
    return this==&n;
  }

  void SceneNode::setPos(const AGVector3&pPos)
  {
  }

  bool SceneNode::transparent()
  {
    return false;
  }

