#ifndef __MESH_H
#define __MESH_H

#include "scene.h"
#include "vertex_array.h"

#include <map>

#include "mesh_data.h"

/** A mesh is a scenenode, that displays a mesh ;)
    ok, now for the real info.
    it contains a MeshData object, color and positional information.
    Actual drawing is done by MeshData.

*/
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

#endif
