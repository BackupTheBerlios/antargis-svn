#include "ag_image.h"
#include "ag_debug.h"

AGImage::AGImage(AGWidget *pParent,const AGPoint &p,AGSurface pSurface,bool pTile,const AGRect &pRect):
  AGWidget(pParent,pSurface.getRect()+p),
  mTexture(pSurface),mTile(pTile)
{
  CTRACE;
  if(pRect!=pSurface.getRect() && pRect.w!=0 && pRect.h!=0)
    {
      mSrcRect=pRect;
      /*
	mTexture=pSurface.getSubSurface(pRect);*/
  //  if(mSrcRect.w==0 || mSrcRect.h==0)
  //    mSrcRect=mTexture.getRect();

    //  if(getRect()!=pRect)
    //    {
      setHeight(pRect.h);
      setWidth(pRect.w);
    }
}

AGImage::~AGImage()
{
}

void AGImage::draw(const AGRect &r)
{
  //cdebug(mTexture.surface()<<endl);
  //mTexture.toScreen(100,100);
  //  AGSurface screen(getScreen());

  // do always center for now
  bool center=true;

  if(mTile)
    getScreen().tile(mTexture,r.project(getRect()),mSrcRect);
  else if(center)
    {
      
      AGRect mr=getRect();

      mr.x+=(width()-mTexture.width())/2;
      mr.y+=(height()-mTexture.height())/2;

      mr=r.project(mr);
      getScreen().blit(mTexture,mr);
    }
  else
    getScreen().blit(mTexture,r.project(getRect()));
}

void AGImage::setSurface(AGSurface pSurface)
{
  mTexture=AGTexture(pSurface);
}

/*
void AGImage::setSourceRect(const AGRect &pRect)
{
  mSrcRect=pRect;
}
*/
