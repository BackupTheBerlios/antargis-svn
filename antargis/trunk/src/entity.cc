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
  
  onGround=true;

  mJob=0;

  mVirtualY=40;

  mEnergy=1.0;
  mHealSpeed=0.1;

  mMorale=1.0;
  mMoraleHeal=0.1;

  mMoveSpeed=2;

  mAggression=0.5;

  //  mSurface=0;
  //esh=0;
  mDir=0;

  //  mDirNum=1;
}


AntEntity::~AntEntity()
{
  //  CTRACE;
  //  cdebug(mRubyObject);
  //  throw int();

  if(getMap())
    if(getMap()->getScene())
      {
	for(std::list<SceneNode*>::iterator i=mMesh.begin();i!=mMesh.end();i++)
	  getMap()->getScene()->removeNode(*i);
      }
}

void AntEntity::resourceChanged()
{
}

void AntEntity::saveXML(xmlpp::Node &node) const
  {
    Node &child=node.newChild("position");
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
  xmlpp::Node::NodeVector v=node.get_children("position");
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
  if(mMesh.size()==1)
    {
      mMesh.front()->setPos(p);
      return;
    }
  for(Meshes::iterator i=mMesh.begin();i!=mMesh.end();i++)
    (*i)->setPos(p+mMeshPos[*i]);
  
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
      if(mMorale<=0.1)
	if(mJob)
	  if(mJob->needsMorale())
	    setJob(0);// kill job

      mEnergy+=pTime*getHealSpeed();
      mMorale+=pTime*mMoraleHeal;
      if(mMorale>1.0)
	mMorale=1.0;
      if(mEnergy>1.0)
        mEnergy=1.0;
    }
  if(mJob)
    {
      mJob->move(this,pTime);
    }
  else
    eventNoJob();

}



void AntEntity::eventJobFinished()
{
  mJobFinished.push_back(mJob);
  mJob=0;
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

void AntEntity::setType(const std::string &pType)
{
  mType=pType;
}

std::string AntEntity::getType() const
{
  return mType;
}

void AntEntity::setMesh(SceneNode *m)
{
  // clear meshes from scene
  for(Meshes::iterator i=mMesh.begin();i!=mMesh.end();i++)
    getMap()->getScene()->removeNode(*i);

  mMesh.clear();
  mMeshPos.clear();
  if(m)
    {
      mMesh.push_back(m);
      getMap()->getScene()->addNode(m);
      updatePos(mPos);
    }
}

void AntEntity::addMesh(SceneNode *m,const AGVector3 &v)
{
  if(m)
    {
      mMesh.push_back(m);
      mMeshPos.insert(std::make_pair(m,v));
      getMap()->getScene()->addNode(m);
      updatePos(mPos);
    }
}


AntEntity::Meshes AntEntity::getMesh()
{
  return mMesh;
}

SceneNode *AntEntity::getFirstMesh()
{
  return mMesh.front();
}



int AntEntity::getID() const
{
  return mID;
}

std::string AntEntity::xmlName() const
{
  return "antEntity";
}


void AntEntity::setDirection(float dir)
{
  mDir=dir;

  if(mMesh.size())
    {
      SceneNode *m=mMesh.front();
      if(m)
	m->setRotation(dir);
    }
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



void AntEntity::newRestJob(int pTime)
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
  setJob(new FightJob(p,target));
}



// AntEntityPtr
AntEntityPtr::AntEntityPtr():p(0)
{
}

AntEntityPtr::AntEntityPtr(const AntEntityPtr &pp):p(pp.p)
{
}
AntEntityPtr::AntEntityPtr(AntEntity *pp):p(pp)
{
}

AntEntity *AntEntityPtr::get()
{
  assert(p);
  return p;
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
        }
    }


void AntEntity::eventDie()
{
  eventJobFinished();
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



void AntEntity_markfunc(void *ptr)
{
  if(!ptr)
    return;
  //  Mesh *cppAnimal;
  AntEntity *zoo;

  zoo = static_cast<AntEntity*>(ptr);

  for(AntEntity::Meshes::iterator i=zoo->mMesh.begin();i!=zoo->mMesh.end();i++)
    if(*i)
      if((*i)->mRubyObject)
	rb_gc_mark((*i)->mRUBY);
  /*
  cppAnimal=zoo->getMesh();
  if(cppAnimal)
    if(cppAnimal->mRubyObject)
    rb_gc_mark(cppAnimal->mRUBY);*/
}
