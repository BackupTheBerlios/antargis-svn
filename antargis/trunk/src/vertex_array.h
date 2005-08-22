#ifndef __vertex_array_h
#define __vertex_array_h

#include <ag_triangle.h>
#include <vector>
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>

#include <ag_surface.h>

#include "scene.h"

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
  void addVertex(AGVector4 pVertex, AGVector4 pColor, AGVector3 pNormal, AGVector2 pTex);
  void addTriangle(size_t p0,size_t p1,size_t p2);

  void setColors(bool color);
  
  void draw();
  void init();

  void clear();

  size_t getTriangles() const
  {
    return mIndices.size()/3;
  }
  AGVector4 lineHit(const AGLine3 &pLine) const;

};

#endif
