/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * map.cpp
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

#include "map.h"
#include "voxel.h"
#include <ag_fs.h>
#include <cstdlib>
#include "entities.h"
#include "ant_tree.h"
#include "ant_man.h"
#include "ant_house.h"
#include <map>

/************************************************************************
* AntargisMap
************************************************************************/

AntargisMap *myAntargisMap=0;

AntargisMap *getMap()
{
  return myAntargisMap;
}


AntargisMap::AntargisMap(int w,int h):
    mHeight(POINTS_PER_TILE*(w+2),POINTS_PER_TILE*(w+2),1,10,20),
    mGrass(POINTS_PER_TILE*(w+2),POINTS_PER_TILE*(w+2),2,5,10), // 1 is no-var.
    mEntities(Rect2D(0,0,w*TILE_SIZE,h*TILE_SIZE)),mW(w),mH(h),mRubyObject(false)
{
  myAntargisMap=this;
  mPaused=false;
  maxID=0;
}

AntargisMap::~AntargisMap()
{
  CTRACE;
}

std::list<AntEntity*> AntargisMap::getAllEntities()
{
  return mEntList;
}

    
Pos2D AntargisMap::truncPos(const Pos2D &p) const
{
  Pos2D maxPos=getMaxPos();
  return Pos2D(std::max(0.0f,std::min(maxPos.x,p.x)),
               std::max(0.0f,std::min(maxPos.y,p.y)));
}

int AntargisMap::getNewID()
{
  return maxID++;
}

void AntargisMap::insertListener(MapListener *l)
{
  mListeners.insert(l);
}

void AntargisMap::removeListener(MapListener *l)
{
  mListeners.erase(l);
}


int AntargisMap::width() const
{
  return mW;
}
int AntargisMap::height() const
{
  return mH;
}

float AntargisMap::getHeight(const Pos2D &p) const
  {
    float mx=float(p.x*POINTS_PER_TILE)/TILE_WIDTH;
    float mz=float(p.y*POINTS_PER_TILE)/TILE_WIDTH;

    return mHeight.get(mx,mz);
  }
float AntargisMap::getGHeight(const Pos2D &p) const
  {
    float mx=float(p.x*POINTS_PER_TILE)/TILE_WIDTH;
    float mz=float(p.y*POINTS_PER_TILE)/TILE_WIDTH;

    return mGrass.get(mx,mz);
  }
  
Pos3D AntargisMap::getNormal(int x,int y) const
 {
    float mx=float(x*POINTS_PER_TILE)/TILE_WIDTH;
    float my=float(y*POINTS_PER_TILE)/TILE_WIDTH;

   return mHeight.getNormal(mx,my);
 }


SplineMapD AntargisMap::getPatchH(int x,int y) const
  {
    return mHeight.getPatch(x,y,POINTS_PER_TILE,POINTS_PER_TILE);
  }
SplineMapD AntargisMap::getPatchG(int x,int y) const
  {
    return mGrass.getPatch(x,y,POINTS_PER_TILE,POINTS_PER_TILE);
  }

Pos3D AntargisMap::getPos3D(const Pos2D &p) const
  {
    return Pos3D(p.x,getHeight(p),p.y);
  }

Pos2D AntargisMap::getMaxPos() const
{
  return Pos2D(mW*TILE_WIDTH/POINTS_PER_TILE-2,mH*TILE_WIDTH/POINTS_PER_TILE-2);
}
  
  
void AntargisMap::insertEntity(AntEntity *e)
{
  mEntities.insert(e);
  mEntList.push_back(e);
  mEntityMap[e->getID()]=e;
  if(e->mRubyObject)
    {
      VALUE rubyAnimal = e->mRUBY;

      rb_gc_register_address(&rubyAnimal);
      //      rb_gc_mark(rubyAnimal);
      cdebug("mark:");
   }

}
    
void AntargisMap::removeEntity(AntEntity *p)
{
  mEntities.remove(p);
  std::list<AntEntity*>::iterator i=std::find(mEntList.begin(),mEntList.end(),p);
  if(i!=mEntList.end())
    mEntList.erase(i);
  mEntityMap.erase(p->getID());
}

