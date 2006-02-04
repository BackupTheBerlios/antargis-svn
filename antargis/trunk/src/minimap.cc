#include "minimap.h"
#include <ag_layoutfactory.h>

#define MAP_BORDER

MiniMap::MiniMap(AGWidget *p,const AGRect &r,AntMap *pMap):
  AGWidget(p,r),
  mMap(pMap),
  mSurface(r.width(),r.height())
{
  mMapBorder=20;
  mScene=0;
  mTexture=getTextureManager()->makeTexture(mSurface);
  //  mapChanged();
  setMap(mMap);
    
}

bool MiniMap::mapChangedComplete(AGEvent *e)
{
  CTRACE;
  mapChangedP(true);
  return false;
}
  
bool MiniMap::mapChanged(AGEvent *e)
{
  mapChangedP(false);
  return false;
}

void MiniMap::mapChangedP(bool forceFull=false)
{
  if(!mMap)
    return;
  int w,h;
  int x,y;
  bool inmem=forceFull;

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

  if((x1-x0)*(y1-y0)>w*h/8 || inmem)
    {
      TRACE;
      p=new AGPainter(mSurface);
      inmem=true;
      x0=0;
      y0=0;
      x1=width()-1;
      y1=height()-1;
    }
  else
    p=new AGPainter(mTexture);

  AGVector3 light(-1,1,1);
  light.normalize();

  TerrainType t;

  assert(mMap);  
  
  if(mMap)
    {
      for(x=x0;x<x1;x++)
	for(y=y0;y<y1;y++)
	  {
	    AGVector2 mv(toMapCoords(AGVector2(x,y)));

	    //	    float mx=toMapCoordx*mw/w;
	    //	    float my=(1-y/float(h))*mh;

	    AGColor c(0,0,0xFF);

	    t=mMap->getTerrain(mv[0],mv[1]);

	    AGVector3 n=mMap->getNormalF(mv[0],mv[1]);

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

	    if(mMap->getHeight(mv[0],mv[1])<0)
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
    {
      mSurface.save("sicke.png");
      mTexture=getTextureManager()->makeTexture(mSurface);
      TRACE;
    }
  return;
}

void MiniMap::draw(AGPainter &p)
{
  AGRect m=getRect().origin();
  p.blit(mTexture,m);

  drawEntities(p);

  if(mScene)
    {
      AGVector4 cam=mScene->getCamera();
      
      int w,h;

      AGVector2 v=fromMapCoords(cam.dim2());

      // now with approx. frustum

      w=25*getRect().width()/mMap->getW();
      h=25*getRect().height()/mMap->getH();
      
      v-=AGVector2(w/2,h/2);

      AGRect r(v[0],v[1],w,h);
      AGColor c1(0xff,0xaa,0);
      AGColor c2(0xee,0x77,0);
      
      p.drawBorder(r,1,c1,c2);
      r=r.shrink(1);
      p.drawBorder(r,1,c2,c1);
    }
}

void MiniMap::drawEntities(AGPainter &p)
{
  if(mMap)
    {
      AntMap::EntityList e=mMap->getAllEntities();
      for(AntMap::EntityList::iterator i=e.begin();i!=e.end();i++)
	{
	  if((*i)->showOnMinimap())
	    {
	      AGVector2 v=(*i)->getPos2D();
	      v=fromMapCoords(v);
	      
	      p.drawRect(AGRect(v[0],v[1],2,2),(*i)->getMinimapColor());
	    }
	}

    }
}

void MiniMap::setMap(AntMap *pMap)
{
  mMap=pMap;
  if(mMap)
    {
      mMap->sigMapChanged.connect(slot(this,&MiniMap::mapChanged));
      mMap->sigMapChangedComplete.connect(slot(this,&MiniMap::mapChangedComplete));
      mapChangedComplete(0);
    }
}
void MiniMap::setScene(Scene *pScene)
{
  mScene=pScene;
}

bool MiniMap::eventMouseClick(AGEvent *m)
{
  AGPoint p(m->getMousePosition()-getScreenRect().getPosition());
  if(mMap==0 || mScene==0)
    return AGWidget::eventMouseClick(m);

  AGVector2 v(p.x,p.y);
  v=toMapCoords(v);

  mScene->setCamera(AGVector4(v[0],v[1],0,0));

  return true;
}

AGVector2 MiniMap::toMapCoords(AGVector2 v) const
{
  AGRect r=getRect();
  v[0]/=r.width();
  v[1]/=r.height();

  v[1]=1-v[1];

#ifdef MAP_BORDER
  v[0]*=mMap->getW()-mMapBorder*2;
  v[1]*=mMap->getH()-mMapBorder*2;
  v[0]+=mMapBorder;
  v[1]+=mMapBorder;
#else
  v[0]*=mMap->getW();
  v[1]*=mMap->getH();
#endif
  return v;
}
AGVector2 MiniMap::fromMapCoords(AGVector2 v) const
{
  AGRect r=getRect();

#ifdef MAP_BORDER
  v[0]-=mMapBorder;
  v[1]-=mMapBorder;

  v[0]/=mMap->getW()-mMapBorder*2;
  v[1]/=mMap->getH()-mMapBorder*2;
#else
  v[0]/=mMap->getW();
  v[1]/=mMap->getH();
#endif

  v[1]=1-v[1];

  v[0]*=r.width();
  v[1]*=r.height();

  return v;
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
