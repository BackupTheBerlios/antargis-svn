#ifndef _SCENE_NODE_H
#define _SCENE_NODE_H

#include <ag_triangle.h>
#include <rubyobj.h>

class Scene;

#define TERRAIN_Z 1
#define WATER_Z 2
#define DECAL_Z 3
#define RING_Z 4
#define MESH_Z 5
#define TREE_Z 6
#define PARTICLE_Z 7


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

  virtual AGBox3 bbox() const;
  virtual AGRect2 getRect() const;

  virtual void clear();

  Scene *getScene();

  void setVisible(bool v);
  bool visible() const;

  void setOrder(int o);
  int getOrder() const;
  
 private:
  int order;

  Scene *scene;
  bool mVisible;
};

typedef SceneNode *SceneNodePtr;
#endif
