#ifndef SMOKE_H
#define SMOKE_H

#include "scenenode.h"
#include "ag_surface.h"

class Smoke:public SceneNode
{
  struct Piece
  {
    AGVector3 pos;
    AGVector3 speed;
    float lived;
    float light;
    float size;
    
  };

  std::list<Piece*> mPieces;
  float maxtime,freq;
  float mtime;
  float stime;
  AGVector3 p;

  AGTexture mTexture;


 public:
  Smoke(const AGVector3 &pStart,float f);
  void advance(float time);

  virtual bool transparent()
  {
    return true;
  }


  virtual void setPos(const AGVector3&pPos);

  void draw();
};

#endif
