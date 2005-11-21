#ifndef __vertex_array_h
#define __vertex_array_h

#include "scene.h"
#include <ag_triangle.h>
#include <vector>
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>

#include <ag_surface.h>

class VertexArray
{
  std::vector<AGVector4> mVertices,mColors;
  std::vector<AGVector3> mNormals;
  std::vector<AGVector2> mTexCoords;
  std::vector<Uint16>  mIndices;
  bool bColor;
  bool mBuffers;
  bool mChanged;

  unsigned int mVertexBuffer,mColorBuffer,mNormalBuffer,mTexBuffer,mIndexBuffer;

  AGBox3 bbox;

 public:
  VertexArray();
  ~VertexArray();
  void addVertex(AGVector4 pVertex, AGVector4 pColor, AGVector3 pNormal, AGVector2 pTex);
  void addTriangle(size_t p0,size_t p1,size_t p2);

  void setColors(bool color);
  void setBuffers(bool pBuffers);
  
  void draw();
  void drawPick();
  void init();

  void clear();
  void update();

  size_t getTriangles() const
  {
    return mIndices.size()/3;
  }
  
  size_t getIndex(size_t i);
  AGVector4 getVertex(size_t i);
  AGVector4 getColor(size_t i);
  AGVector3 getNormal(size_t i);
  AGVector2 getTexCoord(size_t i);

  void setTexCoord(size_t i,const AGVector2 &t);

  AGVector4 lineHit(const AGLine3 &pLine) const;

};

#endif
