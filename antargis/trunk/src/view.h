#ifndef __ANT_ISOVIEW
#define __ANT_ISOVIEW

#include "tree.h"

#include <math.h>

Uint32 myLastTime=0;
float myTimeDiff=0.0;

void doTick()
{
  Uint32 now=SDL_GetTicks();
  if(!myLastTime)
    myLastTime=now;

  myTimeDiff=(now-myLastTime)*0.001;

  myLastTime=now;
}

float getTimeDiff()
{
  return myTimeDiff;
}

class IsoView:public AntargisView
  {
  public:
    struct IVTile
      {
        int x,y;
      };
  private:

    AntargisMap *mMap;
    std::map<AVItem*,IVTile> mTiles;
    float mTime;

  protected:
    std::map<AVItem*,AntEntity*> mEntities;
    std::map<AntEntity*,AVItem*> mEntitiesInv;
    std::set
      <AVItem*> mSelected;

  public:
    IsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map):
        AntargisView(parent,r,p),mMap(map)
    {
      init();
      cdebug("IsoView-Rect:"<<r);
    }
    void update()
    {
      CTRACE;
      clear();
      mEntities.clear();
      mEntitiesInv.clear();
      mSelected.clear();
      mTiles.clear();
      init();
      
    }
  private:
    void init()
    {
      CTRACE;
      Uint32 t1=SDL_GetTicks();

      int mw=width()/64+2;
      int mh=height()/16+4;

//      mh=mw=6;
      
      
      //      mh=mw=11;

      // 800x600 0<=x<13   0<=y<40
      // 1400x1024 0<=x<23   0<=y<74

      for(int y=0;y<mh;y++) // 40
        {
          for(int x=0;x<mw;x++) // 13  for 800x600
            {
              int mx=x*(POINTS_PER_TILE);
              int my=y*(POINTS_PER_TILE/2);
              if(y&1)
                mx+=(POINTS_PER_TILE/2);

              SplineMapD h=mMap->getPatchH(mx,my);
              SplineMapD g=mMap->getPatchG(mx,my);

              
              VoxelImage *i=getSurface(h,g);
              i->setPosition(Pos3D(mx*TILE_WIDTH/POINTS_PER_TILE,0,my*TILE_WIDTH/POINTS_PER_TILE));
              IVTile tile;
              tile.x=mx;
              tile.y=my;
              mTiles[i]=tile;

              insert(i);

            }
        }
      Uint32 t2=SDL_GetTicks();
      std::cout<<"ALL TIME::"<<t2-t1<<std::endl;


      // insert entities
      std::list<AntEntity*> ents=mMap->getEntities(AntRect(0,0,1000,1000)); // FIXME: use reasonable rect

      std::list<AntEntity*>::iterator i=ents.begin();
      for(;i!=ents.end();i++)
        {
          VoxelImage *image=(*i)->getSurface();
          mEntities[image]=*i;
          mEntitiesInv[*i]=image;
          insert(image);
        }



      t2=SDL_GetTicks();
      std::cout<<"ALL TIME::"<<t2-t1<<std::endl;
    }
    
    VoxelImage *getTree()
    {
      std::string name="tree";

      if(fileExists(name+".png"))
        {
          //ComplexVoxelImage *i=new ComplexVoxelImage(name);
          VoxelImage *i=new VoxelImage(name);
          return i;
        }
      else
        {
          VoxelImage *i=makeTree();

          i->save(name);
          return i;
        }
    }

    VoxelImage *getSurface(const SplineMapD &h,const SplineMapD &g)
    {
      std::string name=h.toString()+g.toString(); // FIXME: this is too slow
      //return new VoxelImage("man1dl");

      if(!fileExists(name+".png"))
      {
          VoxelImage *i=makeTerrainTile(h,g,0,0);

          i->save(name);
          delete i;
      }
      
      
      //cdebug("FileExists:"<<name<<".png");
      //if(fileExists(name+".png"))
        {
          //cdebug("File Exists!!!!!!!!!!!!!!!!!!!!!!!");
          //ComplexVoxelImage *i=new ComplexVoxelImage(name);
          VoxelImage *i=new VoxelImage(name);
          return i;
        }
      /*else
        {
          VoxelImage *i=makeTerrainTile(h,g,0,0);

          i->save(name);
          return i;
        }*/
    }

  public:
    // p is local window-coordinate
    std::list<AntEntity *> getEntity(const AGPoint &pp)
    {

      std::list<AntEntity *> found;
      std::vector<AVItem*>::reverse_iterator i=mItems.rbegin();
      for(;i!=mItems.rend();i++)
        {
          AGRect ar=getRect(*i);
          //Pos2D p((*i)->getCenter());
          //ar=(*i)->getRect(mPos)+AGPoint(-p.x,-p.y+height()+32);
          if(ar.contains(pp))
            {
              std::map<AVItem*,AntEntity*>::iterator k=mEntities.find(*i);
              if(k!=mEntities.end())
                {
                  AGPoint p=pp-ar.getPosition();
                  if(k->first->getAlpha(p)>0)
                    found.push_back(k->second);
                }
            }
        }

      return found;
    }

    IVTile getTile(const AGPoint &pp)
    {

      AntEntity *found=0;
      std::vector<AVItem*>::reverse_iterator i=mItems.rbegin();
      for(;i!=mItems.rend() && !found;i++)
        {
          AGRect ar=getRect(*i);
          //Pos2D p((*i)->getCenter());
          //ar=(*i)->getRect(mPos)+AGPoint(-p.x,-p.y+height()+32);
          if(ar.contains(pp))
            {
              std::map<AVItem*,IVTile>::iterator k=mTiles.find(*i);
              if(k!=mTiles.end())
                {
                  AGPoint p=pp-ar.getPosition();
                  // check if alpha channel is zero
                  if(k->first->getAlpha(p)>0)
                    return k->second;
                }
            }
        }

      return IVTile();
    }

  protected:
    void draw(const AGRect &r)
    {
      updatePositions();

      doTick();
      mTime+=getTimeDiff();

      AntargisView::draw(r);

      // overlay selection-rectangle and energy

      std::set
        <AVItem*>::iterator i=mSelected.begin();
      for(;i!=mSelected.end() ;i++)
        {
          AGRect ar=getRect(*i);
          ar=r.project(ar);
          getScreen().drawRect(ar,getSelectColor());
        }

      std::map<AVItem*,AntEntity*>::iterator k=mEntities.begin();
      for(;k!=mEntities.end();k++)
        {
          AGRect ar=getRect(k->first);
          ar=r.project(ar);
          // draw energy
          ar.y-=10;
          ar.h=6;
          getScreen().drawRect(ar,AGColor(0xFF,0,0)); // first red
          ar.w=(short)(ar.w*k->second->getEnergy());
          getScreen().drawRect(ar,AGColor(0,0xFF,0)); // overpaint with green
        }


    }

    void updatePositions()
    {
      std::list<AntEntity*> ents=mMap->getEntities(AntRect(0,0,1000,1000)); // FIXME: use reasonable rect

      std::list<AntEntity*>::iterator i=ents.begin();
      for(;i!=ents.end();i++)
        {
          AVItem *image=mEntitiesInv[*i];
          image->setPosition((*i)->getPos3D());
        }
    }


    AGColor getSelectColor()
    {
      float f=sin(mTime*8);
      Uint8 r=0x44+(Uint8)(0x22*f);
      return AGColor(0,0,0xAA,r);
    }

    Pos2D getTilePos(const IVTile &t)
    {
      int mx=t.x;
      int my=t.y;

      return Pos2D(mx*TILE_WIDTH/POINTS_PER_TILE,my*TILE_WIDTH/POINTS_PER_TILE);
    }

  };


