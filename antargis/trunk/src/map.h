#ifndef ANT_MAP_H
#define ANT_MAP_H

#include "decast.h"
#include "quadtree.h"
#include "voxel.h"

#include "xml.h"

#include <set>
#include <vector>
#include <algorithm>

class AntEntity;
class AntPlayer;
class AntHero;

class Job
  {
    int priority;
  public:
    Job(int p):priority(p)
    {
    }
    virtual void move(AntEntity *,float ptime)
    {}
    
    bool operator<=(const Job &j) const
    {
      return priority<=j.priority;
    }
  }
;

class MoveJob:public Job
  {
    Pos2D mTarget;
    float speed;
    int near;
  public:
    MoveJob(int p,const Pos2D &pTarget,int pnear=10):Job(p),mTarget(pTarget),near(pnear)
    {
      speed=70; // pixels per second
    }
    void move(AntEntity *e,float ptime);
  };

// FIXME: implement near and far fighting (arrows)

class FightJob:public Job
  {
    AntEntity *mTarget;
    int fightDistance;
    float strength;
    float speed;

  public:
    FightJob(int p,AntEntity *pTarget):Job(p),mTarget(pTarget)
    {
      fightDistance=20; // in pixels
      strength=0.2;   // decrease per second
      speed=70; // see MoveJob
    }
    void move(AntEntity *e,float ptime);
    AntEntity *getTarget()
    {
      return mTarget;
    }
  };

class AntEntity
  {
  protected:
    Pos3D mPos;
    Job *mJob;
    bool mJobFinished;
    float mEnergy;
    float mHealSpeed;
    bool onGround;
    
  public:
    AntEntity(const Pos3D &p);
    AntEntity(const Pos2D &p);
    Pos3D getPos3D() const;
    Pos2D getPos2D() const;
    
    virtual std::string xmlName() const
    {
      return "antEntity";
    }
    
    virtual void saveXML(xmlpp::Node &node) const;
    virtual void loadXML(const xmlpp::Node &node);

    void setJob(Job *pJob);

    void setPos2D(const Pos2D &p);
    
    void mapChanged();
    virtual VoxelImage*getSurface() const=0;

    /** do anything in given time frame */
    virtual void move(float pTime);

    virtual Rect2D getRect() const;

    void jobFinished();
    
    bool hasJob() const
    {
      return mJob;
    }
    
    virtual float getHealSpeed() const
    {
      return mHealSpeed;
    }

    void decEnergy(float amount)
    {
      mEnergy-=amount;
      if(mEnergy<0.0)
      {
        mEnergy=0.0;
        jobFinished();
      }
    }

    float getEnergy() const
      {
        return mEnergy;
      }

    virtual void gotFight(AntEntity *e)
    {}

    virtual AntHero *getHero()
    {
      return 0;
    }

  };


class HeightMap
{
  SplineMapD mHeight,mGrass;
  
  public:
  HeightMap(int w,int h);
  
  
};


class AntargisMap
  {
    SplineMapD mHeight,mGrass;
    QuadTree<AntEntity> mEntities;
    std::list<AntEntity*> mEntList;
    std::set
      <AntPlayer*> mPlayers;
      
    bool paused;
    int mW,mH;
      
  public:
    AntargisMap(int w,int h);

    float getHeight(const Pos2D &p) const;
    float getGHeight(const Pos2D &p) const;

    SplineMapD getPatchH(int x,int y) const;
    SplineMapD getPatchG(int x,int y) const;

    void insertEntity(AntEntity *e);
    
    // x,y = position
    // h = amount
    // r = radius
    void editHeightAt(int x,int y,int h,int r);

    std::list<AntEntity*> getEntities(const AntRect&r);

    Pos3D getPos3D(const Pos2D &p) const;

    void move(float pTime);

    void addPlayer(AntPlayer *p)
    {
      mPlayers.insert(p);
    }
    void removePlayer(AntPlayer *p)
    {
      mPlayers.erase(p);
    }

    void killHero(AntHero *h);
    
    void pause()
    {
      paused=true;
    }
    void unpause()
    {
      paused=false;
    }
    
    void saveMap(const std::string &pFilename);
    void loadMap(const std::string &pFilename);
    
    void saveXML(xmlpp::Node &node) const;
    void loadXML(const xmlpp::Node &node);
    
  };

AntargisMap *getMap();


class AntTree:public AntEntity
  {
    int id;
  public:
    AntTree(const Pos2D &p,int ID):AntEntity(p),id(ID)
    {}
    VoxelImage *getSurface() const
      {
        std::ostringstream os;
        if(id==0)
          os<<"tree";//_"<<id;
        else if(id==2)
          os<<"tower1a";//_"<<id;
        else if(id==1)
          os<<"house1b";
        else if(id==3)
          os<<"man1dl";
        VoxelImage *im=new VoxelImage(os.str());//imageCache()->getImage(os.str());
        im->setPosition(mPos);
        if(id==1)
          im->setCenter(Pos2D(100,150)+Pos2D(0,64));
        if(id==2||id==3)
          im->setCenter(Pos2D(100,150)+Pos2D(0,64));
        im->setVirtualY(32);
        return im;
      }
    virtual std::string xmlName() const
    {
      return "antTree";
    }
  };

class AntMan;

class AntHero:public AntEntity
  {
    int id;

    std::set
      <AntMan*> mMen;

  public:
    AntHero(const Pos2D &p,int ID);
    virtual ~AntHero();
    VoxelImage *getSurface() const;
    void signUp(AntMan *man);

    int getID() const;
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
    virtual AntHero *getHero()
    {
      return this;
    }
    virtual float getHealSpeed() const
    {
      if(hasJob())
        return mHealSpeed*0.5;
      else
        return mHealSpeed;
    }
    virtual std::string xmlName() const
    {
      return "antHero";
    }

  };

class AntMan: public AntEntity
  {
    int id;
    AntHero *mHero;
  public:
    AntMan(const Pos2D &p,int ID,AntHero *pHero):AntEntity(p),id(ID),mHero(pHero)
    {
      if(pHero)
        pHero->signUp(this);
    }
    virtual ~AntMan()
    {
      if(mHero)
        mHero->discard(this);
    }
    virtual void move(float pTime)
    {
      if(mHero && !mJob)
        {
          setJob(new MoveJob(0,mHero->getPos2D()));
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
        if(id==0)
          os<<"man1dl";
        else
          os<<"man2dl";
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
        return mHealSpeed*0.5;
      else
        return mHealSpeed;
    }
    virtual std::string xmlName() const
    {
      return "antMan";
    }

  };

// Computer player
class AntPlayer
  {
    std::set
      <AntHero*> mHeroes;
  public:
    AntPlayer()
    {
      getMap()->addPlayer(this);
    }
    ~AntPlayer()
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

    void move(float pTime)
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
              float factor=1.5; // as this hero shouldn't give up to early
              // is fighting - check for stats
              if(h->calcTroupStrength()>(*i)->calcTroupStrength()*factor)
                {
                  // we will lose, so flee
                  flee(*i,h->getPos2D(),100,10);
                }
            }
          else if(!(*i)->hasJob())
            {
              flee(*i,Pos2D(-10+rand()%20,-10+rand()%20),10,0);
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
  };

#endif
