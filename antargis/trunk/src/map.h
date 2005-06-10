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

#ifndef ANT_MAP_H
#define ANT_MAP_H

#include "decast.h"
#include "quadtree.h"
#include "voxel_gen.h"

#include "ag_xml.h"
#include <ag_fs.h>

#include <set>
#include <vector>
#include <algorithm>

#include "entity.h"


/*
class HeightMap
  {
    SplineMapD mHeight,mGrass;

  public:
    HeightMap(int w,int h);


  };
*/

/*class MapListener
{
 public:
  MapListener();
  virtual ~MapListener();

  virtual void mapUpdate();
  };*/

//class AntPlayer;

class AntargisMap
  {
    SplineMapD mHeight,mGrass;
    QuadTree<AntEntity> mEntities;
    std::list<AntEntity*> mEntList;
    std::map<int,AntEntity*> mEntityMap;

    bool mPaused;
    int mW,mH;

    int maxID;

    bool mUpdated;
    bool mHeightChanged;

    std::list<AntEntity*> mToDel;

    size_t mGCcalls;

    std::set<AntEntity*> mLightingEntities;

  public:
    AntargisMap(int w,int h);
    virtual ~AntargisMap();

    float getHeight(const Pos2D &p) const;
    float getGHeight(const Pos2D &p) const;

    SplineMapD getPatchH(int x,int y) const;
    SplineMapD getPatchG(int x,int y) const;

    int getNewID();
    
    Pos3D getNormal(int x,int y) const;

    bool updated() const;
    bool heightChanged() const;

    size_t getGCcalls() const;
    void resetGCcalls();

    AntEntity *getNext(AntEntity *me,const std::string &pType);

    virtual void insertEntity(AntEntity *e);

    // x,y = position
    // h = amount
    // r = radius
    void addPyramid(int x,int y,int h,int r);
    void addFlat(int x,int y,int h,int r);
    void setAllWater();
    void setAllLand();

    std::list<AntEntity*> getEntities(const AntRect&r);
    std::list<AntEntity*> getAllEntities();

    Pos3D getPos3D(const Pos2D &p) const;

    void move(float pTime);
    void frameEnd();

    void clear();

    virtual void removeEntity(AntEntity *p);

    // align to rect / >0 and <width and so 
    Pos2D truncPos(const Pos2D &p) const;

    AntEntity *getEntity(int id) const
      {
        std::map<int,AntEntity*>::const_iterator i=mEntityMap.find(id);
        if(i==mEntityMap.end())
          return 0;
        return i->second;
      }

    AntEntity *getByName(const std::string &pName);

    bool paused()
    {
      return mPaused;
    }
    
    void pause()
    {
      mPaused=true;
    }
    void unpause()
    {
      mPaused=false;
    }
    
    Pos2D getMaxPos() const;

    void saveMap(const std::string &pFilename);
    void loadMap(const std::string &pFilename);

    virtual AntEntity *loadEntity(const xmlpp::Node &node);

    void saveXML(xmlpp::Node &node) const;
    void loadXML(const xmlpp::Node &node);
    
    int width() const;
    int height() const;

    int getDarkness(const Pos2D &pPos) const;
    void setLight(AntEntity *e);


    VALUE mRUBY;
    bool mRubyObject;
    friend void AntargisMap_markfunc(void *ptr);

  };

AntargisMap *getMap();


#endif
