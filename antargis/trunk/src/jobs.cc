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
#include "map.h"

Job::Job(int p):priority(p),mRubyObject(false),mDeleted(false)
{}
void Job::move(AntEntity *,float ptime)
{}

bool Job::operator<=(const Job &j) const
{
  return priority<=j.priority;
}

Job::~Job()
{
  mDeleted=true;
  //  CTRACE;
}

/************************************************************************
* MoveJob
************************************************************************/

MoveJob::MoveJob(int p,const Pos2D &pTarget,int pnear,bool pRun):Job(p),mTarget(getMap()->truncPos(pTarget)),near(pnear),mRun(pRun)
    {
      speed=70; // pixels per second
      runSpeed=100;
    }

// Jobs
void MoveJob::move(AntEntity *e,float ptime)
{
  float aspeed;
  
  if(mRun && e->getCondition()>0.0)
  {
    // decrease condition and if condition is zero - switch of running
    float newtime=e->decCondition(ptime);
    moveBy(e,ptime-newtime,runSpeed);// take same runSpeed always
    
    ptime=newtime;
  }
  aspeed=0.5*speed+0.5*e->getEnergy()*speed;
  assert(this);
  moveBy(e,ptime,aspeed); // use rest of time
  assert(this);
  
}

Pos2D MoveJob::getDirection(const AntEntity *e) const
{
  return (mTarget-e->getPos2D()).normalized();
}


void MoveJob::moveBy(AntEntity *e,float ptime,float aspeed)
{
  Pos2D diff=e->getPos2D()-mTarget;
  float norm=diff.norm();
  if(norm-near>ptime*aspeed)
    {
      diff=diff.normalized();
      e->setDirection(diff*(-1));
      e->setPos2D(e->getPos2D()-diff*ptime*aspeed);
    }
  else
    {
      //   e->setPos2D(mTarget);
      //      CTRACE;
      e->jobFinished();
    }
}

/************************************************************************
* FightJob
************************************************************************/


// FightJobs
void FightJob::move(AntEntity *e,float ptime)
{
  if(mTarget->getEnergy()==0.0)
    e->jobFinished();
  // if target is too far away run there, otherwise fight
  Pos2D diff=e->getPos2D()-mTarget->getPos2D();
  float norm=diff.norm();
  if(norm-fightDistance>ptime*speed)
    {
      diff=diff.normalized();
      e->setPos2D(e->getPos2D()-diff*ptime*speed);
    }
  else
    {
      // fight
      mTarget->decEnergy(ptime*strength);
      mTarget->gotFight(e);
    }
}


/***************************************************************************
 *
 ***************************************************************************/

FetchJob::FetchJob(std::string what):Job(0)
{
}
FetchJob::~FetchJob()
{
}
void FetchJob::move(AntEntity *e,float ptime)
{
}


/***************************************************************************
 *
 ***************************************************************************/

RestJob::RestJob(float pTime):Job(0),mTime(pTime)
{
}
void RestJob::move(AntEntity *e,float ptime)
{
  mTime-=ptime;
  if(mTime<0)
    e->jobFinished();
  
}
