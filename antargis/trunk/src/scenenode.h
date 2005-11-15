#ifndef _SCENE_NODE_H
#define _SCENE_NODE_H

#include <ag_triangle.h>
#include <ruby.h>

class Scene;

class SceneNode
{
 public:
  SceneNode();
  virtual ~SceneNode();

  void setScene(Scene *s);
  virtual void drawShadow();
  virtual void drawDepth();
  virtual void draw();
  virtual void advance(float time);

  virtual size_t getTriangles() const;
  virtual AGVector4 lineHit(const AGLine3 &pLine) const;
  virtual void sort(const AGVector4 &pCamera);
  virtual void mapChanged();

  bool operator==(const SceneNode &n) const;

  virtual void setPos(const AGVector3&pPos);
  virtual void setRotation(float r);

  virtual bool transparent();
  
  public: // must be public, so that swig can set these
#ifndef SWIG
  bool mRubyObject;
  VALUE mRUBY;
#endif
 private:
  Scene *scene;
};

typedef SceneNode *SceneNodePtr;
#endif
