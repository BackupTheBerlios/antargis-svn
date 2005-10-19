#include "map.h"
#include "entity.h"
#include <vector>
#include <math.h>
#include <ag_debug.h>
#include "scene.h"
#include "mesh.h"
#include "terrain.h"

//////////////////////////////////////////////////////////////////////////
// HeightMap
//////////////////////////////////////////////////////////////////////////

std::vector<float> genSomeHeights(int mW,int mH,float mMaxHeight);

HeightMap::HeightMap(int w,int h):
  mW(w),mH(h),mScene(0)
{
  mHeights=genSomeHeights(w+2,h+2,5);
  mGrass=genSomeHeights(w+2,h+2,1);

  mTerrainMesh=new TerrainMesh(*this);
}

HeightMap::~HeightMap()
{
  delete mTerrainMesh;
}

void HeightMap::setScene(Scene *pScene)
{
  mScene=pScene;
}
Scene *HeightMap::getScene()
{
  return mScene;
}

void HeightMap::setHeight(float height)
{
  for(size_t y=0;y<mH+2;y++)
    for(size_t x=0;x<mW+2;x++)
      set(x,y,height);

  mapChanged();
}

void HeightMap::set(size_t x,size_t y,float height)
{
  assert(x>=0);
  assert(y>=0);
  assert(x<mW+2);
  assert(y<mH+2);
  mHeights[x+y*(mW+2)]=height;
}


void HeightMap::loadXML(const Node &node)
{
  mW=toInt(node.get("width"));
  mH=toInt(node.get("height"));

  cdebug("mW:"<<mW);
  cdebug("mH:"<<mH);
  mHeights=std::vector<float>(mW*mH*4);
  mGrass=std::vector<float>(mW*mH*4);
  Node::NodeVector hv=node.get_children("height");
  Node::NodeVector gv=node.get_children("grass");
  if(hv.size()==0 || gv.size()==0)
    return;
  assert(hv.size()==1);
  assert(gv.size()==1);
  Node &h=**hv.begin();
  Node &g=**gv.begin();
  
  std::istringstream ish(h.getContent());
  std::istringstream isg(g.getContent());

  cdebug(g.getContent());
  float f;
  for(size_t y=0;y<mH+2;y++)
    {
      for(size_t x=0;x<mW+2;x++)
	{
	  ish>>f;
	  mHeights[x+y*(mW+2)]=f;
	  isg>>f;
	  mGrass[x+y*(mW+2)]=f;
	}
    }
  mapChanged();
}

void HeightMap::mapChanged()
{
  if(mScene)
    {
      mScene->addNode(mTerrainMesh);
      mScene->mapChanged();
    }
}


void HeightMap::saveXML(Node &node) const
{
  node.set("width",toString(mW));
  node.set("height",toString(mH));

  std::ostringstream osh,osg;
  osh.precision(2);
  osg.precision(2);

  for(size_t y=0;y<mH+2;y++)
    {
      for(size_t x=0;x<mW+2;x++)
	{
	  osh<<get(x,y)<<" ";
	  osg<<getGrass(x,y)<<" ";
	}
      osh<<std::endl;
      osg<<std::endl;
    }
  node.newChild("height").setContent(osh.str());
  node.newChild("grass").setContent(osg.str());
}


std::vector<float> genSomeHeights(int mW,int mH,float mMaxHeight)
{
  std::vector<float> h(mW*mH);
  int randMax=10000;
  
  for(int i=0;i<mW*mH;i++)
    h[i]=(rand()%randMax)*mMaxHeight/randMax;
  
  int x,y;
  // blur
  
  for(int j=0;j<2;j++) // once
    for(x=0;x<mW-1;x++)
      for(y=0;y<mH-1;y++)
	h[x+y*mW]=(h[x+y*mW]+h[x+y*mW+1]+h[x+(y+1)*mW]+h[x+1+(y+1)*mW])/4;
  
  
  return h;
}

float HeightMap::get(size_t x,size_t y) const
{
  assert(x>=0);
  assert(y>=0);
  assert(x<mW+2);
  assert(y<mH+2);
  return mHeights[x+y*(mW+2)];
}
float HeightMap::getGrass(size_t x,size_t y) const
{
  assert(x>=0);
  assert(y>=0);
  assert(x<mW+2);
  assert(y<mH+2);
  return mGrass[x+y*(mW+2)];
}

AGVector4 HeightMap::getVertex(int x,int y)
{
  return AGVector4(x,y,getHeight(x,y),1);
}

