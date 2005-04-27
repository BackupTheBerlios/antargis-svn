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

#include "decast.h"
#include "quadtree.h"
#include "voxel_gen.h"
#include "tree.h"

#include "ag_xml.h"
#include <ag_fs.h>

#include <set>
#include <vector>
#include <algorithm>

class AntEntity
  {
    int mID;
  protected:
    Pos3D mPos;
    Job *mJob;
    bool mJobFinished;
    float mEnergy;
    float mHealSpeed;
    bool onGround;
    float mCondition;
    float mConditionFall; // when used, how much per time -- hero is stronger here
    float mConditionHeal; // refilling

    int mDirNum;
  public:
    AntEntity();
    AntEntity(const Pos3D &p);
    AntEntity(const Pos2D &p);
    Pos3D getPos3D() const;
    Pos2D getPos2D() const;

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
    }

    void setPos2D(const Pos2D &p);

    void mapChanged();
    virtual VoxelImage*getSurface() const=0;

    /** do anything in given time frame */
    virtual void move(float pTime);

    virtual Rect2D getRect() const;

    void jobFinished();

    bool hasJob() const
      {
        return mJob;
      }

    virtual float getHealSpeed() const
      {
        return mHealSpeed;
      }

    void decEnergy(float amount)
    {
      mEnergy-=amount;
      if(mEnergy<0.0)
        {
          mEnergy=0.0;
          jobFinished();
        }
    }

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

    virtual void gotFight(AntEntity *)
    {}

    virtual AntHero *getHero()
    {
      return 0;
    }

  };


#endif
