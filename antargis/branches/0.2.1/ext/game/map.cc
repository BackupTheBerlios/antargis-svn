/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * map.h
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

#include "scene.h"
#include "ag_rand.h"
#include "map.h"
#include "entity.h"
#include <vector>
#include <math.h>
#include <ag_debug.h>
#include "mesh.h"
#include "terrain.h"
#include "quadtree.h"
#include "anim_mesh.h"
#include "mesh_2d.h"
#include "ag_profiler.h"



//////////////////////////////////////////////////////////////////////////
// AntMap
//////////////////////////////////////////////////////////////////////////

AntMap *myAntargisMap=0;
AntMap *getMap()
{
  assert(myAntargisMap);
  return myAntargisMap;
}

AntMap::AntMap(SceneBase *pScene,int w,int h):
  HeightMap(pScene,w,h),
  mEntQuad(new QuadTree<AntEntity>(AGRect2(0,0,w,h))),
  mHeuristicFunction(0)
{
  myAntargisMap=this;
  maxID=0;
}
AntMap::~AntMap()
{
  CTRACE;
  printf("%lX\n",mRUBY);
  if(myAntargisMap==this)
    myAntargisMap=0;
}

AGVector3 AntMap::getPos(const AGVector2 &pPos) const
{
  return AGVector3(pPos[0],pPos[1],getHeight(pPos[0],pPos[1]));
}



AntEntity *AntMap::getEntity(int id) const
{
  EntityMap::const_iterator i=mEntityMap.find(id);
  if(i==mEntityMap.end())
    return 0;
  return i->second;
}

// FIXME: exchange this function somehow!!
int AntMap::getNewID()
{
  return maxID++;
}

void AntMap::useID(int id)
{
  maxID=std::max(maxID,id+1);
}


void AntMap::saveXML(Node &node) const
{
  HeightMap::saveXML(node);
  
  // entities
  std::list<AntEntity*>::const_iterator i=mEntities.begin();
  for(;i!=mEntities.end();i++)
    {
      Node &child=node.addChild((*i)->xmlName());
      (*i)->saveXML(child);
    }
  
}


void AntMap::loadXML(const Node &node)
{
  HeightMap::loadXML(node);

  Node::const_iterator i=node.begin();
  for(;i!=node.end();i++)
    {
      //      AntEntity *e=0;
      Node *n=*i;
      processXMLNode(*n);
      /*        {
	  e->loadXML(*n);
	  insertEntity(e);
	  }*/
    }
  // tell entities, that map has changed
  std::list<AntEntity*>::iterator k=mEntities.begin();
  for(;k!=mEntities.end();k++)
    (*k)->eventMapChanged();
}

void AntMap::insertEntity(AntEntity *e)
{
  //  cdebug("INSERTING:"<<e);
  mEntities.push_back(e);
  //  mEntList.push_back(e);
  mEntityMap[e->getID()]=e;
  mEntQuad->insert(e);
  //  entsChanged();
  //  mByType[e->getType()].insert(e);
}

void AntMap::removeEntity(AntEntity *p)
{
  {
    EntityList::iterator i=std::find(mToDel.begin(),mToDel.end(),p);
    if(i!=mToDel.end())
      {
	cdebug("Entity already deleted:"<<p);
	return;
      }
  }


  EntityList::iterator i=std::find(mEntities.begin(),mEntities.end(),p);
  if(i!=mEntities.end())
    mToDel.push_back(p);
  else
    {
      cdebug("ERROR: Entity not found while removing:"<<p);
      //      throw std::runtime_error("ERROR: Entity not found while removing");
    }
}


