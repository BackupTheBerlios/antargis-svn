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

class Terrain;

class TerrainPieceVA:public SceneNode
{
 public:
  TerrainPieceVA(Terrain *t,HeightMap &map,int x,int y,int w,int h,const AGVector4 &pPos);
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

  virtual AGBox3 bbox() const;

 private:

  Terrain *mTerrain;

  AGBox3 mBBox;

  size_t mXs,mYs;
  size_t mW,mH;
  HeightMap *mMap;

  VertexArray m3dArray;
  //  VertexArray mGrassArray;

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

  AGTexture m3D;

  AGTexture mGrass;
  HeightMap *mMap;

public:
  Terrain(HeightMap &map);

  virtual ~Terrain();

  virtual void mapChanged();
  virtual void mapChangedComplete();
  void addToScenes();

  AGTexture *get3dTexture();
  AGTexture *getGrassTexture();

 private:
  void init();
};


#endif
