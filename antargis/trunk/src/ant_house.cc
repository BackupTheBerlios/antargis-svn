/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_house.cc
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

#include "ant_house.h"
#include "ant_man.h"
#include "map.h"

AntHouse::AntHouse(std::string pName):mName(pName)
{
  mWood=mSteel=mStone=mWater=mFood=10; // some starting point

}
AntHouse::AntHouse(const Pos2D &p,std::string pName):AntBoss(p),mName(pName)
{
  mWood=mSteel=mStone=mWater=mFood=10; // some starting point
}
AntHouse::~AntHouse()
{
}
void AntHouse::updateSurface()
{
  std::ostringstream os;
  VoxelImage *im=0;
  
  os<<mName;
  
  if(!fileExists(TILEDIR+os.str()+".png"))
    throw std::string("File not found")+os.str();
  im=new VoxelImage(os.str());
  im->setPosition(mPos);
  im->setVirtualY(100);
  setSurface(im);
}
std::string AntHouse::xmlName() const
{
  return "antHouse";
}
void AntHouse::saveXML(xmlpp::Node &node) const
{
  AntEntity::saveXML(node);
  node.set("name",mName);

  // load resources
  node.set("wood",toString(mWood));
  node.set("steel",toString(mSteel));
  node.set("stone",toString(mStone));
  node.set("water",toString(mWater));
  node.set("food",toString(mFood));
}
void AntHouse::loadXML(const xmlpp::Node &node)
{
  AntEntity::loadXML(node);
  mName=node.get("name");

  int men=toInt(node.get("men"));

  if(men>0)
    {
      AntEntity *e=new AntMan(getPos2D(),0,this);
      getMap()->insertEntity(e);
    }

  mWood=toInt(node.get("wood"));
  mSteel=toInt(node.get("steel"));
  mStone=toInt(node.get("stone"));
  mWater=toInt(node.get("water"));
  mFood=toInt(node.get("food"));
}

Job *AntHouse::getJob(AntEntity *pEntity)
{
  return 0;
}
