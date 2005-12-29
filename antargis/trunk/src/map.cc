#include "scene.h"
#include "map.h"
#include "entity.h"
#include <vector>
#include <math.h>
#include <ag_debug.h>
#include "mesh.h"
#include "terrain.h"
#include "quadtree.h"



//////////////////////////////////////////////////////////////////////////
// AntMap
//////////////////////////////////////////////////////////////////////////

AntMap *myAntargisMap=0;
AntMap *getMap()
{
  return myAntargisMap;
}

AntMap::AntMap(int w,int h):HeightMap(w,h),mEntQuad(new QuadTree<AntEntity>(AGRect2(0,0,w,h)))
{
  myAntargisMap=this;
  maxID=0;

  mTerrain->addToScenes();
}
AntMap::~AntMap()
{
  CTRACE;
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
  CTRACE;
  return maxID++;
}

void AntMap::saveXML(xmlpp::Node &node) const
{
  HeightMap::saveXML(node);
  
  // entities
  std::list<AntEntity*>::const_iterator i=mEntities.begin();
  for(;i!=mEntities.end();i++)
    {
      xmlpp::Node &child=node.newChild((*i)->xmlName());
      (*i)->saveXML(child);
    }
  
}


void AntMap::loadXML(const xmlpp::Node &node)
{
  HeightMap::loadXML(node);

  xmlpp::Node::const_iterator i=node.begin();
  for(;i!=node.end();i++)
    {
      TRACE;
      AntEntity *e=0;
      Node *n=*i;
      if((e=loadEntity(*n)))
        {
	  e->loadXML(*n);
	  insertEntity(e);
	}
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
}

void AntMap::removeEntity(AntEntity *p)
{
  mToDel.push_back(p);
}


void AntMap::move(float pTime)
{
  //  return;
  // first remove entities, which shall be deleted
  std::list<AntEntity*>::iterator d=mToDel.begin();
  for(;d!=mToDel.end();d++)
    {
      EntityList::iterator i=std::find(mEntities.begin(),mEntities.end(),*d);
      if(i!=mEntities.end())
        mEntities.erase(i);
      mEntityMap.erase((*d)->getID());
      mEntQuad->remove(*i);

      saveDelete(*d);
      /*      AntEntity::Meshes meshes=(*d)->getMesh();
      for(AntEntity::Meshes::iterator i=meshes.begin();i!=meshes.end();i++)
      getScene()->removeNode(*i);*/
    }
  //  if(mToDel.size())
  //    entsChanged();

  mToDel.clear();

  // now move all entities

  EntityList::iterator i=mEntities.begin();

  for(;i!=mEntities.end();i++)
    {
      (*i)->move(pTime);
    }
}


AntEntity *AntMap::loadEntity(const Node &node)
{
  AntEntity *e=0;
  cdebug("type unknown:"<<node.getName());
  return e;
}

AntMap::EntityList AntMap::getEntities(const AGRect2&r)
{
  return mEntQuad->get(r);
}
AntMap::EntityList AntMap::getAllEntities()
{
  return mEntities;
}
std::vector<AntEntityPtr> AntMap::getAllEntitiesV()
{
  std::vector<AntEntityPtr> v;
  std::copy(mEntities.begin(),mEntities.end(),std::back_inserter(v));
  return v;
}
std::vector<AntEntityPtr> AntMap::getEntities(const std::string &pName)
{
  std::vector<AntEntityPtr> v;
  for(EntityList::iterator i=mEntities.begin();i!=mEntities.end();i++)
    if((*i)->getType()==pName)
      v.push_back(*i);
  return v;
}


AntEntity *AntMap::getNext(AntEntity *me,const std::string &pType)
{
  // FIXME: optimize this - use quadtree

  std::multimap<float,AntEntity*> ents;


  EntityList::iterator i=mEntities.begin();
  AGVector2 p=me->getPos2D(); // FIXME: check for reachability, too ??
  //  float dist=0;

  for(;i!=mEntities.end();i++)
    {
      if(me!=*i)
        {
          if((*i)->getType()==pType)
            {
              AGVector2 p2=(*i)->getPos2D()-p;
              float norm=p2.length2();
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
      assert((j!=ents.end()));
      e=j->second;
      //      cdebug("DIST:"<<j->first);
    }

  return e;
}
AntEntity *AntMap::getByName(const std::string &pName)
{
  EntityList::iterator i=mEntities.begin();

  for(;i!=mEntities.end();i++)
    {
      if((*i)->getName()==pName)
        return *i;
    }
  return 0;
}

void AntMap::loadMap(const std::string &pFilename)
{
  
  //getScene()->clear();
  CTRACE;
  Document d;
  std::string c=loadFile(pFilename);
  if(c.length())
    {
      d.parse_memory(c);
      clear();
      loadXML(d.root());
    }
}
void AntMap::saveMap(const std::string &pFilename)
{
  Document d;
  Node *root=d.get_root_node();
  root->setName("antargisLevel");
  cdebug("root:"<<root);
  saveXML(*root);

  std::string c=d.toString();
  cdebug(c);
  saveFile(pFilename,c);
}

void AntMap::clear()
{
  //  getScene()->clear();
  CTRACE;
  //  mPlayers.clear();
  mEntities.clear();
  mEntQuad->clear();
  mEntityMap.clear();
  //  getScene()->addNode(mTerrainMesh);
  mTerrain->addToScenes();

}
/*
void AntMap::entsChanged()
{
  if(mScene)
    {
      for(EntityList::iterator i=mEntities.begin();i!=mEntities.end();i++)
	{
	  AntEntity::Meshes m=(*i)->getMesh();
	  for(AntEntity::Meshes::iterator i=m.begin();i!=m.end();i++)
	    if(*i)
	      mScene->addNode(*i);
	}
    }
    }*/

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







//bool markingFinished=true;

void AntMap::mark()
{
  AntMap::EntityList::iterator i=mEntities.begin();

  for(;i!=mEntities.end();i++)
    markObject(*i);
}


