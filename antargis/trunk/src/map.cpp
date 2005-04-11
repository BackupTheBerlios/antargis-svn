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
#include "fs.h"
#include<cstdlib>

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
    mEntities(Rect2D(0,0,w*TILE_SIZE,h*TILE_SIZE)),mW(w),mH(h)
{
  myAntargisMap=this;
  paused=false;
  maxID=0;
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
  if(paused)
    return;
  // first move computer-players (they decide what to do)

  std::set
    <AntPlayer*>::iterator j=mPlayers.begin();
  for(;j!=mPlayers.end();j++)
    (*j)->move(pTime);


  // now move all entities

  std::list<AntEntity*>::iterator i=mEntList.begin();

  for(;i!=mEntList.end();i++)
    (*i)->move(pTime);
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
       else if(i->getName()=="antTree")
        e=new AntTree;
       else if(i->getName()=="antDeco")
        e=new AntDeco;
       else if(i->getName()=="antHero")
        e=new AntHero;
       else if(i->getName()=="antMan")
        e=new AntMan;
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
}


/*****************************************************************
* AntEntity
*****************************************************************/

AntEntity::AntEntity(const Pos3D &p):mPos(p),mJob(0),mJobFinished(false),mEnergy(1.0),mHealSpeed(0.3),onGround(false),mCondition(1.0),mConditionFall(0.4),mConditionHeal(0.05)
{
  mDirNum=1;
  mID=getMap()->getNewID();
}
AntEntity::AntEntity(const Pos2D &p):mPos(getMap()->getPos3D(p)),mJob(0),mJobFinished(false),mEnergy(1.0),mHealSpeed(0.3),onGround(true),mCondition(1.0),mConditionFall(0.4),mConditionHeal(0.05)
{
  mDirNum=1;
  mID=getMap()->getNewID();
}

AntEntity::AntEntity():mPos(0,0,0),mJob(0),mJobFinished(false),mEnergy(1.0),mHealSpeed(0.0),onGround(false),mCondition(1.0),mConditionFall(0.4),mConditionHeal(0.05)
{
  mDirNum=1;
  mID=getMap()->getNewID();
}

void AntEntity::saveXML(xmlpp::Node &node) const
  {
    myxmlpp::Node &child=node.newChild("position");
    mPos.saveXML(child);
    node.set("energy",toString(mEnergy));
    node.set("healSpeed",toString(mHealSpeed));
    node.set("onGround",toString(onGround));
    node.set("entityID",toString(getID()));
  }
void AntEntity::loadXML(const xmlpp::Node &node)
{
  mEnergy=toFloat(node.get("energy"));
  mHealSpeed=toFloat(node.get("healSpeed"));
  onGround=toBool(node.get("onGround"));
  assert(onGround);
  xmlpp::Node::const_iterator i=node.begin();
  for(;i!=node.end();i++)
    mPos.loadXML(*i);
  mID=toInt(node.get("entityID"));
}

Pos3D AntEntity::getPos3D() const
  {
    return mPos;
  }
Pos2D AntEntity::getPos2D() const
  {
    return Pos2D(mPos.x,mPos.z);
  }

void AntEntity::setPos2D(const Pos2D &p)
{
  mPos=Pos3D(p.x,getMap()->getHeight(p),p.y);
}

void AntEntity::setJob(Job *pJob)
{
  if(mJob)
    {
      if((*mJob)<=(*pJob))
        delete mJob;
      else
        {
          delete pJob;
          return;
        }
    }
  mJob=0;
  if(mEnergy>0.0)
    mJob=pJob;
  else
    delete pJob;
}


/** do anything in given time frame */
void AntEntity::move(float pTime)
{
  if(mJobFinished || mEnergy==0.0)
    {
      delete mJob;
      mJob=0;
      mJobFinished=false;
    }
  else if(mEnergy>0.0)
    {
      mEnergy+=pTime*getHealSpeed();
      if(mEnergy>1.0)
        mEnergy=1.0;
    }
  if(mJob)
    mJob->move(this,pTime);
    
  mCondition+=mConditionHeal*pTime;
}

Rect2D AntEntity::getRect() const
  {
    return Rect2D((int)mPos.x,(int)mPos.y,32,32);
  }

void AntEntity::jobFinished()
{
  if(mJob)
    mJobFinished=true;
}

void AntEntity::mapChanged()
{
  if(onGround)
    mPos=getMap()->getPos3D(Pos2D(mPos.x,mPos.z));
}


/************************************************************************
* MoveJob
************************************************************************/

MoveJob::MoveJob(int p,const Pos2D &pTarget,int pnear,bool pRun):Job(p),mTarget(getMap()->truncPos(pTarget)),near(pnear),mRun(pRun)
    {
      speed=70; // pixels per second
      runSpeed=100;
    }

// Jobs
void MoveJob::move(AntEntity *e,float ptime)
{
  float aspeed;
  
  if(mRun && e->getCondition()>0.0)
  {
    // decrease condition and if condition is zero - switch of running
    float newtime=e->decCondition(ptime);
    moveBy(e,ptime-newtime,runSpeed);// take same runSpeed always
    
    ptime=newtime;
  }
  aspeed=0.5*speed+0.5*e->getEnergy()*speed;
  moveBy(e,ptime,aspeed); // use rest of time
  
}

