/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * minimap.cpp
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

#include <ag_color.h>
#include "minimap.h"
#include "map.h"
#include "entities.h"
//#include "ant_man.h"

#include <ag_layoutfactory.h>

#define MINIMAP_GREEN 0xAA
#define MINIMAP_ALPHA 0xFF

MiniMap::MiniMap(AGWidget *pParent,const AGRect &r,AntargisMap *pMap,const AGRect &pViewRect):
  AGWidget(pParent,r),sigMoveMap(this,"sigMoveMap"),mMap(pMap),mSurface(r.w-16,r.h-16),mViewRect(pViewRect)
{
  mustUpdate=true;
  update();

  mBorder=AGBorder("antButton.button.border.normal");
}

MiniMap::~MiniMap()
{
}

void MiniMap::drawEntities(AGPainter &p)
{
//  CTRACE;
  Pos2D maxPos=mMap->getMaxPos();
  std::list<AntEntity*> l=mMap->getAllEntities();
  std::list<AntEntity*>::iterator i=l.begin();
  for(;i!=l.end();i++)
  {
    AGColor c;
    int x,y;
    Pos2D pos=(*i)->getPos2D();
    bool m=(*i)->getType()=="man";
    bool h=(*i)->getType()=="hero";
    //    AntMan *m=dynamic_cast<AntMan*>(*i);
    //    AntHero *h=dynamic_cast<AntHero*>(*i);
    if(m)
      c=AGColor(0xFF,0,0);
    else if(h)
      c=AGColor(0,0xFF,0);
    else
      c=AGColor(0x77,0x77,0x77);
      
      
    x=(int)(pos.x*mSurface.width()/maxPos.x);
    y=(int)(mSurface.height()-1-pos.y*mSurface.height()/maxPos.y);
    
    p.drawRect(AGRect(x+8,y+8,2,2),c);
    //    getScreen().drawRect(AGRect(p0.x+x,p0.y+y,2,2),c);
  }
}

void MiniMap::update()
{
//  CTRACE;
  mustUpdate=true;
  AGPainter p(mSurface);
  float mx,my;
  
  Pos2D maxPos=mMap->getMaxPos();
  
  for(int x=0;x<mSurface.width();x++)
  {
    for(int y=0;y<mSurface.height();y++)
    {
      mx=float(x)/mSurface.width()*maxPos.x;
      my=float(y)/mSurface.height()*maxPos.y;
      Pos3D normal=mMap->getNormal((int)mx,(int)my);
      float l=normal*Pos3D(1,1,-1).normalized();
      l+=3;
      l*=0.25;
      
      
      if(mMap->getHeight(Pos2D(mx,my))<10)
        p.putPixel(AGPoint(x,mSurface.height()-1-y),AGColor(0,0,(int)(0xFF*l),MINIMAP_ALPHA));
      else
        p.putPixel(AGPoint(x,mSurface.height()-1-y),AGColor(0,(int)(MINIMAP_GREEN*l),0,MINIMAP_ALPHA));
    }
  }
}

void MiniMap::draw(AGPainter &p)//const AGRect &r)
{
  if(mMap->updated())
    mapUpdate();

//  CTRACE;
  AGRect mr=getRect().origin();
  //  mustUpdate=true;
  if(mustUpdate)
    mTexture=getTextureManager()->makeTexture(mSurface);
  mustUpdate=false;
  mBorder.draw(p);
  //  p.blit(mBG,p.getRect());//,mr);
  mr.x+=8;
  mr.y+=8;
  mr.w-=16;
  mr.h-=16;
  
  p.blit(mTexture,mr);//mTexture.getRect());//,getRect());
  //  p.blit(mTexture,mr);

  //  cdebug(mTexture.getRect());

  //  cdebug(mTexture.getRect());
  //  cdebug("Surface:"<<mSurface.getRect());
  //  SDL_SaveBMP(mSurface.surface(),"test.bmp");
  
  
  drawEntities(p);//mr.getPosition());
}



// AntView
class AGAntMiniViewLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(AntMiniView,"antMiniView")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    MiniMap *w=new MiniMap(pParent,pRect,getMap(),AGRect(0,0,10,10));
    return w;
  }
};
IMPLEMENT_COMPONENT_FACTORY(AntMiniView);

