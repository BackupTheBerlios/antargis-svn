#ifndef _SCENE_NODE_H
#define _SCENE_NODE_H

#include <ag_triangle.h>
#include <ruby.h>
#include "rubyobj.h"

class Scene;

class SceneNode:public RubyObject
{
 public:
  SceneNode();
  virtual ~SceneNode();

  virtual void setScene(Scene *s);
  virtual void drawShadow();
  virtual void drawDepth();
  virtual void draw();
  virtual void drawPick();

  virtual void advance(float time);

  virtual size_t getTriangles() const;
  virtual AGVector4 lineHit(const AGLine3 &pLine) const;
  virtual void sort(const AGVector4 &pCamera);
  virtual void mapChanged();

  bool operator==(const SceneNode &n) const;

  virtual void setPos(const AGVector3&pPos);
  virtual void setRotation(float r);

  virtual bool transparent();

  virtual AGBox3 bbox();

  virtual void clear();

  Scene *getScene();

  void setVisible(bool v);
  bool visible() const;
  
 private:
  Scene *scene;
  bool mVisible;
};

typedef SceneNode *SceneNodePtr;
#endif
