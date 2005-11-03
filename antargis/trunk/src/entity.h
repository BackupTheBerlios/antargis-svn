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

#include "scene.h"
#include <ag_xml.h>
#include <ag_triangle.h>


#include <ruby.h>
#include <set>
#include <vector>

class Mesh;
class SceneNode;
class Job;

// FIXME: maybe remove Resource from c++??
class Resource
{
  std::map<std::string,float> r;
 public:
  Resource();
  float get(const std::string &pName);
  void add(const std::string &pName,float value);
  void sub(const std::string &pName,float value);
  void set(const std::string &pName,float value);

  std::map<std::string,float> getAll() const;

  void takeAll(Resource &r);
};

class AntEntity
  {
  public:
    typedef std::list<SceneNode*> Meshes;
  private:
    int mID;
    std::string mName;

    AGVector3 mPos;   // 3d-position
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

    Meshes mMesh;
    std::map<SceneNode*,AGVector3> mMeshPos;

    int mVirtualY;

  public: //virtually protected
    float mDir;  // direction in (0,360)
    Resource resource;

  public:
    AntEntity();
    AntEntity(const AGVector3 &p);
    AntEntity(const AGVector2 &p);
    virtual ~AntEntity();


    // Positions
    AGVector3 getPos3D() const;
    AGVector2 getPos2D() const;
    virtual void setPos(const AGVector2 &p); // overwrite this only on static Entities, otherwise this gets called really (!) often
    void setPos(const AGVector3 &p);

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
    virtual void newFetchJob(int p,AGVector2 &pTarget,const std::string &pWhat);
    virtual void newFetchJob(int p,AntEntity *pTarget,const std::string &pWhat);
    virtual void newMoveJob(int p,const AGVector2 &pTarget,int pnear=0);
    virtual void newFightJob(int p,AntEntity *target);
    virtual void delJob();

    virtual void resourceChanged();

    bool hasJob() const;

    AGRect2 getRect() const;

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
    //    virtual std::string getTexture() const;

    // anything below shouldn't be used by ruby-functions

    void setMesh(Mesh *pMesh);
    Meshes getMesh();
    void addMesh(SceneNode *pMesh,const AGVector3 &v);

    // used only by *Jobs
    void setDirection(float pAngle);

    void decEnergy(float amount);
    void decMorale(float amount);


    // used only by Map - so that Position gets updated, when onGround
    void eventMapChanged();
    virtual void move(float pTime); // move entity FIXME: del move


  public: // must be public, so that swig can set these
    bool mRubyObject;
    VALUE mRUBY;

  private:
    void init();
    void updatePos(const AGVector3 &p);

    friend void AntEntity_markfunc(void *ptr);

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

void AntEntity_markfunc(void *ptr);

#endif
