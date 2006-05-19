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
#include <ag_geometry.h>
#include <ag_color.h>


#include <set>
#include <vector>

class Mesh;
class SceneNode;
class Job;

#include "resource.h"

class AntEntity:public AGRubyObject
  {
  public:
    typedef std::list<SceneNode*> Meshes;
  private:
    int mID;
    std::string mName;

    AGVector3 mPos;   // 3d-position
    bool onGround;    // is this on the ground of the map?
    bool onWater;

    Job *mJob;        // current job

    std::list<Job*> mJobFinished;  // finished jobs, in cue, so that they get deleted next time possible

    std::set<std::string> mProvides;

    // energy and healing
    float mEnergy;
    float mHealSpeed;

    // morale and moralehealing
    float mMorale;
    float mMoraleHeal;

    bool mMoraleFlag;

    /// current food in stomach
    float mFood;
    /// food needed per second
    float mHunger; 
    /// hunger impact on energy
    float mHungerHitEnergy;
    /// hunger impact on morale
    float mHungerHitMorale;

    // moving
    float mMoveSpeed;

    float mAggression;

    std::string mType;

    Meshes mMeshes;
    std::map<SceneNode*,AGVector3> mMeshPos;

    int mVirtualY;

    AGColor mMinimapColor;
    bool mShowOnMinimap;
    float mMinimapSize;

    float mStrength;
    float mMoraleStrength;
    
    float mDefense;


  public: //virtually protected
    float mDir;  // direction in (0,360)
    Resource resource;

  public:
    AntEntity();
    AntEntity(const AGVector3 &p);
    AntEntity(const AGVector2 &p);
    virtual ~AntEntity();

    void setMinimapColor(const AGColor &c);
    void setMinimapSize(float f);
    AGColor getMinimapColor() const;
    bool showOnMinimap() const;
    float getMinimapSize() const;

    void setStrength(float f);
    float getStrength() const;
    void setMoraleStrength(float f);
    float getMoraleStrength() const;
    void setDefense(float f);
    float getDefense() const;

    void setMoraleFlag(bool f);


    // Positions
    AGVector3 getPos3D() const;
    AGVector2 getPos2D() const;
    virtual void setPos(const AGVector2 &p); // overwrite this only on static Entities, otherwise this gets called really (!) often
    void setPos(const AGVector3 &p);

    // IDs, names and types
    int getID() const;

    std::string getName() const;
    void setName(const std::string &pName);

    void setProvide(const std::string &pName,bool flag);
    bool provides(const std::string &pName) const;

    // saving and loading
    virtual std::string xmlName() const;

    virtual void saveXML(Node &node) const;
    virtual void loadXML(const Node &node);

    // jobs

  private:
    void setJob(Job *pJob); // only for internal use and reseting

  public:
    virtual void newRestJob(float pTime);
    virtual void newFetchJob(int p,AGVector2 &pTarget,const std::string &pWhat);
    virtual void newFetchJob(int p,AntEntity *pTarget,const std::string &pWhat);
    virtual void newMoveJob(int p,const AGVector2 &pTarget,float pnear=0.0);
    virtual void newFightJob(int p,AntEntity *target);
    virtual void delJob();

    bool isMoving() const;

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

    void setMoraleHealSpeed(float f);

    void setHunger(float f);

    // aggression - handling

    void setAggression(float agg);
    float getAggression() const;
    
    virtual void eventDie(); // energy too low
    virtual void eventDefeated(); // morale too low
    virtual void eventHaveDefeated(AntEntity *e);
    virtual void eventMoraleLow();

    // called by FightJob
    virtual void eventStartFighting();
    virtual void eventStartMoving();

    virtual void eventHitWaterMark(bool fromAbove);

    void sigDefeated(); // morale too low
    void sigJobFinished();

    float getEnergy() const;
    float getMorale() const;

    bool canFight() const;
    bool isFighting() const;

    // appearance

    // FIXME: this shouldn't be virtual, because it gets called too often!
    //    virtual std::string getTexture() const;

    // anything below shouldn't be used by ruby-functions

    void setMesh(SceneNode *pMesh);
    Meshes getMesh();
    SceneNode *getFirstMesh();
    void addMesh(SceneNode *pMesh,const AGVector3 &v);

    // used only by *Jobs
    void setDirection(float pAngle);
    float getDirection() const;

    void decEnergy(float amount);
    void decMorale(float amount);
    void incMorale(float pTime);

    float getFood() const;
    void incFood(float v);
    bool isStarving() const;

    void setOnWater(bool p);
    void setOnGround(bool p);

    // used only by Map - so that Position gets updated, when onGround
    void eventMapChanged();
    virtual void move(float pTime); // move entity FIXME: del move

    void mark();

    void clear();
    void clearMeshes();

    void setVisible(bool v);

    virtual void animationEvent(const std::string &pName);

    void heal(float pTime);

    AntEntity *getFightTarget();

  private:
    void init();
    void updatePos(const AGVector3 &p);

    void starve(float pTime);

    void loadJob(const Node &pNode);
  };


#endif
