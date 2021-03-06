#include "map.h"
#include "voxel.h"

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
    mEntities(Rect2D(0,0,w*TILE_SIZE,h*TILE_SIZE))
{
  myAntargisMap=this;
  paused=false;
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

void AntargisMap::insertEntity(AntEntity *e)
{
  mEntities.insert(e);
  mEntList.push_back(e);
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
  
  std::set<AntPlayer*>::iterator j=mPlayers.begin();
  for(;j!=mPlayers.end();j++)
    (*j)->move(pTime);

  
  // now move all entities

  std::list<AntEntity*>::iterator i=mEntList.begin();

  for(;i!=mEntList.end();i++)
    (*i)->move(pTime);
}


void AntargisMap::killHero(AntHero *h)
{
    std::set<AntPlayer*>::iterator i=mPlayers.begin();
    
    for(;i!=mPlayers.end();i++)
      (*i)->removeHero(h);
  
}

void AntargisMap::editHeightAt(int x,int y,int h,int r)
{
  for(int i=-r;i<=r;i++)
    for(int j=-r;j<=r;j++)
      mHeight.edit(x+i,y+j,h*sqrt(i*i+j*j)/r);
}


/*****************************************************************
* AntEntity
*****************************************************************/

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
}

Rect2D AntEntity::getRect() const
  {
    return Rect2D(mPos.x,mPos.y,32,32);
  }

void AntEntity::jobFinished()
{
  if(mJob)
    mJobFinished=true;
}

/************************************************************************
* MoveJob
************************************************************************/

// Jobs
void MoveJob::move(AntEntity *e,float ptime)
{
  Pos2D diff=e->getPos2D()-mTarget;
  float norm=diff.norm();
  if(norm-near>ptime*speed)
    {
      diff=diff.normalized();
      e->setPos2D(e->getPos2D()-diff*ptime*speed);
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
* AntHero
************************************************************************/
AntHero::AntHero(const Pos2D &p,int ID):AntEntity(getMap()->getPos3D(p)),id(ID)
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
    if(id==0)
      os<<"hero1dl";
    else
      os<<"hero2dl";
    VoxelImage *im=new VoxelImage(os.str());//imageCache()->getImage(os.str());
    im->setPosition(mPos);
    //im->setCenter(Pos2D(100,150)+Pos2D(0,64));
    im->setVirtualY(40);
    return im;
  }

int AntHero::getID() const
  {
    return id;
  }


void AntHero::fightHero(AntHero *h)
{
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

  
  
/************************************************************************
* AntMan
************************************************************************/
