/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_tree.cc
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

#include "ant_tree.h"

AntTree::AntTree():typeID(0)
{}
AntTree::AntTree(const Pos2D &p,int ID):AntEntity(p),typeID(ID)
{}
VoxelImage *AntTree::getSurface() const
{
  std::ostringstream os;
  VoxelImage *im=0;
  
  os<<"tree"<<typeID;
  
  if(!fileExists(TILEDIR+os.str()+".png"))
    {
      im=makeTree();
      im->save(os.str());
      delete im;
      im=0;
    }

  if(!im)
    im=new VoxelImage(os.str());//imageCache()->getImage(os.str());
  im->setPosition(mPos);
  /*        if(typeID==1)
	    im->setCenter(Pos2D(100,150)+Pos2D(0,64));
	    if(typeID==2||typeID==3)
	    im->setCenter(Pos2D(100,150)+Pos2D(0,64));*/
  im->setVirtualY(100);
  return im;
}
std::string AntTree::xmlName() const
{
  return "antTree";
}

/************************************************************************
* AntTree
************************************************************************/

void AntTree::saveXML(xmlpp::Node &node) const
{
  AntEntity::saveXML(node);
  node.set("typeID",toString(typeID));
}
void AntTree::loadXML(const xmlpp::Node &node)
{
  AntEntity::loadXML(node);
  typeID=toInt(node.get("typeID"));
}

