/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * entities.h
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

#ifndef ANT_ENTITIES_H
#define ANT_ENTITIES_H

#include "decast.h"
#include "quadtree.h"
#include "voxel_gen.h"
#include "tree.h"
#include "map.h"
#include "ant_hero.h"

#include "ag_xml.h"

#include <ag_fs.h>

#include <set>
#include <vector>
#include <algorithm>

class AntDeco:public AntEntity
  {
    int typeID;
  public:
    AntDeco():typeID(0)
    {}
    AntDeco(const Pos2D &p,int ID):AntEntity(p),typeID(ID)
    {}
    VoxelImage *getSurface()
      {
        std::ostringstream os;
        VoxelImage *im=0;
        
        os<<"deco"<<typeID;
        
        if(!fileExists(TILEDIR+os.str()+".png"))
          throw std::string("File not found")+os.str();
        im=new VoxelImage(os.str());
        im->setPosition(mPos);
        im->setVirtualY(100);
        return im;
      }
    virtual std::string xmlName() const
      {
        return "antDeco";
      }
    virtual void saveXML(xmlpp::Node &node) const;
    virtual void loadXML(const xmlpp::Node &node);
  };

// Computer player
class AntPlayer
  {
    std::set
      <AntHero*> mHeroes;
    int id;
  public:
    AntPlayer(int pID):id(pID)
    {
      getMap()->addPlayer(this);
    }
    virtual ~AntPlayer()
    {
      getMap()->removePlayer(this);
    }

    void insertHero(AntHero *pHero)
    {
      mHeroes.insert(pHero);
    }
    void removeHero(AntHero *pHero)
    {
      mHeroes.erase(pHero);
    }

    void move(float /*pTime*/)
    {
      // so add some heuristic here

      std::set
        <AntHero*>::iterator i=mHeroes.begin();
      for(;i!=mHeroes.end();i++)
        {
          AntHero *h=(*i)->fights();
          if(h)
            {
              // ok, is fighting - check for stats
              float factor=3.5; // as this hero shouldn't give up to early
              // is fighting - check for stats
              if(h->calcTroupStrength()>(*i)->calcTroupStrength()*factor)
                {
                  // we will lose, so flee
                  flee(*i,h->getPos2D(),100,10);
                }
            }
          else if(!(*i)->hasJob())
            {
              // do nothing at all
              
              //flee(*i,Pos2D(-10+rand()%20,-10+rand()%20),10,0);
              // hero is not fighting, so do something stupid
            }
        }
    }
    void flee(AntHero *h,Pos2D from,int distance,int p) // from is position of attacking person
    {
      Pos2D diff=h->getPos2D()-from;
      diff=diff.normalized()*distance; // flee by 100 pixels
      h->goTo(p,h->getPos2D()+diff);
    }
    virtual void saveXML(xmlpp::Node &node) const;
    virtual void loadXML(const xmlpp::Node &node);
  };


#endif