std::list<AntEntity*> AntargisMap::getEntities(const AntRect&r)
{
  return mEntities.get(r);
}

void AntargisMap::move(float pTime)
{
  if(mPaused)
    return;
  // first move computer-players (they decide what to do)
  //  rb_eval_string("GC.disable");

  std::set
    <AntPlayer*>::iterator j=mPlayers.begin();
  for(;j!=mPlayers.end();j++)
    (*j)->move(pTime);


  // now move all entities

  std::list<AntEntity*>::iterator i=mEntList.begin();

  for(;i!=mEntList.end();i++)
    {
      //      cdebug("ent:"<<*i);
      (*i)->move(pTime);
    }
  //  rb_eval_string("GC.enable");
}


void AntargisMap::killHero(AntHero *h)
{
  std::set
    <AntPlayer*>::iterator i=mPlayers.begin();

  for(;i!=mPlayers.end();i++)
    (*i)->removeHero(h);

}

void AntargisMap::setAllWater()
{
  int x,y;
  for(x=0;x<mHeight.width();x++)
    for(y=0;y<mHeight.height();y++)
    {
      mHeight.setPoint(x,y,0);
    }
}
void AntargisMap::setAllLand()
{
  int x,y;
  for(x=0;x<mHeight.width();x++)
    for(y=0;y<mHeight.height();y++)
    {
      mHeight.setPoint(x,y,15);
    }
  
}

void AntargisMap::addFlat(int x,int y,int h,int r)
{
  if(r==0)
    mHeight.edit(x,y,h);
  else
  {
    float mmin,mmax;
    mmin=mmax=0.0f;
    bool aset=false;
    for(int i=-r;i<=r;i++)
      for(int j=-r;j<=r;j++)
      {
        if(i*i+j*j<r*r)
        {
        float v=mHeight.getPoint(x+i,y+j);
        if(!aset)
          mmin=mmax=v;
        else
        {
          if(mmin>v)
            mmin=v;
          if(mmax<v)
            mmax=v;
        }
        }
      }
    float nv;
    if(abs((int)(mmin-mmax))<1)
      nv=mmin+h;
    else
      nv=(mmin+mmax)*0.5;
      
    nv=float((int(nv)/5)*5);
    
    if(nv<WATER_HEIGHT)
      nv=0;
    
    
    for(int i=-r;i<=r;i++)
      for(int j=-r;j<=r;j++)
        if(i*i+j*j<r*r)
        {
        mHeight.setPoint(x+i,y+j,nv);
        }
  }
  
  // tell entities, that map has changed
  std::list<AntEntity*>::iterator i=mEntList.begin();
  for(;i!=mEntList.end();i++)
    (*i)->mapChanged();

}

void AntargisMap::addPyramid(int x,int y,int h,int r)
{
  if(r==0)
    mHeight.edit(x,y,h);
  else
  {
    for(int i=-r;i<=r;i++)
      for(int j=-r;j<=r;j++)
        mHeight.edit(x+i,y+j,(int)(h*sqrt((float)i*i+j*j)/r));
  }
  
  // tell entities, that map has changed
  std::list<AntEntity*>::iterator i=mEntList.begin();
  for(;i!=mEntList.end();i++)
    (*i)->mapChanged();

}

