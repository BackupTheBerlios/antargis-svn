#ifndef __antargis_gl_water_h
#define __antargis_gl_water_h

#include "glsl.h"
#include "scene.h"
#include "ag_texturecache.h"
#include "ag_geometry.h"
#include "ag_texture.h"
#include "vertex_array.h"
#include "map.h"
#include <math.h>

class AntWaterShader;

class WaterPiece:public SceneNode
{
  AGTexture tex;
 public:
  WaterPiece(Scene *pScene,HeightMap &map,int x,int y,int w,int h,const AGVector4 &pos);
  virtual ~WaterPiece();

  void draw();
  void drawPick();

  void advance(float t);
  size_t getTriangles() const;
  bool transparent();

  virtual void mapChanged();

 private:
  void addTriangle(int x0,int y0,int x1,int y1,int x2, int y2);

 private:
  VertexArray mArray;
  int mX;
  int mY;
  int mW;
  int mH;
  HeightMap *mMap;
  int step;
  AntWaterShader *mShader;
};
#endif

