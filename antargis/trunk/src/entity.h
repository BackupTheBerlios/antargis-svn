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
    int mPlayerID;
  protected:
    Pos3D mPos;
    Job *mJob;
    std::list<Job*> mJobFinished;
    float mEnergy;
    float mMorale;
    float mMoraleHeal;
    float mHealSpeed;
    float mMoveSpeed;
    bool onGround;
    float mCondition;
    float mConditionFall; // when used, how much per time -- hero is stronger here
    float mConditionHeal; // refilling
    float mAggression;

    std::string mType;

    VoxelImage*mSurface;


    AntEntity *pBoss;
    std::vector<AntEntity*> mClients;

  public: //virtually protected
    int mDirNum; // set public for swig
    Resource resource;
    std::map<std::string,std::string> mVars;
  public:
    AntEntity();
    AntEntity(const Pos3D &p);
    AntEntity(const Pos2D &p);
    virtual ~AntEntity();
    Pos3D getPos3D() const;
    Pos2D getPos2D() const;

    int getPlayerID() const;
    void setPlayerID(int id);

    void setVar(std::string n,std::string v);
    std::string getVar(std::string n) const;

    void setType(const std::string &pType);
    std::string getType() const;

    virtual std::string xmlName() const
      {
        return "antEntity";
      }

    virtual void saveXML(xmlpp::Node &node) const;
    virtual void loadXML(const xmlpp::Node &node);

    int getID() const
      {
        return mID;
      }

    void setJob(Job *pJob);

    void setAggression(float agg);
    float getAggression() const;
    
    virtual std::string getTexture() const
    {
      return "";
    }
    
    virtual void setDirection(const Pos2D &p)
    {
      Pos2D p2=p.normalized();
      if(p2.x<-0.38) // sin(PI/4)
        {
          if(p2.y<-0.38)
            mDirNum=1; // down left
          else if(p2.y<0.38)
            mDirNum=8; // left
          else
            mDirNum=7; // up left
        }
      else if(p2.x<0.38)
        {
          if(p2.y<-0.38)
            mDirNum=2; // down
          else if(p2.y<0.38)
            mDirNum=1; // undefinied
          else
            mDirNum=6; // up
        }
      else
      {
          if(p2.y<-0.38)
            mDirNum=3; // down right
          else if(p2.y<0.38)
            mDirNum=4; // right
          else
            mDirNum=5; // up right
      }
      updateSurface();
    }

    virtual void setPos2D(const Pos2D &p);
    void setPos3D(const Pos3D &p);

    void mapChanged();
    void setSurface(VoxelImage *i);
    virtual VoxelImage*getSurface();
    virtual void updateSurface();

    virtual std::string getSurfaceName() const;
    virtual int getVirtualY() const;

    /** do anything in given time frame */
    virtual void move(float pTime);

    virtual Rect2D getRect() const;

    virtual void noJob();
    virtual void jobFinished();
    virtual void gotNewJob();
    bool isJobFinished() const;

    bool hasJob() const
      {
        return mJob;
      }

    virtual float getHealSpeed() const
      {
        return mHealSpeed;
      }

    void setSpeed(float f);
    float getSpeed() const;
    void setHealSpeed(float f)
    {
      mHealSpeed=f;
    }

    void decEnergy(float amount)
    {
      mEnergy-=amount;
      if(mEnergy<0.0)
        {
          mEnergy=0.0;
	  die();
        }
    }
    void decMorale(float amount)
    {
      mMorale-=amount;
      if(mMorale<0.0)
        {
          mMorale=0.0;
        }
    }
    virtual void die();

    float getEnergy() const
      {
        return mEnergy;
      }

    float getCondition() const
      {
        return mCondition;
      }

    // input time in which condition is used, returns remaining time, if condition is used up
    float decCondition(float pTime)
    {
      // for how much time does it reach
      float allTime=mCondition/mConditionFall;
      if(allTime>pTime)
        {
          mCondition-=pTime*mConditionFall;
          return 0;
        }
      else
        {
          pTime-=allTime;
          mCondition=0;
          return pTime;
        }
    }

    virtual void newRestJob(int pTime);
    virtual void newFetchJob(int p,Pos2D &pTarget,const std::string &pWhat);
    virtual void newMoveJob(int p,const Pos2D &pTarget,int pnear=0);
    virtual void newFightJob(int p,AntEntity *target);


    virtual void assignJob(AntEntity*);

    virtual void gotFight(AntEntity *)
    {}

    virtual AntHero *getHero()
    {
      return 0;
    }

    virtual void defeated();

    float getMorale() const;

    bool mRubyObject;
    VALUE mRUBY;
    bool mDeleted;

    bool mMoving;

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


#endif
