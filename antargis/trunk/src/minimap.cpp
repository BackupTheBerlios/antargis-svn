#include <ag_color.h>
#include "minimap.h"
#include "map.h"

#define MINIMAP_GREEN 0xAA
#define MINIMAP_ALPHA 0xFF

MiniMap::MiniMap(AGWidget *pParent,const AGRect &r,AntargisMap *pMap,const AGRect &pViewRect):
  AGWidget(pParent,r),sigMoveMap(this,"sigMoveMap"),mMap(pMap),mSurface(r.w-16,r.h-16),mViewRect(pViewRect)
{
  mustUpdate=true;
  update();
  mBG=AGTexture(getScreen().loadSurface("data/minimap_bg.png"));
}

void MiniMap::drawEntities(const AGPoint &p0)
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
    AntMan *m=dynamic_cast<AntMan*>(*i);
    AntHero *h=dynamic_cast<AntHero*>(*i);
    if(m)
      c=AGColor(0xFF,0,0);
    else if(h)
      c=AGColor(0,0xFF,0);
    else
      c=AGColor(0x77,0x77,0x77);
      
      
    x=(int)(pos.x*mSurface.width()/maxPos.x);
    y=(int)(mSurface.height()-1-pos.y*mSurface.height()/maxPos.y);
    
    getScreen().drawRect(AGRect(p0.x+x,p0.y+y,2,2),c);
  }
}

void MiniMap::update()
{
//  CTRACE;
  mustUpdate=true;
  AGSurfacePainter p(mSurface);
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
        p.putPixel(x,mSurface.height()-1-y,AGColor(0,0,(int)(0xFF*l),MINIMAP_ALPHA));
      else
        p.putPixel(x,mSurface.height()-1-y,AGColor(0,(int)(MINIMAP_GREEN*l),0,MINIMAP_ALPHA));
    }
  }
}

void MiniMap::draw(const AGRect &r)
{
//  CTRACE;
  AGRect mr=r.project(getRect());
  if(mustUpdate)
    mTexture=AGTexture(mSurface);
  mustUpdate=false;
  getScreen().blit(mBG,mr);
  mr.x+=8;
  mr.y+=8;
  mr.w-=16;
  mr.h-=16;
  
  getScreen().blit(mTexture,mr);
  
  drawEntities(mr.getPosition());
/*
  // draw viewing Rect  
  float sx0=mViewRect.x;
  
  AGRect vr(;
  vr=
  
  getScreen().drawRect(AGRect(*/
}
