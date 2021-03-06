/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * map.h
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "Antargis" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */

#ifndef __MAP_H__
#define __MAP_H__

#include "height_map.h"

//FIXME: add quadtree

template<class T>
class QuadTree;

class AnimMesh;

class AntMap:public HeightMap
{
 public:
  typedef std::list<AntEntity*> EntityList;

  AntMap(Scene *pScene,int w,int h);
  ~AntMap();
  
  virtual void insertEntity(AntEntity *e);
  virtual void removeEntity(AntEntity *p);
  void clear();

  int getNewID();
  void useID(int id);

  void newMap(int w,int h);
  
  EntityList getEntities(const AGRect2&r);
  EntityList getAllEntities();
  std::vector<AntEntityPtr> getAllEntitiesV();
  std::vector<AntEntityPtr> getEntities(const AGString &pName);

  AntEntity *getEntity(const Mesh &pMesh);
  AntEntity *getEntity(const AnimMesh &pMesh);
  AntEntity *getEntity(int id) const;
  AntEntity *getByName(const AGString &pName);

  AntEntity *getNext(AntEntity *me,const AGString &pType,size_t atLeast=0);
  std::vector<AntEntityPtr> getNextList(AntEntity *me,const AGString &pType,size_t atLeast=0);

  virtual AntEntity *loadEntity(const Node &node);

  void saveXML(Node &node) const;
  void loadXML(const Node &node);

  void saveMap(const AGFilename &pFilename);
  virtual void loadMap(const AGFilename &pFilename);
  virtual void loadMapFromMemory(const AGData &Memory);

  void move(float pTime);

  AGVector3 getPos(const AGVector2 &pPos) const;

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
