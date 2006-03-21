/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * entity.cc
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

#include "entity.h"
#include "map.h"
#include "ag_debug.h"
#include "jobs.h"
#include "mesh.h"
#include "anim_mesh.h"

AntEntity::AntEntity(const AGVector3 &p):mPos(p)
{
  init();
}
AntEntity::AntEntity(const AGVector2 &p):mPos(getMap()->getPos(p))
{
  init();
}

AntEntity::AntEntity():mPos(0,0,0)
{
  init();
}

void AntEntity::init()
{
  CTRACE;
  mRubyObject=false;

  mID=getMap()->getNewID();
  cdebug("mID:"<<mID);
  
  mShowOnMinimap=false;
  onGround=true;

  mJob=0;

  mVirtualY=40;

  mEnergy=1.0;
  mHealSpeed=0.1;

  mMorale=1.0;
  mMoraleHeal=0.05;

  mMoveSpeed=2;

  mAggression=0.5;

  //  mSurface=0;
  //esh=0;
  mDir=0;

  mStrength=0.02;
  mMoraleStrength=0.04;

  mFood=1.0;
  mHunger=0.006;
  mHungerHitEnergy=0.1;
  mHungerHitMorale=0.14;

  //  mDirNum=1;
}


AntEntity::~AntEntity()
{
  for(Meshes::iterator i=mMeshes.begin();i!=mMeshes.end();i++)
    saveDelete(*i);
  mMeshes.clear();
  delete mJob;
}

void AntEntity::resourceChanged()
{
}

void AntEntity::saveXML(xmlpp::Node &node) const
  {
    Node &child=node.addChild("position");
    mPos.saveXML(child);
    node.set("energy",toString(mEnergy));
    node.set("healSpeed",toString(mHealSpeed));
    node.set("onGround",toString(onGround));
    //    node.set("entityID",toString(getID()));
    node.set("name",getName());
    node.set("morale",toString(mMorale));
    node.set("aggression",toString(mAggression));
  }
void AntEntity::loadXML(const xmlpp::Node &node)
{
  mEnergy=toFloat(node.get("energy"));
  mHealSpeed=toFloat(node.get("healSpeed"));
  onGround=toBool(node.get("onGround"));
  assert(onGround);
  xmlpp::Node::NodeVector v=node.getChildren("position");
  xmlpp::Node::const_iterator i=v.begin();
  for(;i!=v.end();i++)
    mPos.loadXML(**i);
  //  mID=toInt(node.get("entityID"));
  if(node.get("morale")!="")
    mMorale=toFloat(node.get("morale"));
  else
    mMorale=1.0f;
  mAggression=toFloat(node.get("aggression"));
  setName(node.get("name"));
}

AGVector3 AntEntity::getPos3D() const
  {
    return mPos;
  }
AGVector2 AntEntity::getPos2D() const
  {
    return AGVector2(mPos[0],mPos[1]);
  }

void AntEntity::updatePos(const AGVector3 &p)
{
  if(mMeshes.size()==1)
    {
      mMeshes.front()->setPos(p);
      return;
    }
  for(Meshes::iterator i=mMeshes.begin();i!=mMeshes.end();i++)
    {
      (*i)->setPos(p+mMeshPos[*i]);
      (*i)->setRotation(mDir);
    }
  
}

void AntEntity::setPos(const AGVector3 &p)
{
  mPos=p;
  updatePos(p);
}
void AntEntity::setPos(const AGVector2 &p)
{
  mPos=getMap()->getPos(p);
  onGround=true;
  updatePos(mPos);
}

void AntEntity::delJob()
{
  setJob(0);
}

void AntEntity::setJob(Job *pJob)
{
  //  assert(pJob);
  if(mJob)
    {
      if(pJob)
	{
	  if((*mJob)<=(*pJob))
	    {
	      mJobFinished.push_back(mJob);
	      //delete mJob;
	    }
	  else
	    {
	      throw int(); // FIXME: delete this, it's a test if priorities get handled correctly
	      mJobFinished.push_back(pJob);
	      //	      delete pJob;
	  return;
	    }
	}
      else
	mJobFinished.push_back(mJob);
    }
  mJob=0;
  if(mEnergy>=0.0) // do job anyways
    {
      if(!pJob)
	mJob=pJob;
      else
	{
	  if(mMorale>0.1 || !pJob->needsMorale()) // at least 10% morale
	    mJob=pJob;
	  else
	    mJobFinished.push_back(pJob);
	}
    }
  else
    {
      if(pJob)
	mJobFinished.push_back(pJob);
    }
  if(mJob)
    eventGotNewJob();
}

