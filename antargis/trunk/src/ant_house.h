/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_house.h
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

#ifndef ANT_HOUSE_H
#define ANT_HOUSE_H

#include "decast.h"
#include "quadtree.h"
#include "voxel_gen.h"
#include "tree.h"

#include "ag_xml.h"

#include <ag_fs.h>

#include <set>
#include <vector>
#include <algorithm>

#include "ant_boss.h"

class AntHouse:public AntBoss
{
  std::string mName;
 public:
  AntHouse(std::string pName="tower2"):mName(pName)
    {
    }
    AntHouse(const Pos2D &p,std::string pName="tower1"):AntBoss(p),mName(pName)
      {
      }
      virtual ~AntHouse()
	{
	}
    VoxelImage *getSurface() const
      {
        std::ostringstream os;
        VoxelImage *im=0;
        
        os<<mName;
        
        if(!fileExists(TILEDIR+os.str()+".png"))
          throw std::string("File not found")+os.str();
        im=new VoxelImage(os.str());
        im->setPosition(mPos);
        im->setVirtualY(100);
        return im;
      }
    virtual std::string xmlName() const
      {
        return "antHouse";
      }
};

#endif
