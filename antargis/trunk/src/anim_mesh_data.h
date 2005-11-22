#ifndef ANIM_MESH_DATA_H
#define ANIM_MESH_DATA_H


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

class AnimMeshData:public RubyObject
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

  bool animate;

 private:
  void setupJoints();
  void loadAnt3(const std::string &instr,float scale,const std::string &pTexName);

 public:
  AnimMeshData(const std::string &xmlfile);

  AGBox3 bbox() const;

  void setTransform(const AGMatrix4 &m);
  const AGMatrix4 &getTransform() const;

  friend class AnimMesh;
};

#endif
