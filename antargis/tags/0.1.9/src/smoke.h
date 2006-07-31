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
  bool fire;
  bool enabled;

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

  void draw();

  void setEnabled(bool f);

};

#endif
