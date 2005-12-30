#ifndef __antargis_gl_water_h
#define __antargis_gl_water_h

#include "glsl.h"
#include "scene.h"
#include "ag_texturecache.h"
#include "ag_triangle.h"
#include "ag_surface.h"
#include "vertex_array.h"
#include "map.h"
#include <math.h>

class WaterPiece:public SceneNode
{
  AGTexture tex;
 public:
  WaterPiece(HeightMap &map,int x,int y,int w,int h,const AGVector4 &pos);
  virtual ~WaterPiece();

  void draw();
  void drawPick();

  void advance(float t);
  size_t getTriangles() const;
  bool transparent();

  AGBox3 bbox() const;

  virtual void mapChanged();

 private:
  void addTriangle(int x0,int y0,int x1,int y1,int x2, int y2);

 private:
  VertexArray mArray;
  int mX;
  int mY;
  int mW;
  int mH;
  AGBox3 mBBox;
  AGVector4 mPos;
  HeightMap *mMap;
  int step;
};
#endif

