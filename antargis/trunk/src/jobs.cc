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

MoveJob::MoveJob(int p,const AGVector2 &pTarget,float pNear,bool pRun):Job(p),mTarget(getMap()->truncPos(pTarget)),mNear(pNear),mRun(pRun)
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

  if(norm-mNear>ptime*aspeed)
    {
      diff=diff.normalized();
      e->setDirection(-diff.getAngle().angle*180.0/M_PI);
      e->setPos(e->getPos2D()-diff*ptime*aspeed);
    }
  else
    jobFinished(e);
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
  fightDistance=1;//0.20; // in pixels
  strength=0.2;   // decrease per second
}

void FightJob::move(AntEntity *e,float ptime)
{
  if(mTarget->getEnergy()<=0.0 || mTarget->getMorale()<0.1 || e->getEnergy()<=0)
    {
      mTarget->eventDefeated();
      jobFinished(e);
      return; // early out
    }
  // if target is too far away run there, otherwise fight
  AGVector2 diff=e->getPos2D()-mTarget->getPos2D();
  float norm=diff.length();
  float speed=e->getSpeed();
  if(norm-fightDistance>ptime*speed)
    {
      diff=diff.normalized();
      e->setPos(e->getPos2D()-diff*ptime*speed);
      e->setDirection(-diff.getAngle().angle*180.0/M_PI);
    }
  else
    {
      // fight
      mTarget->decEnergy(ptime*e->getStrength()*e->getAggression());
      mTarget->decMorale(ptime*e->getMoraleStrength()); // FIXME: estimate this value
      mTarget->eventGotFight(e);
    }
}


/***************************************************************************
 *
 ***************************************************************************/

FetchJob::FetchJob(int p,const AGVector2&pTarget,std::string what):MoveJob(p,pTarget,0.5),mWhat(what),mTarget(0)
{
}
FetchJob::FetchJob(int p,AntEntity *pTarget,std::string what):MoveJob(p,pTarget->getPos2D(),0.5),mWhat(what),mTarget(pTarget)
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
  e->resourceChanged();
  MoveJob::jobFinished(e);

  if(mTarget)
    {
      mTarget->resource.sub(mWhat,1);
      mTarget->resourceChanged();
    }
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
MoveJob *newMoveJob(int p,const AGVector2 &pTarget,int pNear)
{
  return new MoveJob(p,pTarget,pNear);
}
