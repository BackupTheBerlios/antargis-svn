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
  void drawPick();

  // returns (0,0,0,0) if no collision
  // returns (x,y,z,1) if collision takes place
  AGVector4 lineHit(const AGLine3 &pLine) const;
  size_t getTriangles() const;

  virtual void mapChanged();

  virtual AGBox3 bbox();

 private:

  AGBox3 mBBox;

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



class Terrain
{
  typedef std::list<TerrainPieceVA*> Pieces;
  typedef std::list<WaterPiece*> WPieces;
  typedef std::list<SceneNode*> Nodes;
  Nodes mNodes;
  Pieces pieces;
  WPieces water;

  float w,h;
public:
  Terrain(HeightMap &map);

  virtual ~Terrain();

  virtual void mapChanged();
  void addToScenes();

};


#endif