void AntEntity::eventGotNewJob()
{
}


/** do anything in given time frame */
void AntEntity::move(float pTime)
{
  if(mJobFinished.size() || mEnergy==0.0)
    {
      std::list<Job*>::iterator i=mJobFinished.begin();
      for(;i!=mJobFinished.end();i++)
	delete *i;
      mJobFinished.clear();
    }
  else if(mEnergy>0.0)
    {
      starve(pTime);

      if(mMorale<=0.1)
	if(mJob)
	  if(mJob->needsMorale())
	    setJob(0);// kill job
      if(!isStarving())
	{
	  mEnergy+=pTime*getHealSpeed();
	  if(mEnergy>1.0)
	    mEnergy=1.0;
	}
    }
  if(mJob)
    {
      mJob->move(this,pTime);
    }
  else
    eventNoJob();

}

void AntEntity::incMorale(float pTime)
{
  if(!isStarving())
    {
      mMorale+=pTime*mMoraleHeal;
      if(mMorale>1.0)
	mMorale=1.0;
    }
}




void AntEntity::eventJobFinished()
{
}

void AntEntity::eventNoJob()
{
}


void AntEntity::eventMapChanged()
{
  if(onGround)
    mPos=getMap()->getPos(AGVector2(getPos2D()));
  updatePos(mPos);
}

void AntEntity::setMesh(SceneNode *m)
{
  // clear meshes from scene
  for(Meshes::iterator i=mMeshes.begin();i!=mMeshes.end();i++)
      saveDelete(*i);

  mMeshes.clear();
  mMeshPos.clear();
  if(m)
    {
      AnimMesh *mesh=dynamic_cast<AnimMesh*>(m);
      if(mesh)
	mesh->setEntity(this);
      mMeshes.push_back(m);

      updatePos(mPos);
    }
}

void AntEntity::addMesh(SceneNode *m,const AGVector3 &v)
{
  if(m)
    {
      AnimMesh *mesh=dynamic_cast<AnimMesh*>(m);
      if(mesh)
	mesh->setEntity(this);
      mMeshes.push_back(m);
      mMeshPos.insert(std::make_pair(m,v));

      updatePos(mPos);
    }
}


AntEntity::Meshes AntEntity::getMesh()
{
  return mMeshes;
}

SceneNode *AntEntity::getFirstMesh()
{
  return mMeshes.front();
}



int AntEntity::getID() const
{
  return mID;
}

std::string AntEntity::xmlName() const
{
  return "antEntity";
}
float AntEntity::getDirection() const
{
  return mDir;
}


void AntEntity::setDirection(float dir)
{
  mDir=dir;

  if(mMeshes.size())
    {
      SceneNode *m=mMeshes.front();
      if(m)
	m->setRotation(dir);
    }
}

void AntEntity::setVisible(bool v)
{
  for(Meshes::iterator i=mMeshes.begin();i!=mMeshes.end();i++)
    (*i)->setVisible(v);
}


// RESOURCE

Resource::Resource()
{
}
float Resource::get(const std::string &pName)
{
  return r[pName];
}
void Resource::add(const std::string &pName,float value)
{
  r[pName]+=value;
}
void Resource::sub(const std::string &pName,float value)
{
  r[pName]=std::max(r[pName]-value,0.0f);
}
void Resource::set(const std::string &pName,float value)
{
  r[pName]=value;
}

void Resource::takeAll(Resource &pr)
{
  std::map<std::string,float>::iterator i=pr.r.begin();
  for(;i!=pr.r.end();i++)
    add(i->first,i->second);
  pr.r.clear();
}

std::map<std::string,float> Resource::getAll() const
{
  return r;
}



void AntEntity::newRestJob(float pTime)
{
  setJob(new RestJob(pTime));
}
void AntEntity::newFetchJob(int p,AGVector2 &pTarget,const std::string &what)
{
  setJob(new FetchJob(p,pTarget,what));
}
void AntEntity::newFetchJob(int p,AntEntity *pTarget,const std::string &what)
{
  setJob(new FetchJob(p,pTarget,what));
}
void AntEntity::newMoveJob(int p,const AGVector2 &pTarget,int pnear)
{
  setJob(new MoveJob(p,pTarget,pnear));
}

