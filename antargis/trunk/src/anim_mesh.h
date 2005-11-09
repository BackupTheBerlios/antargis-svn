#ifndef ANIM_MESH_H
#define ANIM_MESH_H

#include "ag_triangle.h"
#include "scenenode.h"

#include <vector>

struct Bone
{
  size_t id;
  AGVector3 pos,rot;

  Bone *parent;
};

struct KeyFrame
{
  float time;
  std::vector<AGVector3> pos; // usually =0
  std::vector<AGVector3> rot;
};

struct Skeleton
{
  std::vector<Bone*> bones;
  std::vector<KeyFrame*> frames;
};

class AnimMeshData
{
  std::vector<AGVector3> pos;
  std::vector<AGVector2> uv;
  std::vector<AGVector3> normal;
  std::vector<size_t> bone;

  std::vector<size_t> indices;

  Skeleton skeleton;

 public:
  AnimMeshData(const std::string &instr);

  friend class AnimMesh;
};

class AnimMesh:public SceneNode
{
  AnimMeshData *mData;
  size_t curKey;
  float mTime;

 public:
  AnimMesh(AnimMeshData *data);
  virtual ~AnimMesh();

  virtual void draw();
};

#endif
