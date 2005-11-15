#ifndef ANIM_MESH_H
#define ANIM_MESH_H

#include "scene.h"
#include "ag_surface.h"
#include "ag_triangle.h"
#include "scenenode.h"

#include <vector>

struct Bone
{
  size_t id;
  AGVector3 pos,rot;
  AGMatrix4 mRelative,mAbsolute,mFinal;

  Bone *parent;
};

struct KeyFrame
{
  float time;
  std::vector<AGVector3> pos;
  std::vector<AGVector3> rot;
};

class AnimMeshData
{
  std::vector<AGVector3> pos;
  std::vector<AGVector2> uv;
  std::vector<AGVector3> normal;
  std::vector<int> bone;

  std::vector<size_t> indices;

  std::vector<Bone*> bones;
  std::vector<KeyFrame*> frames;
  float animTime;

  AGTexture mTexture;

  AGMatrix4 mBase;

 private:
  void setupJoints();

 public:
  AnimMeshData(const std::string &instr,float scale=1.0f,const std::string &pTexName="");

  void setTransform(const AGMatrix4 &m);
  const AGMatrix4 &getTransform() const;

  friend class AnimMesh;

#ifndef SWIG
  bool mRubyObject;
  VALUE mRUBY;
#endif
};

class AnimMesh:public SceneNode
{
  AnimMeshData *mData;
  size_t curKey;
  float mTime;
  AGMatrix4 mTransform;
  AGMatrix4 mComplete;
  AGVector4 mRot;
  AGVector4 mPos;

 public:
  AnimMesh(AnimMeshData *data);
  virtual ~AnimMesh();

  void setTransform(const AGMatrix4 &m);

  void setPos(const AGVector3 &p);
  void setRotation(const AGVector3 &r,float a);
  void setRotation(float r);

  AnimMeshData *getData();

  virtual void draw();
  virtual void advance(float time);
};

void AnimMesh_markfunc(void *ptr);

#endif