float HeightMap::getHeight(float x,float y) const
{
  int mx=(int)(x);
  int my=(int)(y);

  float dx=x-mx;
  float dy=y-my;

  float a0=get(mx,my);
  float a1=get(mx+1,my);
  float a2=get(mx,my+1);
  float a3=get(mx+1,my+1);

  float h=(a0*(1-dx)+a1*dx)*(1-dy)+(a2*(1-dx)+a3*dx)*dy;
  return h;

}
AGVector3 HeightMap::getNormal(int x,int y) const
{
  AGVector4 v1=AGVector4(1,0,get(x+1,y)-get(x,y),0);
  AGVector4 v2=AGVector4(0,1,get(x,y+1)-get(x,y),0);
  
  AGVector4 v3=v2%v1;
  v3.normalize3();
  v3*=-1;
  return AGVector3(v3[0],v3[1],v3[2]);
}

AGVector2 HeightMap::truncPos(const AGVector2 &p) const
{
  AGVector2 maxPos(mW+1,mH+1);
  return AGVector2(std::max(0.0f,std::min(maxPos[0],p[0])),
		   std::max(0.0f,std::min(maxPos[1],p[1])));
}


//////////////////////////////////////////////////////////////////////////
// AntMap
//////////////////////////////////////////////////////////////////////////

AntMap *myAntargisMap=0;
AntMap *getMap()
{
  return myAntargisMap;
}

AntMap::AntMap(int w,int h):HeightMap(w,h),mEntQuad(AGRect2(0,0,w,h))
{
  myAntargisMap=this;
  maxID=0;

  if(getScene())
    getScene()->addNode(mTerrainMesh);
}
AntMap::~AntMap()
{
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
  cdebug("INSERTING:"<<e);
  mEntities.push_back(e);
  //  mEntList.push_back(e);
  mEntityMap[e->getID()]=e;
  mEntQuad.insert(e);
  entsChanged();
}

void AntMap::removeEntity(AntEntity *p)
{
  mToDel.push_back(p);
}


void AntMap::move(float pTime)
{
  // first remove entities, which shall be deleted
  std::list<AntEntity*>::iterator d=mToDel.begin();
  for(;d!=mToDel.end();d++)
    {
      EntityList::iterator i=std::find(mEntities.begin(),mEntities.end(),*d);
      if(i!=mEntities.end())
        mEntities.erase(i);
      mEntityMap.erase((*d)->getID());
      mEntQuad.erase(*i);
    }
  if(mToDel.size())
    entsChanged();

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
  return mEntQuad.get(r);
}
AntMap::EntityList AntMap::getAllEntities()
{
  return mEntities;
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
      cdebug("DIST:"<<j->first);
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
  getScene()->clear();
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
  getScene()->clear();
  CTRACE;
  //  mPlayers.clear();
  mEntities.clear();
  mEntQuad.clear();
  mEntityMap.clear();
  getScene()->addNode(mTerrainMesh);

}

void AntMap::entsChanged()
{
  if(mScene)
    {
      for(EntityList::iterator i=mEntities.begin();i!=mEntities.end();i++)
	{
	  Mesh *m=(*i)->getMesh();
	  if(m)
	    mScene->addNode(m);
	}
    }
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
    if((*i)->getMesh()==&pMesh)
      return *i;
  return 0;
}







bool markingFinished=true;

void AntMap_markfunc(void *ptr)
{
  cdebug("TRACE:"<<ptr);
  if(!ptr)
    return;
  if(!markingFinished)
    return;
#ifdef USE_RUBY
  markingFinished=false;
  AntEntity *cppAnimal;
  VALUE   rubyAnimal;
  AntMap *zoo;

  //  TRACE;
  //  cdebug(ptr<<endl);
  zoo = static_cast<AntMap*>(ptr);

  //  zoo->mGCcalls++;

  AntMap::EntityList::iterator i=zoo->mEntities.begin();

  for(;i!=zoo->mEntities.end();i++)
    {
      //      cdebug("children:"<<*i);
      cppAnimal = *i;//zoo->getAnimal(i);
      //      assert(!cppAnimal->mDeleted);
      if(cppAnimal->mRubyObject)
        {
          rubyAnimal = cppAnimal->mRUBY;//SWIG_RubyInstanceFor(cppAnimal);
          rb_gc_mark(rubyAnimal);
          //      cdebug("mark:");//<<cppAnimal->getName());
        }
      //      AntEntity_markfunc(*i);
    }
  markingFinished=true;
#endif
}


