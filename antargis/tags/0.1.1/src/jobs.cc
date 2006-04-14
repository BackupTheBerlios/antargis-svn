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

Job::Job():priority(0),inited(false)
{
}

Job::Job(int p):priority(p),inited(true)
{}

bool Job::valid() const
{
  return inited;
}

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
  e->sigJobFinished();
}

bool Job::needsMorale() const
{
  return false;
}

void Job::saveXML(Node &pNode) const
{
  pNode.set("priority",toString(priority));
}
void Job::loadXML(const Node &pNode)
{
  if(pNode.get("priority").length())
    priority=toInt(pNode.get("priority"));
  inited=true;
}

std::string Job::xmlName() const
{
  return "antJob";
}


/************************************************************************
* MoveJob
************************************************************************/

MoveJob::MoveJob()
{
}

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
    {
      if(norm>mNear)
	e->setPos(mTarget+diff.normalized()*mNear);
      e->setDirection(-diff.getAngle().angle*180.0/M_PI);
      jobFinished(e);
    }
}

void MoveJob::saveXML(Node &pNode) const
{
  Job::saveXML(pNode);
  pNode.set("pos",mTarget.toString());
  pNode.set("near",toString(mNear));
  pNode.set("run",mRun?"true":"false");
}
void MoveJob::loadXML(const Node &pNode)
{
  Job::loadXML(pNode);
  if(pNode.get("pos").length())
    mTarget=AGVector2(pNode.get("pos"));
  if(pNode.get("near").length())
    mNear=toFloat(pNode.get("near"));
  if(pNode.get("run").length())
    mRun=pNode.get("run")=="true";
}
std::string MoveJob::xmlName() const
{
  return "moveJob";
}



/************************************************************************
* FightJob
************************************************************************/

FightJob::FightJob():mTarget(0),mTargetID(-1)
{
}

FightJob::FightJob(int p,AntEntity *pTarget):Job(p),mTarget(pTarget)
{
  fightDistance=1;//0.20; // in pixels
  moving=true;
}

FightJob::~FightJob()
{
}

bool FightJob::needsMorale() const
{
  return true;
}

void FightJob::move(AntEntity *e,float ptime)
{
  if(mTarget==0)
    {
      mTarget=getMap()->getEntity(mTargetID);
      assert(mTarget);
    }

  if(e->getEnergy()<0)
    throw std::runtime_error("Problem: fighting, but I'm alread dead!");
    
  if(mTarget->getEnergy()<=0.0)
    {
      // already died - so no sigDefeated
      e->eventHaveDefeated(mTarget);
      jobFinished(e);
      return; // early out
    }
  else if(mTarget->getMorale()<0.1)
    {
      mTarget->sigDefeated();
      e->eventHaveDefeated(mTarget);
      jobFinished(e);
      return; // early out
    }
  // if target is too far away run there, otherwise fight
  AGVector2 diff=e->getPos2D()-mTarget->getPos2D();
  float norm=diff.length();
  float speed=e->getSpeed();


  e->setDirection(-diff.getAngle().angle*180.0/M_PI);

  if(norm-fightDistance>ptime*speed)
    {
      // run
      diff=diff.normalized();
      e->setPos(e->getPos2D()-diff*ptime*speed);
      //    e->setDirection(-diff.getAngle().angle*180.0/M_PI);
      if(!moving)
	{
	  e->eventStartMoving();
	  moving=true;
	}
    }
  else
    {
      // fight
      mTarget->decEnergy(ptime*e->getStrength()*e->getAggression()/mTarget->getDefense());
      mTarget->decMorale(ptime*e->getMoraleStrength()/mTarget->getDefense()); // FIXME: estimate this value
      mTarget->eventGotFight(e);

      if(moving)
	{
	  e->eventStartFighting();
	  moving=false;
	}

    }
}

void FightJob::saveXML(Node &pNode) const
{
  Job::saveXML(pNode);
  pNode.set("target",toString(mTarget->getID()));
  pNode.set("distance",toString(fightDistance));
  pNode.set("moving",moving?"true":"false");
}
void FightJob::loadXML(const Node &pNode)
{
  Job::loadXML(pNode);
  if(pNode.get("target").length())
    {
      mTarget=0;
      mTargetID=toInt(pNode.get("target"));
    }

  if(pNode.get("distance").length())
    fightDistance=toFloat(pNode.get("distance"));
  if(pNode.get("moving").length())
    moving=pNode.get("moving")=="true";
}
std::string FightJob::xmlName() const
{
  return "fightJob";
}


/***************************************************************************
 *
 ***************************************************************************/

FetchJob::FetchJob():
  mTarget(0)
{
}

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

void FetchJob::saveXML(Node &pNode) const
{
  MoveJob::saveXML(pNode);
  pNode.set("target",toString(mTarget->getID()));
  pNode.set("what",mWhat);
}
void FetchJob::loadXML(const Node &pNode)
{
  MoveJob::loadXML(pNode);
  if(pNode.get("target").length())
    {
      mTarget=0;
      mTargetID=toInt(pNode.get("target"));
    }

  if(pNode.get("what").length())
    mWhat=pNode.get("what");

}
std::string FetchJob::xmlName() const
{
  return "fetchJob";
}


/***************************************************************************
 *
 ***************************************************************************/

RestJob::RestJob():mTime(0)
{
}

RestJob::RestJob(float pTime):Job(0),mTime(pTime)
{
}
RestJob::~RestJob()
{
}
void RestJob::move(AntEntity *e,float ptime)
{
  e->incMorale(ptime);
  mTime-=ptime;
  if(mTime<0)
    jobFinished(e);
}

void RestJob::saveXML(Node &pNode) const
{
  Job::saveXML(pNode);

  pNode.set("time",toString(mTime));
}
void RestJob::loadXML(const Node &pNode)
{
  Job::loadXML(pNode);

  if(pNode.get("time").length())
    mTime=toFloat(pNode.get("time"));
}
std::string RestJob::xmlName() const
{
  return "restJob";
}

/*
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
*/