void AntEntity::newFightJob(int p,AntEntity *target)
{
  if(!canFight())
    return;
  if(mJob)
    {
      FightJob *f=dynamic_cast<FightJob*>(mJob);
      if(f)
	if(f->getTarget()==target)
	  return;
    }
  setJob(new FightJob(p,target));
}

void AntEntity::setSpeed(float f)
{
  mMoveSpeed=f;
}
float AntEntity::getSpeed() const
{
  return mMoveSpeed;
}

void AntEntity::setHealSpeed(float f)
{
  mHealSpeed=f;
}

void AntEntity::decEnergy(float amount)
{
  mEnergy-=amount;
  if(mEnergy<0.0)
    {
      mEnergy=0.0;
      eventDie();
    }
}

bool AntEntity::hasJob() const
      {
        return mJob;
      }

float AntEntity::getHealSpeed() const
      {
        return mHealSpeed;
      }


void AntEntity::decMorale(float amount)
{
  mMorale-=amount;
  if(mMorale<0.0)
    {
      mMorale=0.0;
      eventMoraleLow();
    }
}


void AntEntity::eventDie()
{
  sigJobFinished();
}

float AntEntity::getMorale() const
{
  return mMorale;
}
float AntEntity::getEnergy() const
{
  return mEnergy;
}

void AntEntity::setAggression(float agg)
{
  mAggression=agg;
}
float AntEntity::getAggression() const
{
  return mAggression;
}

void AntEntity::eventGotFight(AntEntity*pOther)
{
}

void AntEntity::eventDefeated()
{
}

void AntEntity::sigDefeated()
{
  mMorale=-0.1; // really deep morale
  // this prevents other fight-jobs from not being discarded
  eventDefeated();
}



void AntEntity::setName(const std::string &pName)
{
  mName=pName;
}
std::string AntEntity::getName() const
{
  return mName;
}

AGRect2 AntEntity::getRect() const
{
  // FIXME: exchange this with something suitable
  return AGRect2(mPos[0]-0.1, mPos[1]-0.1, 0.2, 0.2);
}



void AntEntity::mark()
{
  for(Meshes::iterator i=mMeshes.begin();i!=mMeshes.end();i++)
    markObject(*i);
}


void AntEntity::clear()
{
  clearMeshes();
}


void AntEntity::clearMeshes()
{
  for(Meshes::iterator i=mMeshes.begin();i!=mMeshes.end();i++)
    {
      saveDelete(*i);
    }
  mMeshes.clear();
}

void AntEntity::animationEvent(const std::string &pName)
{
}

void AntEntity::setMinimapColor(const AGColor &c)
{
  mMinimapColor=c;
  mShowOnMinimap=true;
}

AGColor AntEntity::getMinimapColor() const
{
  return mMinimapColor;
}

bool AntEntity::showOnMinimap() const
{
  return mShowOnMinimap;
}

void AntEntity::setStrength(float f)
{
  mStrength=f;
}
float AntEntity::getStrength() const
{
  return mStrength;
}
void AntEntity::setMoraleStrength(float f)
{
  mMoraleStrength=f;
}
float AntEntity::getMoraleStrength() const
{
  return mMoraleStrength;
}

void AntEntity::setProvide(const std::string &pName,bool flag)
{
  if(flag)
    mProvides.insert(pName);
  else
    mProvides.erase(pName);
}

bool AntEntity::provides(const std::string &pName) const
{
  return(mProvides.find(pName)!=mProvides.end());
}

bool AntEntity::canFight() const
{
  return (getEnergy()>0.0 && getMorale()>=0.1);
}

bool AntEntity::isFighting() const
{
  if(mJob)
    {
      FightJob *f=dynamic_cast<FightJob*>(mJob);
      return f;
    }
  return false;
}

void AntEntity::sigJobFinished()
{
  mJobFinished.push_back(mJob);
  mJob=0;
  eventJobFinished();
}

void AntEntity::eventHaveDefeated(AntEntity *e)
{
}



float AntEntity::getFood() const
{
  return mFood;
}

void AntEntity::incFood(float v)
{
  mFood+=v;
}

void AntEntity::starve(float pTime)
{
  mFood-=pTime*mHunger;
  if(isStarving())
    {
      // get time for which mFood==0
      float t=-mFood/mHunger;

      // hit energy
      decEnergy(mHungerHitEnergy*t);

      // hit morale
      decMorale(mHungerHitMorale*t);


      mFood=0;
    }
}

void AntEntity::eventMoraleLow()
{
}

bool AntEntity::isStarving() const
{
  return mFood<=0;
}
