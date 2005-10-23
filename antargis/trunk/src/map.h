#ifndef __MAP_H__
#define __MAP_H__

#include <ag_triangle.h>
#include <ag_xml.h>
#include "quadtree.h"

class AntEntity;
class AntEntityPtr;
class Scene;
class Mesh;
class TerrainMesh;

class HeightMap
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

//FIXME: add quadtree

class AntMap:public HeightMap
{
 public:
  typedef std::list<AntEntity*> EntityList;

  AntMap(int w,int h);
  ~AntMap();
  
  virtual void insertEntity(AntEntity *e);
  virtual void removeEntity(AntEntity *p);
  void clear();

  int getNewID();
  
  EntityList getEntities(const AGRect2&r);
  EntityList getAllEntities();
  std::vector<AntEntityPtr> getAllEntitiesV();

  AntEntity *getEntity(const Mesh &pMesh);
  AntEntity *getEntity(int id) const;
  AntEntity *getByName(const std::string &pName);

  AntEntity *getNext(AntEntity *me,const std::string &pType);

  virtual AntEntity *loadEntity(const xmlpp::Node &node);

  void saveXML(xmlpp::Node &node) const;
  void loadXML(const xmlpp::Node &node);

  void saveMap(const std::string &pFilename);
  void loadMap(const std::string &pFilename);

  void move(float pTime);

  AGVector3 getPos(const AGVector2 &pPos) const;

  virtual void entsChanged();  


  virtual void mapChanged();  
 private:
  typedef std::map<size_t,AntEntity*> EntityMap;

  EntityList mEntities;
  EntityMap mEntityMap;
  QuadTree<AntEntity> mEntQuad;

  EntityList mToDel;

  int maxID;

 public:
  VALUE mRUBY;
  bool mRubyObject;
  friend void AntMap_markfunc(void *ptr);
};

AntMap *getMap();


#endif
