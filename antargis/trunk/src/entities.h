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

#include "ag_xml.h"
#include <ag_fs.h>

#include <set>
#include <vector>
#include <algorithm>


class AntTree:public AntEntity
  {
    int typeID;
  public:
    AntTree():typeID(0)
    {}
    AntTree(const Pos2D &p,int ID):AntEntity(p),typeID(ID)
    {}
    VoxelImage *getSurface() const
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
        /*
        else if(typeID==2)
          os<<"tower2";//_"<<id;
        else if(typeID==1)
          os<<"house2a";
        else if(typeID==3)
          os<<"barn1a";
        else if(typeID==4)
          os<<"blacksmith2";
        else if(typeID==5)
          os<<"monument";*/
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
    virtual std::string xmlName() const
      {
        return "antTree";
      }
    virtual void saveXML(xmlpp::Node &node) const;
    virtual void loadXML(const xmlpp::Node &node);
  };

class AntDeco:public AntEntity
  {
    int typeID;
  public:
    AntDeco():typeID(0)
    {}
    AntDeco(const Pos2D &p,int ID):AntEntity(p),typeID(ID)
    {}
    VoxelImage *getSurface() const
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

class AntMan;

class AntHero:public AntEntity
  {
    int typeID;

    std::string mName;

    std::set
      <AntMan*> mMen;

  public:
    AntHero():typeID(0)
  {}
    AntHero(const Pos2D &p,int ID,const std::string &pName);
    virtual ~AntHero();
    VoxelImage *getSurface() const;
    void signUp(AntMan *man);

    int getTypeID() const;
    void fightHero(AntHero *h);
    void goTo(int prio,const Pos2D &pos);
    void discard(AntMan *man);

    float calcTroupStrength() const;

    AntHero *fights(); // returns 0 if is not fighting otherwise pointer to other hero
    void gotFight(AntEntity *e)
    {
      if(!fights())
        {
          AntHero *h=e->getHero();

          if(h)
            fightHero(h);
        }
    }

    Pos2D getFormation(AntMan *m) const
      {
        size_t count=mMen.size();

        // if(count>20) FIXME: do second circle

        std::set
          <AntMan*>::const_iterator j=mMen.begin();
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

    virtual AntHero *getHero()
    {
      return this;
    }
    virtual float getHealSpeed() const
      {
        if(hasJob())
          return mHealSpeed*0.2;
        else
          return mHealSpeed;
      }
    virtual std::string xmlName() const
      {
        return "antHero";
      }
    virtual void saveXML(xmlpp::Node &node) const;
    virtual void loadXML(const xmlpp::Node &node);

  };

class AntMan: public AntEntity
  {
    int typeID;
    AntHero *mHero;
    int mHeroID;

  public:
    AntMan():typeID(0),mHero(0),mHeroID(0)
    {}
    AntMan(const Pos2D &p,int pTypeID,AntHero *pHero):AntEntity(p),typeID(pTypeID),mHero(pHero),mHeroID(0)
    {
      if(pHero)
        pHero->signUp(this);
    }
    virtual ~AntMan()
    {
      if(mHero)
        mHero->discard(this);
    }
    
    virtual std::string getTexture() const
    {
      std::ostringstream os;
      os<<"man"<<mDirNum;
      return os.str();
    }

    
    virtual void move(float pTime)
    {
      if(mHeroID && !mHero)
        {
          mHero=dynamic_cast<AntHero*>(getMap()->getEntity(mHeroID));
          mHeroID=0;
          mHero->signUp(this);
        }
      if(mHero && !mJob)
        {
          setJob(new MoveJob(0,mHero->getPos2D()+mHero->getFormation(this),0,true));//Pos2D()));
        }
      else
        {
          // search house here
        }
      AntEntity::move(pTime);
    }
    VoxelImage *getSurface() const
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
        return im;
      }

    void discard(AntHero *hero)
    {
      assert(mHero==hero);
      mHero=0;
    }
    virtual AntHero *getHero()
    {
      return mHero;
    }
    virtual float getHealSpeed() const
      {
        if(hasJob())
          return mHealSpeed*0.2;
        else
          return mHealSpeed;
      }
    virtual std::string xmlName() const
      {
        return "antMan";
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