class CompleteIsoView: public IsoView
  {
  public:
    CompleteIsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map):IsoView(parent,r,p,map)
    {}

    AntHero *getHero(std::list<AntEntity*> &es)
    {
      std::list<AntEntity*>::iterator i=es.begin();
      for(;i!=es.end();i++)
        {
          AntHero *e=dynamic_cast<AntHero*>(*i);
          if(e)
            return e;
        }
      return 0;
    }
    
    virtual bool eventDragBy(const AGEvent *event,const AGPoint &pDiff)
    {
      const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(event);
      if(e)
        {
          if(getButtonState()==SDL_BUTTON(SDL_BUTTON_RIGHT))
          {
            mPos=Pos3D(mPos.x-pDiff.x,mPos.y+pDiff.y,mPos.z);
            return true;
          }
        }
       return false;
    }

    bool eventMouseClick(const AGEvent *m)
    {
      const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
      if(e)
        {
          if(e->getButton()==SDL_BUTTON_RIGHT)
            mSelected.clear();
          else if(getScreenRect().contains(e->getMousePosition()))
            {
              //clicked
              AGPoint p(e->getMousePosition());
              cdebug("clicked on "<<p.x<<","<<p.y);

              std::list<AntEntity *> es=getEntity(p);
              if(es.size())
                {
                  if(mSelected.size())
                    {
                      // already something selected - so fight
                      for(std::set
                            <AVItem*>::iterator k=mSelected.begin();k!=mSelected.end();k++) // loop selected
                          {
                            AVItem *i=*k;

                            AntHero *h=dynamic_cast<AntHero*>(mEntities[i]);
                            if(isMyHero(h))
                              {
                                AntHero *otherHero=getHero(es);
                                if(otherHero)
                                  h->fightHero(otherHero);
                                //h->setJob(new FightJob(*es.begin())); // FIXME: currently fighting first
                              }
                          }

                    }
                  else
                    {
                      std::list<AntEntity*>::iterator i=es.begin();
                      for(;i!=es.end();i++)
                        mSelected.insert(mEntitiesInv[*i]);
                    }

                }
              else
                {

                  IVTile t=getTile(p);
                  cdebug(t.x<<","<<t.y);

                  if(mSelected.size() && es.size()==0)
                    {
                      for(std::set
                            <AVItem*>::iterator k=mSelected.begin();k!=mSelected.end();k++)
                          {
                            AVItem *i=*k;

                            AntHero *h=dynamic_cast<AntHero*>(mEntities[i]);
                            if(isMyHero(h))
                              {
                                CTRACE;
                                h->goTo(1,getTilePos(t));
                                //h->setJob(new MoveJob(getTilePos(t)));
                              }
                          }
                    }
                }
            }
        }
      return false;

    }

    bool isMyHero(AntHero *h)
    {
      if(h)
        if(h->getTypeID()==0)
          return true;
      return false;
    }

  };

class EditIsoView: public IsoView
  {
  public:
    EditIsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map):IsoView(parent,r,p,map)
    {}
    
    // moving about
    virtual bool eventDragBy(const AGEvent *event,const AGPoint &pDiff)
    {
      const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(event);
      if(e)
        {
          if(getButtonState()==SDL_BUTTON(SDL_BUTTON_RIGHT))
          {
            mPos=Pos3D(mPos.x-pDiff.x,mPos.y+pDiff.y,mPos.z);
            return true;
          }
        }
       return false;
    }
    bool eventMouseClick(const AGEvent *m)
    {
      const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
      if(e)
        {
          if(getScreenRect().contains(e->getMousePosition()))
            {
              if(e->getButton()==SDL_BUTTON_LEFT)
              {
                editAt(e->getMousePosition());
                update();
                return true;
              }
            }
        }
      return false;
    }
    void editAt(const AGPoint &p)
    {
       IVTile t=getTile(p);
       Pos2D p2=getTilePos(t)/32;
       p2.x+=2;
       cdebug("p2:"<<p2.x<<","<<p2.y);
       
       getMap()->editHeightAt((int)p2.x,(int)p2.y,30,1);
      
    }
  };
  
#endif
