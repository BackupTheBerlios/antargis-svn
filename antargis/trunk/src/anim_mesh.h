#ifndef ANIM_MESH_H
#define ANIM_MESH_H

#include "scene.h"
#include "ag_surface.h"
#include "ag_triangle.h"
#include "scenenode.h"

#include <vector>
#include <map>

#include "anim_mesh_data.h"

class AnimMesh:public SceneNode
{
  AnimMeshData *mData;
  size_t curKey;
  float mTime;
  AGMatrix4 mTransform;
  AGMatrix4 mComplete;
  AGVector4 mRot;
  AGVector4 mPos;
  std::string mAnimName;
  Animation *mAnimation;

  std::vector<AGMatrix4> mMatrices;

 public:
  AnimMesh(AnimMeshData *data);
  virtual ~AnimMesh();

  void setTransform(const AGMatrix4 &m);

  std::string getAnimation() const;
  void setAnimation(const std::string &pName);

  void setPos(const AGVector3 &p);
  void setRotation(const AGVector3 &r,float a);
  void setRotation(float r);

  AnimMeshData *getData();

  virtual void draw();
  virtual void drawPick();
  virtual void advance(float time);

  AGBox3 bbox();
	
  virtual void mark();

 private:

  void drawPrivate(bool textured);

  void update();
};

#endif
