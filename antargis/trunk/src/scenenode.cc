#include "scenenode.h"
#include "scene.h"
#include "ag_debug.h"

SceneNode::SceneNode(Scene *s)
{
  assert(s);
  mRubyObject=false;
  scene=s;
  mVisible=true;
  order=1;
}

SceneNode::~SceneNode()
{
  mRubyObject=false;
  removeFromAllScenes(this);
}

void SceneNode::resetScene()
{
  scene=0;
}
/*
void SceneNode::setScene(Scene *s)
{
  assert(s==0);
  scene=s;
}*/
void SceneNode::drawShadow()
{
}
void SceneNode::drawDepth()
{
}
void SceneNode::draw()
{
}
void SceneNode::drawPick()
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
/*void SceneNode::mapChanged()
{
}*/

bool SceneNode::operator==(const SceneNode &n) const
{
  return this==&n;
}

void SceneNode::setPos(const AGVector3&pPos)
{
}

void SceneNode::setRotation(float r)
{
}


  bool SceneNode::transparent()
  {
    return false;
  }

AGBox3 SceneNode::bbox() const
{
  return AGBox3(AGVector3(0,0,0),AGVector3(0,0,0));
}

AGRect2 SceneNode::getRect() const
{
  AGBox3 b=bbox();
  return AGRect2(AGVector2(b.base[0],b.base[1]),AGVector2(b.base[0]+b.dir[0],b.base[1]+b.dir[1]));
}


Scene *SceneNode::getScene()
{
  if(!scene)
    throw std::runtime_error("scene==0");
  return scene;
}

void SceneNode::clear()
{
  removeFromAllScenes(this);
}

void SceneNode::setVisible(bool v)
{
  mVisible=v;
}
bool SceneNode::visible() const
{
  return mVisible;
}

void SceneNode::setOrder(int o)
{
  order=o;
}
int SceneNode::getOrder() const
{
  return order;
}
