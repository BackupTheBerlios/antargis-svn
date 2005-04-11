/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * view.cpp
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

#include "view.h"
#include "tree.h"
#include <ag_color.h>
#include <ag_button.h>
#include "tree.h"

#include <ag_layout.h>
#include <ag_tools.h>
#include <ag_layoutfactory.h>


/***********************************************************
 * globals
 ***********************************************************/

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

/***********************************************************
 * IsoView
 ***********************************************************/


IsoView::IsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map):
    AntargisView(parent,r,p,false),mMap(map),mRain(r.w,r.h,0),shallUpdate(false),maxPos(0,0,0)
{
  cdebug("IsoView-Rect:"<<r);
  inited=false;
  initTileCache();
}
void IsoView::update()
{
  clear();
  mEntities.clear();
  mEntitiesInv.clear();
  mSelected.clear();
  mTiles.clear();
  init();
}


void IsoView::init()
{
  // screen size
  int mw=width()/64+2;
  int mh=height()/16+4;

  // 800x600 0<=x<13   0<=y<40
  // 1400x1024 0<=x<23   0<=y<74

  int sx=0;
  int sy=0;

  sx=(int)(mPos.x/TILE_WIDTH);
  if(sx<0)
    sx=0;
  sy=(int)(mPos.y/(TILE_WIDTH/4));
  if(sy<0)
    sy=0;


  for(int y=sy;y<mh+sy;y++) // 40
    {
      for(int x=sx;x<mw+sx;x++) // 13  for 800x600
        {
          int mx=x*(POINTS_PER_TILE);
          int my=y*(POINTS_PER_TILE/2);
          if(y&1)
            mx+=(POINTS_PER_TILE/2);


          AVItem *i;
          // insert water tile
          i=getWaterTile();
          i->setPosition(Pos3D(mx*TILE_WIDTH/POINTS_PER_TILE,0,my*TILE_WIDTH/POINTS_PER_TILE));
          i->setVirtualY(-42);
          insert(i);


          // now insert terrain (that it's after water)
          //          cdebug("mx:"<<mx<<"//"<<my);
          // insert terrain tile
          SplineMapD h=mMap->getPatchH(mx,my);          // insert water tile
          i=getWaterTile();
          i->setPosition(Pos3D(mx*TILE_WIDTH/POINTS_PER_TILE,0,my*TILE_WIDTH/POINTS_PER_TILE));
          i->setVirtualY(-42);
          insert(i);

          SplineMapD g=mMap->getPatchG(mx,my);

          IVTile tile;
          tile.x=mx;
          tile.y=my;
          i=mTileCache[tile]; // get AVItem from cache
          if(i)
            {
              i->setVirtualY(-32);
              insert(i);
              mTiles[i]=tile;
            }


        }
    }

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


}

void IsoView::initTileCache()
{
  maxPos.x=(mMap->width()/2-4)*TILE_WIDTH-width();
  maxPos.y=(mMap->height()-8)*TILE_WIDTH/4-height();


  // FIXME: this should be called only once and updates done incrementally
  // first clear old tile cache
  std::map<IVTile,AVItem*>::iterator k=mTileCache.begin();
  for(;k!=mTileCache.end();k++)
    delete k->second;
  mTileCache.clear();


  int mw,mh;

  // goes to max of map
  mw=mMap->width()/2-3;
  mh=mMap->height()-6;
  for(int y=0;y<mh;y++) // 40
    {
      for(int x=0;x<mw;x++) // 13  for 800x600
        {
          int mx=x*(POINTS_PER_TILE);
          int my=y*(POINTS_PER_TILE/2);
          if(y&1)
            mx+=(POINTS_PER_TILE/2);

          //          cdebug("mx:"<<mx<<"//"<<my);

          SplineMapD h=mMap->getPatchH(mx,my);
          SplineMapD g=mMap->getPatchG(mx,my);

          VoxelImage *i=getSurface(h,g);
          i->setPosition(Pos3D(mx*TILE_WIDTH/POINTS_PER_TILE,0,my*TILE_WIDTH/POINTS_PER_TILE));

          IVTile tile;
          tile.x=mx;
          tile.y=my;
          mTileCache[tile]=i;
        }
    }

}

