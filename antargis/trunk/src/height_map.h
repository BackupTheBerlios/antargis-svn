#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H

#include "scene.h"
#include <ag_triangle.h>
#include <ag_xml.h>
#include "quadtree.h"
#include "entptr.h"

class AntEntity;
class AntEntityPtr;
class Scene;
class Mesh;
class TerrainMesh;

class HeightMap:public RubyObject
{
 public:
  HeightMap(int w,int h);
  virtual ~HeightMap();

  void setScene(Scene *pScene);
  Scene *getScene();

  // get status
  float getHeight(float x,float y) const;
  AGVector3 getNormal(int x,int y) const;
  AGVector4 HeightMap::getVertex(int x,int y);
  float get(size_t x,size_t y) const;
  float getGrass(size_t x,size_t y) const;

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

  TerrainMesh *getTerrainMesh();


  //protected:
  virtual void mapChanged();  
 private:

  std::vector<float> mHeights;
  std::vector<float> mGrass;
  size_t mW,mH;
 protected:
  Scene *mScene;

  TerrainMesh *mTerrainMesh;
};

#endif
