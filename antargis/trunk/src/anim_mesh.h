#ifndef ANIM_MESH_H
#define ANIM_MESH_H

#include "scene.h"
#include "ag_surface.h"
#include "ag_triangle.h"
#include "scenenode.h"

#include <vector>
#include <map>

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

struct Animation
{
  float begin;
  float end;
  float fps;
  float len;

  Animation()
  {
    begin=end=fps=1;
    len=0;
  }
  Animation(float b,float e,float f):begin(b),end(e),fps(f)
  {
    len=end-begin;
  }
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

  std::map<std::string,Animation> mAnimations;

  AGBox3 mBBox;

 private:
  void setupJoints();
  void loadAnt3(const std::string &instr,float scale,const std::string &pTexName);

 public:
  AnimMeshData(const std::string &xmlfile);

  AGBox3 bbox() const;

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

 private:

  void drawPrivate(bool textured);

  void update();
};

void AnimMesh_markfunc(void *ptr);

#endif