Pos2D MoveJob::getDirection(const AntEntity *e) const
{
  return (mTarget-e->getPos2D()).normalized();
}


void MoveJob::moveBy(AntEntity *e,float ptime,float aspeed)
{
  Pos2D diff=e->getPos2D()-mTarget;
  float norm=diff.norm();
  if(norm-near>ptime*aspeed)
    {
      diff=diff.normalized();
      e->setDirection(diff*(-1));
      e->setPos2D(e->getPos2D()-diff*ptime*aspeed);
    }
  else
    {
      //   e->setPos2D(mTarget);
      e->jobFinished();
    }
}

/************************************************************************
* FightJob
************************************************************************/


// FightJobs
void FightJob::move(AntEntity *e,float ptime)
{
  if(mTarget->getEnergy()==0.0)
    e->jobFinished();
  // if target is too far away run there, otherwise fight
  Pos2D diff=e->getPos2D()-mTarget->getPos2D();
  float norm=diff.norm();
  if(norm-fightDistance>ptime*speed)
    {
      diff=diff.normalized();
      e->setPos2D(e->getPos2D()-diff*ptime*speed);
    }
  else
    {
      // fight
      mTarget->decEnergy(ptime*strength);
      mTarget->gotFight(e);
    }
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
* AntHero
************************************************************************/
AntHero::AntHero(const Pos2D &p,int pTypeID,const std::string &pName):AntEntity(p),typeID(pTypeID),mName(pName)
{}
AntHero::~AntHero()
{
  std::set
    <AntMan*>::iterator i=mMen.begin();
  for(;i!=mMen.end();i++)
    (*i)->discard(this);

  getMap()->killHero(this);
}
VoxelImage *AntHero::getSurface() const
  {
    std::ostringstream os;
    if(typeID==0)
      os<<"hero1dl";
    else
      os<<"hero2dl";
    VoxelImage *im=new VoxelImage(os.str());//imageCache()->getImage(os.str());
    im->setPosition(mPos);
    //im->setCenter(Pos2D(100,150)+Pos2D(0,64));
    im->setVirtualY(40);
    im->setName(mName);
    return im;
  }

int AntHero::getTypeID() const
  {
    return typeID;
  }


void AntHero::fightHero(AntHero *h)
{
  CTRACE;
  // myselfs attacks other hero
  setJob(new FightJob(0,h));

  // get other's men and other hero and put them all into a vector
  std::set
    <AntMan*> otherMen=h->mMen;
  std::vector<AntEntity*> otherList;

  std::set
    <AntMan*>::iterator i=otherMen.begin();
  for(;i!=otherMen.end();i++)
    otherList.push_back(*i);
  otherList.push_back(h); // push other hero

  std::random_shuffle(otherList.begin(),otherList.end()); // shuffle all

  std::vector<AntEntity*>::iterator k=otherList.begin();
  // assign from front to back all the entities for fighting
  for(i=mMen.begin();i!=mMen.end();i++)
    {
      (*i)->setJob(new FightJob(0,*k));

      k++;
      if(k==otherList.end())
        k=otherList.begin(); // simply restart, if end is hit
    }


}

void AntHero::goTo(int prio,const Pos2D &pos)
{
  std::set
    <AntMan*>::iterator i;
  for(i=mMen.begin();i!=mMen.end();i++)
    (*i)->jobFinished();
  setJob(new MoveJob(prio,pos));
}


void AntHero::signUp(AntMan *man)
{
  mMen.insert(man);
}


void AntHero::discard(AntMan *man)
{
  mMen.erase(man);
}

float AntHero::calcTroupStrength() const
  {
    float s=getEnergy();
    std::set
      <AntMan*>::iterator i;
    for(i=mMen.begin();i!=mMen.end();i++)
      s+=(*i)->getEnergy();
    return s;
  }

AntHero *AntHero::fights()
{
  if(mJob)
    {
      FightJob *f=dynamic_cast<FightJob*>(mJob);
      if(f)
        {
          return dynamic_cast<AntHero*>(f->getTarget());
        }
    }
  return 0;
}

void AntHero::saveXML(xmlpp::Node &node) const
{
  AntEntity::saveXML(node);
  node.set("typeID",toString(typeID));
  node.set("name",mName);
}
void AntHero::loadXML(const xmlpp::Node &node)
{
  AntEntity::loadXML(node);
  typeID=toInt(node.get("typeID"));
  mName=node.get("name");
}


/************************************************************************
* AntMan
************************************************************************/
void AntMan::saveXML(xmlpp::Node &node) const
{
  AntEntity::saveXML(node);
  node.set("typeID",toString(typeID));
  if(mHero)
    node.set("heroID",toString(mHero->getID()));
}
void AntMan::loadXML(const xmlpp::Node &node)
{
  AntEntity::loadXML(node);
  typeID=toInt(node.get("typeID"));
  if(node.get("heroID").length())
    mHeroID=toInt(node.get("heroID"));
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
    child.set("heroID",toString((*i)->getID()));
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
      int id=toInt(i->get("heroID"));
      AntHero *h=dynamic_cast<AntHero*>(getMap()->getEntity(id));
      if(!h)
        cdebug("Hero not found!");
      else
        mHeroes.insert(h);
    }
  }
}

