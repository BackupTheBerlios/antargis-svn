/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_hero.cc
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

#include "ant_hero.h"
#include "ant_man.h"
#include "map.h"
#include "jobs.h"


AntHero::AntHero():typeID(0)
{}

/************************************************************************
* AntHero
************************************************************************/
AntHero::AntHero(const Pos2D &p,int pTypeID,const std::string &pName):AntBoss(p),typeID(pTypeID),mName(pName)
{}
AntHero::~AntHero()
{
  std::set
    <AntMan*>::iterator i=mMen.begin();
  for(;i!=mMen.end();i++)
    (*i)->discard(this);

  getMap()->killHero(this);
}
void AntHero::updateSurface()
  {
    std::ostringstream os;
    if(typeID==0)
      os<<"hero1dl";
    else
      os<<"hero2dl";
    VoxelImage *im=new VoxelImage(os.str());//imageCache()->getImage(os.str());
    im->setPosition(mPos);
    //im->setCenter(Pos2D(100,150)+Pos2D(0,64));
    im->setVirtualY(40);
    im->setName(mName);
    setSurface(im);
  }

int AntHero::getTypeID() const
  {
    return typeID;
  }


void AntHero::fightHero(AntHero *h)
{
  CTRACE;
  // myselfs attacks other hero
  setJob(new FightJob(0,h));

  // get other's men and other hero and put them all into a vector
  std::set
    <AntMan*> otherMen=h->mMen;
  std::vector<AntEntity*> otherList;

  std::set
    <AntMan*>::iterator i=otherMen.begin();
  for(;i!=otherMen.end();i++)
    otherList.push_back(*i);
  otherList.push_back(h); // push other hero

  std::random_shuffle(otherList.begin(),otherList.end()); // shuffle all

  std::vector<AntEntity*>::iterator k=otherList.begin();
  // assign from front to back all the entities for fighting
  for(i=mMen.begin();i!=mMen.end();i++)
    {
      (*i)->setJob(new FightJob(0,*k));

      k++;
      if(k==otherList.end())
        k=otherList.begin(); // simply restart, if end is hit
    }


}

void AntHero::goTo(int prio,const Pos2D &pos)
{
  std::set
    <AntMan*>::iterator i;
  for(i=mMen.begin();i!=mMen.end();i++)
    (*i)->jobFinished();
  setJob(new MoveJob(prio,pos));
}

/*
void AntHero::signUp(AntMan *man)
{
  mMen.insert(man);
}


void AntHero::discard(AntMan *man)
{
  mMen.erase(man);
  }*/

float AntHero::calcTroupStrength() const
  {
    float s=getEnergy();
    std::set
      <AntMan*>::iterator i;
    for(i=mMen.begin();i!=mMen.end();i++)
      s+=(*i)->getEnergy();
    return s;
  }

AntHero *AntHero::fights()
{
  if(mJob)
    {
      FightJob *f=dynamic_cast<FightJob*>(mJob);
      if(f)
        {
          return dynamic_cast<AntHero*>(f->getTarget());
        }
    }
  return 0;
}

void AntHero::saveXML(xmlpp::Node &node) const
{
  AntEntity::saveXML(node);
  node.set("typeID",toString(typeID));
  node.set("name",mName);
}
void AntHero::loadXML(const xmlpp::Node &node)
{
  AntEntity::loadXML(node);
  typeID=toInt(node.get("typeID"));
  mName=node.get("name");
}


void AntHero::gotFight(AntEntity *e)
{
  if(!fights())
    {
      AntHero *h=e->getHero();
      
      if(h)
            fightHero(h);
    }
}

Pos2D AntHero::getFormation(AntMan *m) const
{
  size_t count=mMen.size();
  
  // if(count>20) FIXME: do second circle
  
  std::set<AntMan*>::const_iterator j=mMen.begin();
  size_t c=0;
  for(;j!=mMen.end() && *j!=m;j++,c++)
    ;
  if(j==mMen.end())
    return Pos2D(0,0);
  else
    {
      
      if(hasJob())
	{
	  MoveJob *j=dynamic_cast<MoveJob*>(mJob);
	  if(j)
	    {
	      Pos2D dir=j->getDirection(this);
	      Pos2D normal=dir.normal();
	      Pos2D t=dir*(-1)*(c/6)*32+normal*((c%6)-2.5)*16;
	      return t;
	    }
	}
      float angle=float(c)/float(count)*M_PI*2.0;
      return Pos2D(sin(angle)*64,cos(angle)*64);
    }
  
}

AntHero *AntHero::getHero()
{
  return this;
}
float AntHero::getHealSpeed() const
{
  if(hasJob())
    return mHealSpeed*0.2;
  else
    return mHealSpeed;
}
std::string AntHero::xmlName() const
{
  return "antHero";
}