void AntMap::move(float pTime)
{
  STACKTRACE; 
  //return;
  // first remove entities, which shall be deleted
  std::list<AntEntity*>::iterator d=mToDel.begin();
  for(;d!=mToDel.end();d++)
    {
      EntityList::iterator i=std::find(mEntities.begin(),mEntities.end(),*d);
      if(i!=mEntities.end())
	mEntities.erase(i);
      else
	cdebug("ERROR: Entity not found while deleting:"<<*d);
      mEntityMap.erase((*d)->getID());
      mEntQuad->remove(*i);
      //      mByType[(*d)->getType()].erase(*d);

      saveDelete(*d);
    }
  mToDel.clear();

  // now move all entities

  EntityList::iterator i=mEntities.begin();

  for(;i!=mEntities.end();i++)
    {
      (*i)->move(pTime);
    }
}


void AntMap::processXMLNode(const Node &node)
{
  //  AntEntity *e=0;
  cdebug("type unknown:"<<node.getName());
  //  return e;
}

/*std::vector<AntEntityPtr> AntMap::getByType(const AGString &pType)
{
  std::vector<AntEntityPtr> l;
  EntityByType::iterator i=mByType.find(pType);
  if(i!=mByType.end())
    {
      std::copy(i->second.begin(),i->second.end(),std::back_inserter(l));
    }

  return l;  
  }*/


AntMap::EntityList AntMap::getEntities(const AGRect2&r)
{
  return mEntQuad->get(r);
}
AntMap::EntityList AntMap::getAllEntities()
{
  return mEntities;
}
/*
std::vector<AntEntityPtr> AntMap::getAllEntitiesV()
{
  std::vector<AntEntityPtr> v;
  std::copy(mEntities.begin(),mEntities.end(),std::back_inserter(v));
  return v;
}
*/

std::vector<AntEntity*> AntMap::getEntities(const AGString &pName)
{
  std::vector<AntEntity*> v;
  for(EntityList::iterator i=mEntities.begin();i!=mEntities.end();i++)
    if((*i)->provides(pName))
      v.push_back(*i);
  return v;
}

std::vector<AntEntity*> AntMap::getNextList(AntEntity *me,const AGString &pType,size_t atLeast)
{
  //  assert(mHeuristicFunction);

  // FIXME: optimize this - use quadtree

  std::multimap<float,AntEntity*> ents;


  EntityList::iterator i=mEntities.begin();
  AGVector2 p=me->getPos2D(); // FIXME: check for reachability, too ??

  for(;i!=mEntities.end();i++)
    {
      if(me!=*i)
        {
          if((*i)->provides(pType) && (*i)->resource.get(pType)>=atLeast)
            {
	      /*
              AGVector2 p2=(*i)->getPos2D()-p;
              float norm=p2.length2();*/
	      float norm;
	      if(mHeuristicFunction)
		norm=(*mHeuristicFunction)(std::make_pair((*i)->getPos2D(),p));
	      else
		norm=((*i)->getPos2D()-p).length2();
              ents.insert(std::make_pair(norm,*i));
            }
        }
    }

  std::vector<AntEntity*> vec;

  // take one of the nearest, but no farer away than 30% of nearest
  //  AntEntity *e=0;
  if(ents.size())
    {
      std::multimap<float,AntEntity*>::iterator j=ents.begin();
      float nearest=j->first;
      for(;j!=ents.end();j++)
	{
	  if(j->first<=nearest*1.3)
	    vec.push_back(j->second);
	}
    }

  return vec;
}