VoxelImage *IsoView::getTree()
{
  std::string name="tree";

  if(!fileExists(TILEDIR+"tree.png"))
    {
      VoxelImage *i=makeTree();
      i->save(name);
      return i;
    }
  VoxelImage *i=new VoxelImage(name);
      return i;
}

VoxelImage *IsoView::getWaterTile()
{
  if(!fileExists(TILEDIR+"water.png"))
    {
      VoxelImage *i=makeWaterTile();
      i->save("water");
      delete i;
    }
  VoxelImage *i=new VoxelImage("water");
  return i;
}

VoxelImage *IsoView::getSurface(const SplineMapD &h,const SplineMapD &g)
{
  std::string name=h.toString()+g.toString(); // FIXME: this is too slow
  //return new VoxelImage("man1dl");

  if(!fileExists(TILEDIR+name+".png"))
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

// p is local window-coordinate
std::list<AntEntity *> IsoView::getEntity(const AGPoint &pp)
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

IsoView::IVTile IsoView::getTile(const AGPoint &pp)
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

void IsoView::draw(const AGRect &r)
{
  if(!inited)
    {
      init();
      inited=true;
      shallUpdate=false;
    }
  if(shallUpdate)
    {
      update();
      shallUpdate=false;
    }

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

  std::map<AVItem*,AntEntity*>::iterator k=mEntities.end();// FIXME: don't draw anything ATM begin();
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

  // overlay rain
  mRain.draw(r);
}

void IsoView::updatePositions()
{
  std::list<AntEntity*> ents=mMap->getEntities(AntRect(0,0,1000,1000)); // FIXME: use reasonable rect

  std::string tName;

  std::list<AntEntity*>::iterator i=ents.begin();
  for(;i!=ents.end();i++)
    {
      AVItem *image=mEntitiesInv[*i];
      image->setPosition((*i)->getPos3D());

      VoxelImage *vi=dynamic_cast<VoxelImage*>(image);
      if(vi)
        {
          tName=(*i)->getTexture();
          if(tName.length())
            vi->setTexture(tName);
        }
    }
}


AGColor IsoView::getSelectColor()
{
  float f=sin(mTime*8);
  Uint8 r=0x44+(Uint8)(0x22*f);
  return AGColor(0,0,0xAA,r);
}

Pos2D IsoView::getTilePos(const IVTile &t)
{
  int mx=t.x;
  int my=t.y;

  return Pos2D(mx*TILE_WIDTH/POINTS_PER_TILE,my*TILE_WIDTH/POINTS_PER_TILE);
}

void IsoView::checkView()
{
  // AntargisView::checkView // FIXME: delete objects, which are not in range
  clear(); // FIXME: delete this


}



/***********************************************************
 * CompleteIsoView
 ***********************************************************/

CompleteIsoView::CompleteIsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map):IsoView(parent,r,p,map)
{}

AntHero *CompleteIsoView::getHero(std::list<AntEntity*> &es)
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

bool CompleteIsoView::eventDragBy(const AGEvent *event,const AGPoint &pDiff)
{
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(event);
  if(e)
    {
      if(getButtonState()==SDL_BUTTON(SDL_BUTTON_RIGHT))
        {
          mPos=Pos3D(mPos.x-pDiff.x,mPos.y+pDiff.y,mPos.z);
          if(mPos.x<0)
            mPos.x=0;
          if(mPos.y<0)
            mPos.y=0;

          if(mPos.x>maxPos.x)
            mPos.x=maxPos.x;
          if(mPos.y>maxPos.y)
            mPos.y=maxPos.y;
          //update();//checkView();
          shallUpdate=true; // delay update til drawing - as this may be called several times a frame
          return true;
        }
    }
  return false;
}

bool CompleteIsoView::eventMouseClick(const AGEvent *m)
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

bool CompleteIsoView::isMyHero(AntHero *h)
{
  if(h)
    if(h->getTypeID()==0)
      return true;
  return false;
}

/***********************************************************
 * EditIsoView
 ***********************************************************/

