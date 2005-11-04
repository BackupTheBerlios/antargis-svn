#ifndef __antargis_gl_water_h
#define __antargis_gl_water_h

#include "glsl.h"
#include "scene.h"
#include "ag_texturecache.h"
#include "ag_triangle.h"
#include "ag_surface.h"
#include "vertex_array.h"
#include <math.h>

#ifdef SLOW_WATER
class WaterPiece:public SceneNode
{
  AGTexture tex;
 public:
  WaterPiece(const AGVector4 &pos);
  virtual ~WaterPiece();

  void tick();

  void draw();

  void advance(float t);
  size_t getTriangles() const;

 private:
  void drawPrivate(bool texture=false);

  AGVector4 calcBaseVertex(int x,int y);
  float getArcX(int x,int y);
  float getArcY(int x,int y);
  AGVector4 getV(int x,int y);
  AGVector4 getHeight(int x,int y);
  AGVector4 getNormal(int x,int y);
  void drawVertex(AGVector4 v,AGVector4 n,float tx,float ty, bool texture);

 private:
  Uint32 mFrameCount,mLast;
  int mW;
  int mH;
  AGVector4 mPos;

};
#else
class WaterPiece:public SceneNode
{
  AGTexture tex;
 public:
  WaterPiece(const AGVector4 &pos);
  virtual ~WaterPiece();

  void draw();

  void advance(float t);
  size_t getTriangles() const;

 private:
  void addTriangle(int x0,int y0,int x1,int y1,int x2, int y2);

 private:
  VertexArray mArray;
  int mW;
  int mH;
};
#endif
#endif
