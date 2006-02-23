#ifndef SMOKE_H
#define SMOKE_H

#include "scenenode.h"
#include "ag_texture.h"

class Smoke:public SceneNode
{
  struct Piece
  {
    AGVector3 pos;
    AGVector3 speed;
    AGVector3 color;
    float lived;
    float light;
    float size;
    
  };

  std::list<Piece*> mPieces;
  float maxtime,freq;
  float mtime;
  float stime;
  AGVector3 p;
  bool fire;

  AGBox3 mBBox;

  AGTexture mTexture;


 public:
  Smoke(Scene *pScene,float f);
  void setFire(bool f);
  void setMaxTime(float m);
  void advance(float time);

  virtual bool transparent()
  {
    return true;
  }

  AGBox3 bbox() const;

  virtual void setPos(const AGVector3&pPos);

  void draw();
};

#endif
