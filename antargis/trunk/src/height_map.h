#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H

#include "scene.h"
#include <ag_geometry.h>
#include <ag_xml.h>
#include "entptr.h"
#include <ag_messageobject.h>

class AntEntity;
class AntEntityPtr;
class Scene;
class Mesh;
class Terrain;

enum TerrainType { WATER=0, SAND, EARTH, GRASS, GRASS2, FOREST, ROCK, ROCK2, LASTTERRAIN};

#define FIRSTTERRAIN WATER

class HeightMap:public AGMessageObject
{
 public:
  HeightMap(Scene *pScene,int w,int h);
  virtual ~HeightMap();

  // get status
  float getHeight(float x,float y) const;
  AGVector3 getNormal(int x,int y) const;
  AGVector3 getNormalF(float x,float y) const;
  AGVector4 getVertex(int x,int y);
  float get(size_t x,size_t y) const;
  //float getGrass(size_t x,size_t y) const;

  void setTerrain(size_t x,size_t y,TerrainType t,float v);
  float getTerrain(size_t x,size_t y,TerrainType t) const;

  float getMean(float x,float y);
  size_t getW() const
  {
    return mW;
  }
  size_t getH() const
  {
    return mH;
  }

  // truncate pos at borders
  AGVector2 truncPos(const AGVector2 &p) const;

  // save load
  virtual void saveXML(Node &node) const;
  virtual void loadXML(const Node &node);

  // editing
  void setHeight(float height); // for whole plane
  void set(size_t x,size_t y,float height);

  virtual void mapChanged();  


  void setTerrainScale(TerrainType t,float s);
  float getTerrainValue(float x,float y,TerrainType t);
  TerrainType getTerrain(float x,float y);
  float getTerrainWeight(float x,float y);
  float getTerrainScale(float x,float y);


  void addChange(const AGVector2 &v);

  AGRect2 getChangeRect() const;

  virtual void newMap(int w,int h);

  AGSignal sigMapChanged;
  AGSignal sigMapChangedComplete;

  Scene *getScene();


 private:

  void loadBinary(const std::string &pName);
  void saveBinary(const std::string &pName) const;


  std::vector<float> mHeights;


  std::map<TerrainType,float> mTerrainScale;

  // sum of all should be one - or at least they get normalized to 1 and then the heighest is chosen and weighted with the mean value
  std::map<TerrainType,std::vector<float> > mTerrainTypes;

  size_t mW,mH;

  AGRect2 mChangeRect;
  size_t mChanges;

  Scene *mScene;

 protected:

  Terrain *mTerrain;
  std::string mName;
};

#endif
