#ifndef __antargis_gl_tree_h
#define __antargis_gl_tree_h

#include "scene.h"
#include "ag_triangle.h"
#include "ag_surface.h"
#include "vertex_array.h"

class GLTree:public SceneNode
{
  AGTexture firTex,trunkTex;

 public:
  GLTree(AGVector4 p,float h=9);

  virtual ~GLTree();

  void draw();
  void drawDepth();
  void drawShadow();
  void draw(bool texture=false);

  void drawVertex(const AGVector4 &v,AGVector4 n,float t1,float t2);

 private:
  void init();


  AGVector4 position;
  float height;
  float wu,wl;
  VertexArray mTrunkVA, mFirVA;
};

#endif
