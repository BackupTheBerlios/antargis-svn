/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_man.cc
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

#include "ant_man.h"
#include "ant_boss.h"
#include "ant_hero.h"
#include "map.h"
#include "jobs.h"

AntMan::AntMan():typeID(0),mBoss(0),mBossID(0)
    {}
AntMan::AntMan(const Pos2D &p,int pTypeID,AntBoss *pBoss):AntEntity(p),typeID(pTypeID),mBoss(pBoss),mBossID(0)
    {
      if(pBoss)
        pBoss->insert(this);
    }
AntMan::~AntMan()
    {
      if(mBoss)
        mBoss->remove(this);
    }
    
std::string AntMan::getTexture() const
    {
      std::ostringstream os;
      os<<"man"<<mDirNum;
      return os.str();
    }

    
void AntMan::move(float pTime)
{
  if(mBossID && !mBoss)
    {
      mBoss=dynamic_cast<AntBoss*>(getMap()->getEntity(mBossID));
      mBossID=0;
      mBoss->insert(this);
    }
  if(mBoss && !mJob)
    {
      AntHero *h=dynamic_cast<AntHero*>(mBoss);
      
      if(h)
	setJob(new MoveJob(0,h->getPos2D()+h->getFormation(this),0,true));
      else
	{
	  CTRACE;
	  Job *j=mBoss->getJob(this);
	  if(j)
	    setJob(j);
	}
    }
  else if(!mBoss)
    {
        CTRACE;

      // search house here
    }
  AntEntity::move(pTime);
}
void AntMan::updateSurface()
{
  std::ostringstream os;
  os<<"man1";
  /*        if(typeID==0)
	    os<<"man1dl";
	    else
	    os<<"man2dl";*/
  VoxelImage *im=new VoxelImage(os.str());//imageCache()->getImage(os.str());
  im->setPosition(mPos);
  //im->setCenter(Pos2D(100,150)+Pos2D(0,64));
  im->setVirtualY(40);
  setSurface(im);
}

void AntMan::discard(AntBoss *boss)
{
  assert(mBoss==boss);
  mBoss=0;
}
AntBoss *AntMan::getBoss()
{
  return mBoss;
}
float AntMan::getHealSpeed() const
{
  if(hasJob())
    return mHealSpeed*0.2;
  else
    return mHealSpeed;
}
std::string AntMan::xmlName() const
{
  return "antMan";
}
/************************************************************************
* AntMan
************************************************************************/
void AntMan::saveXML(xmlpp::Node &node) const
{
  AntEntity::saveXML(node);
  node.set("typeID",toString(typeID));
  if(mBoss)
    node.set("bossID",toString(mBoss->getID()));
}
void AntMan::loadXML(const xmlpp::Node &node)
{
  AntEntity::loadXML(node);
  typeID=toInt(node.get("typeID"));
  if(node.get("bossID").length())
    mBossID=toInt(node.get("bossID"));
}
