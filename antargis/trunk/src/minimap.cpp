#include <ag_color.h>
#include "minimap.h"
#include "map.h"

#define MINIMAP_GREEN 0xAA

MiniMap::MiniMap(AGWidget *pParent,const AGRect &r,AntargisMap *pMap):
  AGWidget(pParent,r),mMap(pMap),mSurface(r.w,r.h)
{
  mustUpdate=true;
  update();
}

void MiniMap::update()
{
  mustUpdate=true;
  AGSurfacePainter p(mSurface);
  float zoom=2;
  float mx,my;
  
  
  for(int x=0;x<width();x++)
  {
    for(int y=0;y<height();y++)
    {
      mx=x*TILE_SIZE/zoom;
      my=y*TILE_SIZE/zoom;
      if(mx<mMap->width()*TILE_WIDTH && my<mMap->height()*TILE_WIDTH)
      {
      Pos3D normal=mMap->getNormal(mx,my);
      if(!(normal==Pos3D(0,1,0)))
      {
//        cdebug("normaL:"<<normal);
//        cdebug(mMap->getHeight(Pos2D(mx,my)));
      }
      float l=normal*Pos3D(1,1,-1).normalized();
      l+=3;
      l*=0.25;
//              cdebug(mx<<","<<my<<","<<mMap->getHeight(Pos2D(mx,my)));
      if(mMap->getHeight(Pos2D(mx,my))<10)
        p.putPixel(x,height()-1-y,AGColor(0,0,(int)(0xFF*l)));
      else
        p.putPixel(x,height()-1-y,AGColor(0,(int)(MINIMAP_GREEN*l),0));
      }
    }
  }
}
    
void MiniMap::draw(const AGRect &r)
{
  if(mustUpdate)
    mTexture=AGTexture(mSurface);
  mustUpdate=false;
  getScreen().blit(mTexture,r.project(getRect()));
}