void AntargisMap::saveXML(xmlpp::Node &node) const
  {
    // entities
    std::list<AntEntity*>::const_iterator i=mEntList.begin();
    for(;i!=mEntList.end();i++)
      {
        xmlpp::Node &child=node.newChild((*i)->xmlName());
        (*i)->saveXML(child);
      }
      
    // height and grass map
    xmlpp::Node &hmap=node.newChild("heightMap");
    xmlpp::Node &gmap=node.newChild("grassMap");
    hmap.set("width",toString(mW));
    hmap.set("height",toString(mH));
    gmap.set("width",toString(mW));
    gmap.set("height",toString(mH));
    std::ostringstream hmaps,gmaps;
    for(int j=0;j<mH;j++)
      {
        for(int i=0;i<mW;i++)
          {
            hmaps<<(int)(mHeight.getPoint(i,j))<<" ";
            gmaps<<(int)(mGrass.getPoint(i,j))<<" ";
          }
        hmaps<<std::endl;
        gmaps<<std::endl;
      }
    cdebug(hmaps.str());
    hmap.setContent(hmaps.str());
    gmap.setContent(gmaps.str());
    
    // players
    std::set<AntPlayer*>::const_iterator k=mPlayers.begin();
    
    for(;k!=mPlayers.end();k++)
    {
      xmlpp::Node &child=node.newChild("player");
      (*k)->saveXML(child);
    }

  }

AntEntity *AntargisMap::loadEntity(const xmlpp::Node &node)
{
  std::string n=node.getName();
  AntEntity *e=0;
  if(n=="antTree")
    e=new AntTree;
  else if(n=="antDeco")
    e=new AntDeco;
  else if(n=="antHero")
    e=new AntHero;
  else if(n=="antMan")
    e=new AntMan;
  else if(n=="antHouse")
    e=new AntHouse;
  return e;
}

void AntargisMap::loadXML(const xmlpp::Node &node)
{
  xmlpp::Node::const_iterator i=node.begin();
  for(;i!=node.end();i++)
    {
      AntEntity *e=0;
      if(i->getName()=="heightMap")
        {
          CTRACE;
          cdebug("content:"<<i->getContent());
          cdebug("---");
          std::istringstream hmaps;
          hmaps.str(i->getContent());
          // parse height map
          float h;
          cdebug("mH:"<<mH);
          cdebug("mW:"<<mW);
          for(int j=0;j<mH;j++)
            {
              for(int i=0;i<mW;i++)
                {
                  hmaps>>h;
                  mHeight.setPoint(i,j,h);
                }
              cdebug("j:"<<j);
            }
        }
      else if((e=loadEntity(*i)))
	{
	}
      /*       else if(i->getName()=="antTree")
        e=new AntTree;
       else if(i->getName()=="antDeco")
        e=new AntDeco;
       else if(i->getName()=="antHero")
        e=new AntHero;
       else if(i->getName()=="antMan")
        e=new AntMan;
       else if(i->getName()=="antHouse")
       e=new AntHouse;*/
      else if(i->getName()=="player")
        {
          AntPlayer *p=new AntPlayer(-1);
          p->loadXML(*i);
          mPlayers.insert(p);
        }
       
       if(e)
       {
        e->loadXML(*i);
        insertEntity(e);
       }
        
    }
  // tell entities, that map has changed
  std::list<AntEntity*>::iterator k=mEntList.begin();
  for(;k!=mEntList.end();k++)
    (*k)->mapChanged();
}

void AntargisMap::clear()
{
  CTRACE;
  mPlayers.clear();
  mEntities.clear();
  mEntList.clear();
  mEntityMap.clear();
  std::set<MapListener*>::iterator i=mListeners.begin();
  for(;i!=mListeners.end();i++)
    (*i)->mapUpdate();
}

void AntargisMap::endChange()
{
  std::set<MapListener*>::iterator i=mListeners.begin();
  for(;i!=mListeners.end();i++)
    (*i)->mapUpdate();
}
void AntargisMap::saveMap(const std::string &pFilename)
{
  xmlpp::Document d;
  Node *root=createRootNode(d,"antargisLevel");
  saveXML(*root);
  std::string c=toString(d);
  cdebug(c);
  saveFile(pFilename,c);
}
void AntargisMap::loadMap(const std::string &pFilename)
{
  CTRACE;
  Document d;
  std::string c=loadFile(pFilename);
  if(c.length())
    {
      d.parse_memory(c);
      clear();
      loadXML(d.root());
    }

  std::set<MapListener*>::iterator i=mListeners.begin();
  for(;i!=mListeners.end();i++)
    (*i)->mapUpdate();
}


