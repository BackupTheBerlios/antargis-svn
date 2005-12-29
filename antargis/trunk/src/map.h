#ifndef __MAP_H__
#define __MAP_H__

#include "height_map.h"

//FIXME: add quadtree

template<class T>
class QuadTree;

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
  std::vector<AntEntityPtr> getEntities(const std::string &pName);

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

  //  virtual void entsChanged();  
	
  virtual void mark();

  virtual void mapChanged();  
 private:
  typedef std::map<size_t,AntEntity*> EntityMap;

  EntityList mEntities;
  EntityMap mEntityMap;
  QuadTree<AntEntity> *mEntQuad;

  EntityList mToDel;

  int maxID;

};

AntMap *getMap();


#endif
