#include "path_weighter.h"

#include <ag_debug.h>
#include <ag_profiler.h>
#include "height_map.h"
#include <math.h>

DistanceComputer::DistanceComputer(HeightMap *pHeightMap,
				   float pStepX,float pStepY):
  mHeightMap(pHeightMap),
  mStepX(pStepX),mStepY(pStepY)
{
  assert(mHeightMap);
}

bool DistanceComputer::isPassable(const AGVector2 &pPoint) const
{
  return mHeightMap->getHeight(pPoint[0],pPoint[1])>-0.2;
}

float DistanceComputer::beginX() const
{
  return 0;
}
float DistanceComputer::beginY() const
{
  return 0;
}
float DistanceComputer::stepX() const
{
  return mStepX;
}
float DistanceComputer::stepY() const
{
  return mStepY;
}
float DistanceComputer::endX() const
{
  return mHeightMap->getW();
}
float DistanceComputer::endY() const
{
  return mHeightMap->getH();
}

float DistanceComputer::width() const
{
  return endX()-beginX();
}

float DistanceComputer::height() const
{
  return endY()-beginY();
}


// FIXME: maybe move this to other class ?
float DistanceComputer::simpleWeight(const AGVector2 &a,const AGVector2 &b) const
{
  STACKTRACE;
  float ha=mHeightMap->getHeight(a[0],a[1]);
  float hb=mHeightMap->getHeight(b[0],b[1]);

  float d=(a-b).length();
  float hd=ha-hb;

  if(hb>ha) // up hill
    return sqrt(d*d+hd*hd);
  return d; // down hill - normal speed
}

/**
 * compute possible neighbors (w.r.t. to map-borders)
 */
std::vector<AGVector2> DistanceComputer::getNeighbors(const AGVector2 &p) const
{
  STACKTRACE;

  std::vector<AGVector2> diffList,rList;
  AGVector2 t;

  diffList.push_back(AGVector2(-stepX(),0));
  diffList.push_back(AGVector2(+stepX(),0));
  diffList.push_back(AGVector2(0,-stepY()));
  diffList.push_back(AGVector2(0,+stepY()));

  /*  
  diffList.push_back(AGVector2(-stepX(),-stepY()));
  diffList.push_back(AGVector2(+stepX(),-stepY()));
  diffList.push_back(AGVector2(-stepX(),+stepY()));
  diffList.push_back(AGVector2(+stepX(),+stepY()));
  */
  for(std::vector<AGVector2>::iterator i=diffList.begin();i!=diffList.end();i++)
    {
      t=*i+p;
      {
	STACKTRACE;
	
	if(t.getX() >= beginX() &&
	   t.getX()<=endX() && 
	   t.getY()>=beginY() && 
	   t.getY()<=endY())
	  rList.push_back(t);
      }
    }

  return rList;
}

std::vector<std::pair<AGVector2,float> > DistanceComputer::getAllReachableFrom(const AGVector2 &p) const
{
  STACKTRACE;
  float w;
  std::vector<std::pair<AGVector2,float> > rList;
  std::vector<AGVector2> nList=getNeighbors(p);

  for(std::vector<AGVector2>::iterator i=nList.begin();i!=nList.end();i++)
    {
      STACKTRACE;

      {
	w=simpleWeight(p,*i);
	STACKTRACE;
      }
      rList.push_back(std::make_pair(*i,w));
    }
  return rList;
}


std::vector<AGVector2> DistanceComputer::getAllPassable() const
{
  std::vector<AGVector2> l;

  float x,y;
  AGVector2 v(x,y);
  for(x=beginX();x<=endX();x+=stepX())
    for(y=beginY();y<=endY();y+=stepY())
      {
	v=AGVector2(x,y);
	if(isPassable(v))
	  l.push_back(v);
      }
  return l;
}
