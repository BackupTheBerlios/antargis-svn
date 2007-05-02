#ifndef IMPOSTOR_H
#define IMPOSTOR_H

#include <ag_texture.h>
#include <scene.h>

class AntImpostorData:public AGRubyObject
{
  AGTexture mTexture;
  AGSurface mSurface;
  
 public:
  // create impostor from a scene-node - to make it as simple as possible
  AntImpostorData(SceneNode *pNode,int width,int height);

  AGTexture &getTexture();
  AGSurface &getSurface();

  void mark();

 private:
  void create();
};

class AntImpostor:public SceneNode
{
  AntImpostorData *mData;
 public:
  AntImpostor(Scene *pScene,AntImpostorData *pData,const AGVector4 &pPos,float pRot);

  void draw();
  void drawDepth();
  void drawShadow();
  void drawPick();

 private:
  
};

#endif
