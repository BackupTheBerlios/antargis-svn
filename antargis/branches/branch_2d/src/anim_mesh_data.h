#ifndef ANIM_MESH_DATA_H
#define ANIM_MESH_DATA_H

#include "vertex_array.h"
#include "glsl.h"
#include "ag_texture.h"

struct KeyFrame
{
  float time;
  AGVector3 v;
};

struct Bone
{
  size_t id;
  AGVector3 pos,rot;
  AGMatrix4 mRelative,mAbsolute,mFinal;

  Bone *parent;

  std::vector<KeyFrame> rFrames,tFrames;


  AGVector3 interpolateRot(float t);
  AGVector3 interpolateTrans(float t);

  AGVector3 interpolate(const std::vector<KeyFrame> &frames,float t);
};

struct Animation
{
  float begin;
  float end;
  float fps;
  float len;
  bool loop;

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

class AnimMeshData:public AGRubyObject
{
  AntShaderProgram animShader;
  AntShaderProgram animShaderDepth;
  VertexArrayShader mArray;
  VertexArrayShader mArrayDepth;

  std::vector<AGVector3> pos;
  std::vector<AGVector2> uv;
  std::vector<AGVector3> normal;
  std::vector<int> bone;
  std::vector<float> bonef;
  std::vector<Uint16> boneui;

  std::vector<size_t> indices;

  std::vector<Bone*> bones;

  std::map<int,AGString> frameEvents;

  float animTime;

  AGTexture mTexture;

  AGMatrix4 mBase;

  std::map<AGString,Animation> mAnimations;

  AGBox3 mBBox;

  bool animate;

  AGString mName;

  bool events;

 private:
  void setupJoints();
  void setupArray();
  void loadAnt3(const AGData &instr,float scale,const AGFilename &pTexName);

 public:
  AnimMeshData(const AGFilename &xmlfile);
  ~AnimMeshData();

  AGBox3 bbox() const;

  void setEvents(bool e);

  void setTransform(const AGMatrix4 &m);
  const AGMatrix4 &getTransform() const;

  friend class AnimMesh;

  std::vector<std::string> getAnimations() const;
};

#endif
