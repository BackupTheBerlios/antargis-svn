#include "minimap.h"
#include "map.h"

MiniMap::MiniMap(AGWidget *pParent,const AGRect &r,AntargisMap *pMap):
  AGWidget(pParent,r),mMap(pMap),mSurface(r.w,r.h)
{
}

void MiniMap::update()
{
  mustUpdate=false;
  
}
    
void MiniMap::draw(const AGRect &r)
{
  if(mustUpdate)
    mTexture=AGTexture(mSurface);
  mustUpdate=false;
  getScreen().blit(mTexture,r.project(getRect()));
}