AntEntity *AntMap::getNext(AntEntity *me,const AGString &pType,size_t atLeast)
{
  //  assert(mHeuristicFunction);
  // FIXME: optimize this - use quadtree

  std::multimap<float,AntEntity*> ents;


  EntityList::iterator i=mEntities.begin();
  AGVector2 p=me->getPos2D(); // FIXME: check for reachability, too ??
  //  float dist=0;

  for(;i!=mEntities.end();i++)
    {
      if(me!=*i)
        {
          if((*i)->provides(pType) && (*i)->resource.get(pType)>=atLeast)
            {
	      //              AGVector2 p2=(*i)->getPos2D()-p;
	      float norm;

	      if(mHeuristicFunction)
	        norm=(*mHeuristicFunction)(std::make_pair((*i)->getPos2D(),p));
	      else
		norm=((*i)->getPos2D()-p).length2();
	      cdebug("norm:"<<norm<<" i:"<<*i<<" name:"<<(*i)->getName());
	      //              float norm=p2.length2();
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
      int r=agRand(std::min((int)ents.size(),5));
      while(r>0 && (j->first<=nearest*1.3 || j->first<2000*2000))
        {
          j++;
          r--;
        }
      if(r>0)
        j--;
      assert((j!=ents.end()));
      e=j->second;
      //      cdebug("DIST:"<<j->first);
    }

  return e;
}

void AntMap::setHeuristic(HeuristicFunction *pFunction)
{
  mHeuristicFunction=pFunction;
}


AntEntity *AntMap::getByName(const AGString &pName)
{
  EntityList::iterator i=mEntities.begin();

  for(;i!=mEntities.end();i++)
    {
      if((*i)->getName()==pName)
        return *i;
    }
  return 0;
}

void AntMap::loadMapFromMemory(const AGData &pMem)
{
  if(pMem.length())
    {
      Document d;
      d.parseMemory(pMem);
      loadXML(d.root());
    }
}

void AntMap::loadMap(const AGFilename &pFilename)
{
  
  loadMapFromMemory(loadFile(pFilename));
}
void AntMap::saveMap(const AGFilename &pFilename)
{
  mName=AGString(pFilename);
  Document d;
  Node &root=d.root();
  root.setName("antargisLevel");
  //  cdebug("root:"<<root);
  saveXML(root);

  AGData c=d.toString();
  cdebug(c);
  saveFile(pFilename,c);
}

void AntMap::clear()
{
  //  getScene()->clear();
  CTRACE;
  //  mPlayers.clear();

  mEntQuad->clear();
  mEntityMap.clear();

  // save delete entities
  for(EntityList::iterator i=mEntities.begin();i!=mEntities.end();i++)
    saveDelete(*i);
  

  mEntities.clear();

}

void AntMap::mapChanged()
{
  HeightMap::mapChanged();
  // send info to entitites
  for(EntityList::iterator i=mEntities.begin();i!=mEntities.end();i++)
    (*i)->setPos((*i)->getPos2D());
}

AntEntity *AntMap::getEntity(const Mesh &pMesh)
{
  for(EntityList::iterator i=mEntities.begin();i!=mEntities.end();i++)
    {
      AntEntity::Meshes meshes=(*i)->getMesh();
      if(std::find(meshes.begin(),meshes.end(),&pMesh)!=meshes.end())
	//      if((*i)->getMesh()==&pMesh)
	return *i;
    }
  return 0;
}

AntEntity *AntMap::getEntity(const AnimMesh &pMesh)
{
  for(EntityList::iterator i=mEntities.begin();i!=mEntities.end();i++)
    {
      AntEntity::Meshes meshes=(*i)->getMesh();
      if(std::find(meshes.begin(),meshes.end(),&pMesh)!=meshes.end())
	return *i;
    }
  return 0;
}

AntEntity *AntMap::getEntity(const Mesh2D &pMesh)
{
  for(EntityList::iterator i=mEntities.begin();i!=mEntities.end();i++)
    {
      AntEntity::Meshes meshes=(*i)->getMesh();
      if(std::find(meshes.begin(),meshes.end(),&pMesh)!=meshes.end())
	//      if((*i)->getMesh()==&pMesh)
	return *i;
    }
  return 0;
}






//bool markingFinished=true;

void AntMap::mark()
{
  CTRACE;
  HeightMap::mark();
  AntMap::EntityList::iterator i=mEntities.begin();

  for(;i!=mEntities.end();i++)
    markObject(*i);

  // mark to del, too, otherwise it crashes when tidying before move() is called
  for(i=mToDel.begin();i!=mToDel.end();i++)
    markObject(*i);
}


void AntMap::newMap(int w,int h)
{
  clear();
  HeightMap::newMap(w,h);
}