EditIsoView::EditIsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map):
    CompleteIsoView(parent,r,p,map),sigMapEdited(this,"sigMapEdited")
{
  mOldPoint=0;
  mShowPoints=true;
  mEditing=false;
  AGButton *b;

  char *editNames[]={"edit1.png","edit2.png","edit3.png","edit4.png","edit5.png","edit10.png","edit15.png",""};
  int editSizes[]={1,2,3,4,5,10,15};

  editSize=1;

  // add edit buttons at the bottom
  AGLayout *layout;
  addChild(mLayout=layout=new AGLayout(this,getFile("edit_layout.xml")));
  layout->hide();
  for(int i=0;i<10 && std::string(editNames[i]).length();i++)
    layout->getChild(std::string("edit")+toString(editSizes[i]))->sigClick.connect(slot(this,&EditIsoView::selectSize));
  layout->getChild("grass")->sigClick.connect(slot(this,&EditIsoView::setAll));
  layout->getChild("water")->sigClick.connect(slot(this,&EditIsoView::setAll));
  layout->getChild("rubber")->sigClick.connect(slot(this,&EditIsoView::setRubber));
  layout->getChild("tree")->sigClick.connect(slot(this,&EditIsoView::addEntity));
  layout->getChild("stones")->sigClick.connect(slot(this,&EditIsoView::addEntity));
  layout->getChild("pins")->sigClick.connect(slot(this,&EditIsoView::togglePoints));
  
}
bool EditIsoView::togglePoints(const char *name,const AGEvent *e,AGMessageObject *pCaller)
{
  toggleShowPoints();
  return true;
}


bool EditIsoView::setRubber(const char *,const AGEvent *,AGMessageObject *pCaller)
{
  mAddEntity="rubber";
  return true;
}

bool EditIsoView::addEntity(const char *,const AGEvent *,AGMessageObject *pCaller)
{
  AGWidget *b=dynamic_cast<AGWidget*>(pCaller);
  if(b)
  {
    std::string n=b->getName();
    mAddEntity=n;
    cdebug(mAddEntity);
  }
  return true;
}
bool EditIsoView::selectSize(const char *,const AGEvent *,AGMessageObject *pCaller)
{
  CTRACE;
  cdebug(dynamic_cast<AGWidget*>(pCaller)->getName());
  editSize=toInt(dynamic_cast<AGWidget*>(pCaller)->getName().substr(4,std::string::npos));//mEditButtons[pCaller];
  cdebug(editSize);
  mAddEntity="";
  return true;
}
bool EditIsoView::setAll(const char *,const AGEvent *,AGMessageObject *pCaller)
{
  AGWidget *w=dynamic_cast<AGWidget*>(pCaller);
  if(w->getName()=="water")
    getMap()->setAllWater();
  else if(w->getName()=="grass")
    getMap()->setAllLand();
  completeUpdate();
  sigMapEdited(0);
  return true;
}


void EditIsoView::toggleEdit()
{
  mEditing=!mEditing;
  update();
  if(mEditing)
  {
    mLayout->show();
    /*std::list<AGWidget*>::iterator i=mEditWidgets.begin();
    for(;i!=mEditWidgets.end();i++)
      (*i)->show();*/
  }
  else
  {
    mLayout->hide();
/*    std::list<AGWidget*>::iterator i=mEditWidgets.begin();
    for(;i!=mEditWidgets.end();i++)
      (*i)->hide();*/
  }
}

// moving about
bool EditIsoView::eventDragBy(const AGEvent *event,const AGPoint &pDiff)
{
  if(!mEditing)
    return CompleteIsoView::eventDragBy(event,pDiff);
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(event);
  if(e)
    {
      if(getButtonState()==SDL_BUTTON(SDL_BUTTON_MIDDLE))
        {
          mPos=Pos3D(mPos.x-pDiff.x,mPos.y+pDiff.y,mPos.z);
          shallUpdate=true; // delay update til drawing - as this may be called several times a frame
          return true;
        }
    }
  return false;
}
bool EditIsoView::eventMouseClick(const AGEvent *m)
{
  if(!mEditing)
    return CompleteIsoView::eventMouseClick(m);
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      if(getScreenRect().contains(e->getMousePosition()))
        {
          if(e->getButton()==SDL_BUTTON_LEFT||e->getButton()==SDL_BUTTON_RIGHT)
            {
              if(mOldPoint)
              {
                Pos3D p=mOldPoint->getPosition();
                if(mAddEntity=="")
                  editAt(p,e->getButton()==SDL_BUTTON_LEFT);
                else if(mAddEntity=="rubber")
                {
                  // getEntity and delete
                  std::list<AntEntity *> es=getEntity(e->getMousePosition());
                  if(es.size())
                  {
                    for(std::list<AntEntity*>::iterator i=es.begin();i!=es.end();i++)
                      getMap()->removeEntity(*i);
                  }
                }
                else if(mAddEntity=="tree")
                  getMap()->insertEntity(new AntTree(Pos2D(p.x,p.z),rand()%11));
                else if(mAddEntity=="stones")
                  getMap()->insertEntity(new AntDeco(Pos2D(p.x,p.z),rand()%2));
                  
              }

              update();
              return true;
            }
        }
    }
  return false;
}

