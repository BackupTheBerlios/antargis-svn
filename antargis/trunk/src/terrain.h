#ifndef __antargis_gl_terrain_h
#define __antargis_gl_terrain_h

#include <vector>

#include "scene.h"
#include "ag_triangle.h"
#include <ag_surface.h>
#include <math.h>

#include "vertex_array.h"
#include "water.h"

#include <ag_xml.h>

#include "map.h"

using namespace std;

#define THREE_LAYERS


class TerrainPieceVA:public SceneNode
{
 public:
  TerrainPieceVA(HeightMap &map,int x,int y,int w,int h,const AGVector4 &pPos);
  virtual ~TerrainPieceVA()
    {
    }

  void draw();
  void drawShadow();
  void drawDepth();

  // returns (0,0,0,0) if no collision
  // returns (x,y,z,1) if collision takes place
  AGVector4 lineHit(const AGLine3 &pLine) const;
  size_t getTriangles() const;

  virtual void mapChanged();
 private:
  size_t mXs,mYs;
  size_t mW,mH;
  //  AGVector4 mPosition;
  HeightMap *mMap;
  VertexArray mEarthArray;
  VertexArray mGrassArray;
#ifdef THREE_LAYERS
  VertexArray mGrass2Array;
#endif
  AGTexture mEarth,mGrass,mGrassShadow;
};



class TerrainMesh:public SceneNode
{
  typedef std::list<TerrainPieceVA*> Pieces;
  typedef std::list<WaterPiece*> WPieces;
  Pieces pieces;
  WPieces water;
public:
  TerrainMesh(HeightMap &map);

  virtual ~TerrainMesh();

  void advance(float t);
  void draw();
  void drawShadow();
  void drawDepth();

  void sort(const AGVector4 &camera); // sort drawing, so that things get drawn from back to front
  virtual AGVector4 lineHit(const AGLine3 &pLine) const;

  virtual size_t getTriangles() const;

  virtual void mapChanged();


};

TerrainMesh *toTerrainMesh(SceneNode *n);


#endif