/*****************************************************************
* AntEntity
*****************************************************************/



/************************************************************************
* AntDeco
************************************************************************/

void AntDeco::saveXML(xmlpp::Node &node) const
{
  AntEntity::saveXML(node);
  node.set("typeID",toString(typeID));
}
void AntDeco::loadXML(const xmlpp::Node &node)
{
  AntEntity::loadXML(node);
  typeID=toInt(node.get("typeID"));
}




/************************************************************************
* AntPlayer
************************************************************************/
void AntPlayer::saveXML(xmlpp::Node &node) const
{
   node.set("playerID",toString(id));
   std::set<AntHero*>::const_iterator i=mHeroes.begin();
   for(;i!=mHeroes.end();i++)
   {
    xmlpp::Node &child=node.newChild("hero");
    child.set("bossID",toString((*i)->getID()));
   }
}
void AntPlayer::loadXML(const xmlpp::Node &node)
{
  std::set<int> mHIDs;
  // FIXME: load heroes
  xmlpp::Node::const_iterator i=node.begin();
  for(;i!=node.end();i++)
  {
    if(i->getName()=="hero")
    {
      int id=toInt(i->get("bossID"));
      AntHero *h=dynamic_cast<AntHero*>(getMap()->getEntity(id));
      if(!h)
        cdebug("Hero not found!");
      else
        mHeroes.insert(h);
    }
  }
}

// MapListener
MapListener::MapListener()
{
  getMap()->insertListener(this);
}

MapListener::~MapListener()
{
  getMap()->removeListener(this);
}

void MapListener::mapUpdate()
{
}

bool markingFinished=true;

void AntargisMap_markfunc(void *ptr)
{
  cdebug("TRACE");
  if(!markingFinished)
    return;
#ifdef USE_RUBY
  markingFinished=false;
  AntEntity *cppAnimal;
  VALUE   rubyAnimal;
  AntargisMap *zoo;
  
  TRACE;  
  cdebug(ptr<<endl);
  zoo = static_cast<AntargisMap*>(ptr);
  
  std::list<AntEntity*>::iterator i=zoo->mEntList.begin();

  for(;i!=zoo->mEntList.end();i++)
    {
      cdebug("children:"<<*i);
      cppAnimal = *i;//zoo->getAnimal(i);
      assert(!cppAnimal->mDeleted);
      if(cppAnimal->mRubyObject)
	{
	  rubyAnimal = cppAnimal->mRUBY;//SWIG_RubyInstanceFor(cppAnimal);
	  rb_gc_mark(rubyAnimal);
	  cdebug("mark:");//<<cppAnimal->getName());
	}
      //      AntEntity_markfunc(*i);
    }
  markingFinished=true;
#endif
}

AntEntity *AntargisMap::getNext(AntEntity *me,const std::string &pType)
{
  // FIXME: optimize this - use quadtree

  std::multimap<float,AntEntity*> ents;

  
  std::list<AntEntity*>::iterator i=mEntList.begin();
  Pos2D p=me->getPos2D(); // FIXME: check for reachability, too ??
  float dist=0;

  for(;i!=mEntList.end();i++)
    {
      if(me!=*i)
	{
	  if((*i)->getType()==pType)
	    {
	      Pos2D p2=(*i)->getPos2D()-p;
	      float norm=p2.norm2();
	      ents.insert(std::make_pair(norm,*i));
	    }
	}
    }


  // take one of the nearest, but no farer away than 30% of nearest
  AntEntity *e=0;
  if(ents.size())
    {
      std::multimap<float,AntEntity*>::iterator j=ents.begin();
      float nearest=j->first;
      int r=rand()%std::min(ents.size(),5U);
      while(r>0 && (j->first<=nearest*1.3 || j->first<2000*2000))
	{
	  j++;
	  r--;
	}
      if(r>0)
	j--;
      assert(j!=ents.end());
      e=j->second;
      cdebug("DIST:"<<j->first);
    }

  return e;
}

