/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * jobs.cc
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

#include "jobs.h"
#include "entity.h"
#include "map.h"
#include <math.h>

Job::Job(int p):priority(p)

{}
void Job::move(AntEntity *,float ptime)
{}

bool Job::operator<=(const Job &j) const
{
  return priority<=j.priority;
}

Job::~Job()
{
}

void Job::jobFinished(AntEntity *e)
{
  e->eventJobFinished();
}

bool Job::needsMorale() const
{
  return false;
}

/************************************************************************
* MoveJob
************************************************************************/

MoveJob::MoveJob(int p,const AGVector2 &pTarget,float pnear,bool pRun):Job(p),mTarget(getMap()->truncPos(pTarget)),near(pnear),mRun(pRun)
{
  // speed=70; // pixels per second
  //  runSpeed=100;
}
MoveJob::~MoveJob()
{
}

// Jobs
void MoveJob::move(AntEntity *e,float ptime)
{
  float aspeed;
  float speed=e->getSpeed();
 
#ifdef ENABLE_RUNNING  
  float runSpeed=speed*1.3;
 
  if(mRun && e->getCondition()>0.0)
  {
    // decrease condition and if condition is zero - switch of running
    float newtime=e->decCondition(ptime);
    moveBy(e,ptime-newtime,runSpeed);// take same runSpeed always
    
    ptime=newtime;
  }
#endif
  aspeed=0.5*speed+0.5*e->getEnergy()*speed;
  assert(this);
  moveBy(e,ptime,aspeed); // use rest of time
  assert(this);
  
}

AGVector2 MoveJob::getDirection(const AntEntity *e) const
{
  return (mTarget-e->getPos2D()).normalized();
}


void MoveJob::moveBy(AntEntity *e,float ptime,float aspeed)
{
  AGVector2 diff=e->getPos2D()-mTarget;
  float norm=diff.length();
  //  cdebug("norm:"<<norm);
  //  cdebug("near:"<<near);
  if(norm-near>ptime*aspeed)
    {
      //      cdebug(diff);
      diff=diff.normalized();
      //      cdebug(diff);
      e->setDirection(-diff.getAngle().angle*180.0/M_PI);
      //      cdebug(e->getAGVector2());
      e->setPos(e->getPos2D()-diff*ptime*aspeed);
      //      cdebug(e->getAGVector2());
    }
  else
    {
      //      e->setAGVector2(mTarget);
      //      CTRACE;
      jobFinished(e);
    }
}

/************************************************************************
* FightJob
************************************************************************/

FightJob::~FightJob()
{
}

bool FightJob::needsMorale() const
{
  return true;
}


// FightJobs

FightJob::FightJob(int p,AntEntity *pTarget):Job(p),mTarget(pTarget)
{
  fightDistance=0.20; // in pixels
  strength=0.2;   // decrease per second
  //  speed=70; // see MoveJob
}

void FightJob::move(AntEntity *e,float ptime)
{
  if(mTarget->getEnergy()==0.0 || mTarget->getMorale()<0.1)
    {
      //      cdebug("target-e:"<<mTarget->getEnergy());
      //      cdebug("target-m:"<<mTarget->getMorale());
      mTarget->eventDefeated();
      jobFinished(e);
    }
  // if target is too far away run there, otherwise fight
  AGVector2 diff=e->getPos2D()-mTarget->getPos2D();
  float norm=diff.length();
  float speed=e->getSpeed();
  if(norm-fightDistance>ptime*speed)
    {
      diff=diff.normalized();
      e->setPos(e->getPos2D()-diff*ptime*speed);
    }
  else
    {
      // fight
      mTarget->decEnergy(ptime*strength*e->getAggression());
      mTarget->decMorale(ptime*strength*0.7); // FIXME: estimate this value
      mTarget->eventGotFight(e);
    }
}


/***************************************************************************
 *
 ***************************************************************************/

FetchJob::FetchJob(int p,const AGVector2&pTarget,std::string what):MoveJob(p,pTarget,0.2),mWhat(what)
{
}
FetchJob::~FetchJob()
{
}
void FetchJob::move(AntEntity *e,float ptime)
{
  MoveJob::move(e,ptime);
}

void FetchJob::jobFinished(AntEntity *e)
{
  e->resource.add(mWhat,1);
  MoveJob::jobFinished(e);
}


/***************************************************************************
 *
 ***************************************************************************/

RestJob::RestJob(float pTime):Job(0),mTime(pTime)
{
}
RestJob::~RestJob()
{
}
void RestJob::move(AntEntity *e,float ptime)
{
  mTime-=ptime;
  if(mTime<0)
    jobFinished(e);
}


RestJob *newRestJob(int pTime)
{
  return new RestJob(pTime);
}
FetchJob *newFetchJob(int p,AGVector2 &pTarget,const std::string &what)
{
  return new FetchJob(p,pTarget,what);
}
MoveJob *newMoveJob(int p,const AGVector2 &pTarget,int pnear)
{
  return new MoveJob(p,pTarget,pnear);
}
