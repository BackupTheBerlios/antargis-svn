#ifndef _SCENE_NODE_H
#define _SCENE_NODE_H

#include <ag_geometry.h>
#include <ag_rubyobj.h>

class Scene;

#define TERRAIN_Z 1
#define WATER_Z 2
#define DECAL_Z 3
#define RING_Z 4
#define MESH_Z 5
#define TREE_Z 6
#define PARTICLE_Z 7

/**
   A scenenode represents any part of a scene. It holds information
   about position, drawing order, bounding box and visibility.
   A scenenode is created for exactly one scene and it can't be
   assigned to any other scene, which shouldn't be necessary.

   Scene and SceneNode give each other information about their 
   "destroy-state". That means the destructor notifies the other object.

*/
class SceneNode:public AGRubyObject
{
 public:
  SceneNode(Scene *s,const AGVector4 &pPos,const AGBox3 &pBox);
  virtual ~SceneNode();

  virtual void resetScene();
  virtual void drawShadow();
  virtual void drawDepth();
  virtual void draw();
  virtual void drawPick();

  virtual void advance(float time);

  virtual size_t getTriangles() const;
  virtual AGVector4 lineHit(const AGLine3 &pLine) const;
  virtual void sort(const AGVector4 &pCamera);

  bool operator==(const SceneNode &n) const;

  virtual void setPos(const AGVector3&pPos);
  AGVector4 getPos() const;
  virtual void setRotation(float r);
  void setBBox(const AGBox3 &pBox);

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
  int mOrder;

  Scene *mScene;
  bool mVisible;

  AGVector4 mPos;
  AGBox3 mBBox;
};

typedef SceneNode *SceneNodePtr;
#endif
