/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * entity.h
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

#ifndef ANT_ENTITY_H
#define ANT_ENTITY_H

#include "ag_xml.h"
#include "ant_geometry.h"
#include "ant_rect.h"

#include <ruby.h>
#include <set>
#include <vector>
#include <algorithm>

class VoxelImage;
class Job;
class AntHero;

class Resource
{
  std::map<std::string,int> r;
 public:
  Resource();
  int get(const std::string &pName);
  void add(const std::string &pName,int value);
  void set(const std::string &pName,int value);

  std::map<std::string,int> getAll() const;

  void takeAll(Resource &r);
};

class AntEntity
  {
    int mID;
    std::string mName;

    Pos3D mPos;       // 3d-position
    bool onGround;    // is this on the ground of the map?

    Job *mJob;        // current job

    std::list<Job*> mJobFinished;  // finished jobs, in cue, so that they get deleted next time possible

    // energy and healing
    float mEnergy;
    float mHealSpeed;

    // morale and moralehealing
    float mMorale;
    float mMoraleHeal;

    // moving
    float mMoveSpeed;

    float mAggression;

    std::string mType;

    VoxelImage*mSurface;

    int mVirtualY;

  public: //virtually protected
    int mDirNum; // set public for swig
    Resource resource;

  public:
    AntEntity();
    AntEntity(const Pos3D &p);
    AntEntity(const Pos2D &p);
    virtual ~AntEntity();


    // Positions
    Pos3D getPos3D() const;
    Pos2D getPos2D() const;
    virtual void setPos2D(const Pos2D &p); // overwrite this only on static Entities, otherwise this gets called really (!) often
    void setPos3D(const Pos3D &p);


    // IDs, names and types
    int getID() const;

    std::string getName() const;
    void setName(const std::string &pName);

    void setType(const std::string &pType);
    std::string getType() const;


    // saving and loading
    virtual std::string xmlName() const;

    virtual void saveXML(xmlpp::Node &node) const;
    virtual void loadXML(const xmlpp::Node &node);

    // jobs

  private:
    void setJob(Job *pJob); // only for internal use and reseting

  public:
    virtual void newRestJob(int pTime);
    virtual void newFetchJob(int p,Pos2D &pTarget,const std::string &pWhat);
    virtual void newMoveJob(int p,const Pos2D &pTarget,int pnear=0);
    virtual void newFightJob(int p,AntEntity *target);

    bool hasJob() const;

    virtual void eventNoJob();
    virtual void eventJobFinished();
    virtual void eventGotNewJob();
    virtual void eventGotFight(AntEntity*pOther);

    // set/get speeds
    void setSpeed(float f);
    float getSpeed() const;

    float getHealSpeed() const;
    void setHealSpeed(float f);

    // aggression - handling

    void setAggression(float agg);
    float getAggression() const;
    
    virtual void eventDie(); // energy too low
    virtual void eventDefeated(); // morale too low

    float getEnergy() const;
    float getMorale() const;

    // appearance

    // FIXME: this shouldn't be virtual, because it gets called too often!
    virtual std::string getTexture() const;
    void setVirtualY(int y);

    // anything below shouldn't be used by ruby-functions
//#ifndef SWIG
    void setSurface(VoxelImage *i);
    VoxelImage*getSurface();
    void updateSurface();

    // used only by *Jobs
    void setDirection(const Pos2D &p);

    void decEnergy(float amount);
    void decMorale(float amount);


    // used only by Map - so that Position gets updated, when onGround
    void eventMapChanged();
    virtual void move(float pTime); // move entity FIXME: del move

    // used only View
    virtual Rect2D getRect() const;
    int getVirtualY() const;
//#endif

  public: // must be public, so that swig can set these
    bool mRubyObject;
    VALUE mRUBY;

    friend void AntEntity_markfunc(void *ptr);

  private:
    void init();
  };

class AntEntityPtr
{
 public:
  AntEntityPtr(const AntEntityPtr &p);
  AntEntityPtr(AntEntity *p);
  AntEntityPtr();

  AntEntity *get();

 private:
  AntEntity *p;
};


#endif
