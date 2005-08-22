#ifndef __MESH_H
#define __MESH_H

#include "scene.h"
#include "vertex_array.h"

class MeshData:public SceneNode
{
  VertexArray mArray;
  AGTexture mTexture;
  bool mWithTexture;
  bool mShadow;

 public:
  MeshData(const std::string &filename,float zoom,const std::string &pTexture="",bool pShadow=true);

  void draw();
  void drawShadow();
  void drawDepth();

  virtual size_t getTriangles() const;

  virtual AGVector4 lineHit(const AGLine3 &pLine) const
  {
    return mArray.lineHit(pLine);
  }

};

class Mesh:public SceneNode
{
  MeshData *mData;
  AGVector4 mPos;
  float mRotation;
  
 public:
  Mesh(MeshData &data,const AGVector4 &pPos,float pRot);

  void draw();
  void drawDepth();
  void drawShadow();

  virtual AGVector4 lineHit(const AGLine3 &pLine) const;

  virtual size_t getTriangles() const;
  void setPos(const AGVector3&pPos);
  void setRotation(float r);

 private:
  void begin();
  void end();
};

Mesh *toMesh(SceneNode *node);

#endif
