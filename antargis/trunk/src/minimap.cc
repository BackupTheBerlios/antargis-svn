#include "minimap.h"
#include <ag_layoutfactory.h>

MiniMap::MiniMap(AGWidget *p,const AGRect &r,AntMap *pMap):
  AGWidget(p,r),
  mMap(pMap),
  mSurface(r.width(),r.height())
{
  mScene=0;
  mTexture=getTextureManager()->makeTexture(mSurface);
  mapChanged();
}
  
void MiniMap::mapChanged()
{
  if(!mMap)
    return;
  int w,h;
  int x,y;
  bool inmem=false;

  w=getRect().width();
  h=getRect().height();

  AGRect2 change=mMap->getChangeRect();
  float mw=mMap->getW();
  float mh=mMap->getH();

  int x0=change.x()/mw*w;
  int y1=change.y()/mh*h;
  int x1=(change.x()+change.w())/mw*w;
  int y0=(change.y()+change.h())/mh*h;

  y0=h-y0;
  y1=h-y1;

  AGPainter *p=0;

  if((x1-x0)*(y1-y0)>w*h/8)
    {
      p=new AGPainter(mSurface);
      inmem=true;
    }
  else
    p=new AGPainter(mTexture);

  AGVector3 light(-1,1,1);
  light.normalize();

  TerrainType t;
  
  
  if(mMap)
    {
      for(x=x0;x<x1;x++)
	for(y=y0;y<y1;y++)
	  {
	    float mx=x*mw/w;
	    float my=(1-y/float(h))*mh;

	    AGColor c(0,0,0xFF);

	    t=mMap->getTerrain(mx,my);

	    AGVector3 n=mMap->getNormalF(mx,my);

	    float l=std::min(1.0f,n*light*0.5f+0.5f);
	    
	    if(t==WATER)
	      c=AGColor(0,0,0xFF);
	    else if(t==SAND)
	      c=AGColor(0xFF,0x77,0);
	    else if(t==EARTH)
	      c=AGColor(0xAA,0x66,0);
	    else if(t==GRASS)
	      c=AGColor(0,0xFF,0);
	    else if(t==GRASS2)
	      c=AGColor(0x55,0xdd,0x55);
	    else if(t==FOREST)
	      c=AGColor(0x00,0xdd,0x00);
	    else if(t==ROCK)
	      c=AGColor(0xAA,0xAA,0xAA);
	    else if(t==ROCK2)
	      c=AGColor(0xFF,0xFF,0xFF);

	    c=c*l;

	    if(mMap->getHeight(mx,my)<0)
	      c=c*0.25+AGColor(0,0,0xFF)*0.75;
	    
	    p->putPixel(AGPoint(x,y),c);
	  }
    }
  else
    {
      for(x=0;x<w;x++)
	for(y=0;y<h;y++)
	  {
	    p->putPixel(AGPoint(x,y),AGColor(0xFF,0,0));
	  }
    }
  delete p;
  if(inmem)
    mTexture=getTextureManager()->makeTexture(mSurface);

}

void MiniMap::draw(AGPainter &p)
{
  AGRect m=getRect().origin();
  p.blit(mTexture,m);


  if(mScene)
    {
      AGVector4 cam=mScene->getCamera();
      
      int x,y,w,h;

      float mx=cam[0],my=cam[1];

      x=getRect().width()*mx/mMap->getW();
      y=getRect().height()*my/mMap->getH();
      
      y=getRect().height()-y;


      // now with approx. frustum

      w=25*getRect().width()/mMap->getW();
      h=25*getRect().height()/mMap->getH();
      
      x+=w/2;
      y+=h/2;

      AGRect r(x-w,y-h,w,h);
      AGColor c1(0xff,0xaa,0);
      AGColor c2(0xee,0x77,0);
      
      p.drawBorder(r,1,c1,c2);
      r=r.shrink(1);
      p.drawBorder(r,1,c2,c1);
    }
}

void MiniMap::setMap(AntMap *pMap)
{
  mMap=pMap;
  mapChanged();
}
void MiniMap::setScene(Scene *pScene)
{
  mScene=pScene;
}




// AGLayout creator
class AGMiniMapLayoutCreator:public AGLayoutCreator
{
public:
  REGISTER_COMPONENT(MiniMap,"miniMap")

  virtual AGWidget *create(AGWidget *pParent,const AGRect &pRect,const xmlpp::Node &pNode)
  {
    CTRACE;
    return new MiniMap(pParent,pRect,0);
  }
};
IMPLEMENT_COMPONENT_FACTORY(MiniMap);

MiniMap *toMiniMap(AGWidget *w)
{
  return dynamic_cast<MiniMap*>(w);
}
