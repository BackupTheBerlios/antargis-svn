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

AntEntity::AntEntity(const Pos3D &p):mPos(p)
{
  init();
}
AntEntity::AntEntity(const Pos2D &p):mPos(getMap()->getPos3D(p))
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

  mMoveSpeed=70;

  mAggression=0.5;

  mSurface=0;

  mDirNum=1;
}


AntEntity::~AntEntity()
{
  //  CTRACE;
  //  cdebug(mRubyObject);
  //  throw int();
}

void AntEntity::saveXML(xmlpp::Node &node) const
  {
    myxmlpp::Node &child=node.newChild("position");
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
  xmlpp::Node::const_iterator i=node.begin();
  for(;i!=node.end();i++)
    mPos.loadXML(*i);
  //  mID=toInt(node.get("entityID"));
  if(node.get("morale")!="")
    mMorale=toFloat(node.get("morale"));
  else
    mMorale=1.0f;
  mAggression=toFloat(node.get("aggression"));
  setName(node.get("name"));
}

Pos3D AntEntity::getPos3D() const
  {
    return mPos;
  }
Pos2D AntEntity::getPos2D() const
  {
    return Pos2D(mPos.x,mPos.z);
  }

void AntEntity::setPos3D(const Pos3D &p)
{
  mPos=p;
}
void AntEntity::setPos2D(const Pos2D &p)
{
  mPos=Pos3D(p.x,getMap()->getHeight(p),p.y);
  onGround=true;
}

void AntEntity::setJob(Job *pJob)
{
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
      //      if(!pJob->mRubyObject) // let ruby delete it
	{
	  //	  cdebug("DELETE JOB:"<<pJob);
	  mJobFinished.push_back(pJob);
	  //	  delete pJob;
	}
    }
  if(mJob)
    {
      //      if(mJob->mRubyObject)
      //	rb_gc_register_address(&(mJob->mRUBY));
      //	rb_gc_mark(mJob->mRUBY);
      eventGotNewJob();
    }

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
      //      if(!mJob->mRubyObject)
      //      delete mJob;
      //      mJob=0;
      //      mJobFinished=false;
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

     
  //  mCondition+=mConditionHeal*pTime;
}



Rect2D AntEntity::getRect() const
  {
    return Rect2D((int)mPos.x,(int)mPos.y,32,32);
  }

void AntEntity::eventJobFinished()
{
  mJobFinished.push_back(mJob);
  mJob=0;
  //  if(mJob)
  //    mJobFinished=true;
}

void AntEntity::eventNoJob()
{
}

/*
bool AntEntity::isJobFinished() const
{
  return mJobFinished.size();
  }*/

void AntEntity::eventMapChanged()
{
  if(onGround)
    mPos=getMap()->getPos3D(Pos2D(mPos.x,mPos.z));
}

void AntEntity::setType(const std::string &pType)
{
  mType=pType;
}

std::string AntEntity::getType() const
{
  return mType;
}

void AntEntity::setSurface(VoxelImage *i)
{
  mSurface=i;
}

/*
void AntEntity::assignJob(AntEntity*)
{
}*/

void AntEntity::updateSurface()
{
  std::string s=getTexture();
  if(s.length())
    {
      VoxelImage *im=new VoxelImage(s);
      im->setVirtualY(getVirtualY());
      setSurface(im);
    }
}


int AntEntity::getVirtualY() const
{
  return mVirtualY;
}

void AntEntity::setVirtualY(int y)
{
  mVirtualY=y;
}

VoxelImage*AntEntity::getSurface()
    {
      //      CTRACE;
      //      cdebug(mSurface);
      if(!mSurface)
	updateSurface();
      //      cdebug(mSurface);
      //      cdebug(typeid(*this).name());
      return mSurface;
    }

/*
void AntEntity::setVar(std::string n,std::string v)
{
  mVars[n]=v;
}
std::string AntEntity::getVar(std::string n) const
{
  std::map<std::string,std::string>::const_iterator i=mVars.find(n);
  if(i==mVars.end())
    return "";
  return i->second;
  }

int AntEntity::getPlayerID() const
{
  return mPlayerID;
}
void AntEntity::setPlayerID(int id)
{
  mPlayerID=id;
}*/

int AntEntity::getID() const
{
  return mID;
}

std::string AntEntity::xmlName() const
{
  return "antEntity";
}


std::string AntEntity::getTexture() const
    {
      return "";
    }

void AntEntity::setDirection(const Pos2D &p)
    {
      Pos2D p2=p.normalized();
      int oldDir=mDirNum;

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
      if(mDirNum!=oldDir)
	updateSurface();
    }



// RESOURCE

Resource::Resource()
{
}
int Resource::get(const std::string &pName)
{
  return r[pName];
}
void Resource::add(const std::string &pName,int value)
{
  r[pName]+=value;
}
void Resource::set(const std::string &pName,int value)
{
  r[pName]=value;
}

void Resource::takeAll(Resource &pr)
{
  std::map<std::string,int>::iterator i=pr.r.begin();
  for(;i!=pr.r.end();i++)
    add(i->first,i->second);
  pr.r.clear();
}

std::map<std::string,int> Resource::getAll() const
{
  return r;
}



void AntEntity::newRestJob(int pTime)
{
  setJob(new RestJob(pTime));
}
void AntEntity::newFetchJob(int p,Pos2D &pTarget,const std::string &what)
{
  setJob(new FetchJob(p,pTarget,what));
}
void AntEntity::newMoveJob(int p,const Pos2D &pTarget,int pnear)
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