AVItem *EditIsoView::getClosest(const AGPoint &p) const
  {
    std::list<AVItem*>::const_iterator i=mPoints.begin();
    AVItem *fi=0;
    float distance=0;
    for(;i!=mPoints.end();i++)
      {
        AGRect r=getRect(*i);
        AGPoint diff=r.getPosition()-p;
        float md=sqrt((float)(diff.x*diff.x+diff.y*diff.y));
        if(fi==0 || md<distance)
          {
            distance=md;
            fi=*i;
          }
      }
    return fi;
  }

void EditIsoView::toggleShowPoints()
{
  mShowPoints=!mShowPoints;
  update();
}

bool EditIsoView::eventMouseMotion(const AGEvent *m)
{
  if(!mEditing)
    CompleteIsoView::eventMouseMotion(m);
  const AGSDLEvent *e=reinterpret_cast<const AGSDLEvent*>(m);
  if(e)
    {
      if(getScreenRect().contains(e->getMousePosition()))
        {
          AVItem *closest=getClosest(e->getMousePosition());
          if(closest)
            {
              VoxelImage *i=dynamic_cast<VoxelImage*>(closest);
              if(mOldPoint)
                mOldPoint->setTexture("white_pin");
              mOldPoint=i;
              if(i)
                i->setTexture("blue_pin");
            }

        }
    }
  return IsoView::eventMouseMotion(m);
}

void EditIsoView::editAt(const Pos3D &p,bool dir)
{
  int x=(int)(2*p.x/TILE_WIDTH+2);
  int z=(int)(2*p.z/TILE_WIDTH+3);
  if(dir)
    getMap()->addFlat(x,z,30,editSize);
  else
    getMap()->addFlat(x,z,-30,editSize);

  completeUpdate();
  sigMapEdited(0);
}
void EditIsoView::init()
{
  mOldPoint=0;
  mPoints.clear();
  if(!mEditing)
    {
      CompleteIsoView::init();
      return;
    }
  //CTRACE;
  CompleteIsoView::init();


  if(!mShowPoints)
    return;
  // add pins

  int mw=width()/64+2;
  int mh=height()/16+4;


  mw*=2;
  mh*=2;

  int sx=0;
  int sy=0;

  sx=(int)(mPos.x/(TILE_WIDTH/2));
  if(sx<0)
    sx=0;

  sy=(int)(mPos.y/(TILE_WIDTH/4));
  if(sy<0)
    sy=0;

  for(int y=-1+sy;y<mh+sy;y++)
    {
      for(int x=-1+sx;x<mw+sx;x++)
        {
          int mx=x;
          int my=y;

          mx*=TILE_WIDTH/POINTS_PER_TILE;
          my*=TILE_WIDTH/POINTS_PER_TILE;

          //mx-=TILE_WIDTH/2;
          //my-=TILE_WIDTH/2;

          Pos3D mp(mx,getMap()->getHeight(Pos2D(mx,my)),my); // start one into zeroness
          VoxelImage *i=new VoxelImage("white_pin");
          i->setPosition(mp);
          i->setVirtualY(150);

          insert(i);
          mPoints.push_back(i);
        }
    }
}



// AntView
class AGAntViewLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(AntView,"antView")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    EditIsoView *w=new EditIsoView(pParent,pRect,Pos3D(0,0,0),getMap());
    return w;
  }
};
IMPLEMENT_COMPONENT_FACTORY(AntView);

