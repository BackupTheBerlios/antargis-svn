#ifndef __MESH_H
#define __MESH_H

#include "scene.h"
#include "vertex_array.h"

#include <map>

#include "mesh_data.h"

class Mesh:public SceneNode
{
  MeshData *mData;
  float mRotation;
  AGVector4 mColor;
  bool mVisible;
  
 public:
  Mesh(Scene *pScene);
  Mesh(Scene *pScene,MeshData &data,const AGVector4 &pPos,float pRot);
  virtual ~Mesh();

  void draw();
  void drawDepth();
  void drawShadow();
  void drawPick();

  //  AGBox3 bbox() const;

  virtual AGVector4 lineHit(const AGLine3 &pLine) const;

  virtual size_t getTriangles() const;
  void setRotation(float r);
  void setColor(const AGVector4 &pColor);
  void setVisible(bool v);

  MeshData *getData();

  void mark();

  virtual bool transparent();


 private:
  void begin();
  void end();
};

Mesh *toMesh(SceneNode *node);

#endif
