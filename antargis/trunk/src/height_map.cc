#include "height_map.h"
#include "terrain.h"

//////////////////////////////////////////////////////////////////////////
// HeightMap
//////////////////////////////////////////////////////////////////////////

std::vector<float> genSomeHeights(int mW,int mH,float mMaxHeight);

HeightMap::HeightMap(int w,int h):
  mW(w),mH(h)
{
  mHeights=genSomeHeights(w+2,h+2,5);
  mGrass=genSomeHeights(w+2,h+2,1);

  mTerrain=new Terrain(*this);
}

HeightMap::~HeightMap()
{
  delete mTerrain;
}

/*Terrain *HeightMap::getTerrainMesh()
{
  return mTerrainMesh;
  }*/

/*
void HeightMap::setScene(Scene *pScene)
{
  mScene=pScene;
}
Scene *HeightMap::getScene()
{
  return mScene;
  }*/

void HeightMap::setHeight(float height)
{
  for(size_t y=0;y<mH+2;y++)
    for(size_t x=0;x<mW+2;x++)
      {
	set(x,y,height);
	setGrass(x,y,0);
      }

  mapChanged();
}

void HeightMap::set(size_t x,size_t y,float height)
{
  assert(x>=0);
  assert(y>=0);
  assert(x<mW+2);
  assert(y<mH+2);
  mHeights[x+y*(mW+2)]=height;
}
void HeightMap::setGrass(size_t x,size_t y,float height)
{
  assert(x>=0);
  assert(y>=0);
  assert(x<mW+2);
  assert(y<mH+2);
  mGrass[x+y*(mW+2)]=height;
}


void HeightMap::loadXML(const Node &node)
{
  mW=toInt(node.get("width"));
  mH=toInt(node.get("height"));

  cdebug("mW:"<<mW);
  cdebug("mH:"<<mH);
  mHeights=std::vector<float>(mW*mH*4);
  mGrass=std::vector<float>(mW*mH*4);
  Node::NodeVector hv=node.get_children("height");
  Node::NodeVector gv=node.get_children("grass");
  if(hv.size()==0 || gv.size()==0)
    return;
  assert(hv.size()==1);
  assert(gv.size()==1);
  Node &h=**hv.begin();
  Node &g=**gv.begin();
  
  std::istringstream ish(h.getContent());
  std::istringstream isg(g.getContent());

  //  cdebug(g.getContent());
  float f;
  for(size_t y=0;y<mH+2;y++)
    {
      for(size_t x=0;x<mW+2;x++)
	{
	  ish>>f;
	  mHeights[x+y*(mW+2)]=f;
	  isg>>f;
	  mGrass[x+y*(mW+2)]=f;
	}
    }
  mapChanged();
}

void HeightMap::mapChanged()
{
  mTerrain->addToScenes();
  mTerrain->mapChanged();
  /*
  if(mScene)
    {
      mScene->addNode(mTerrainMesh);
      mScene->mapChanged();
      }*/
}


void HeightMap::saveXML(Node &node) const
{
  node.set("width",toString(mW));
  node.set("height",toString(mH));

  std::ostringstream osh,osg;
  osh.precision(2);
  osg.precision(2);

  for(size_t y=0;y<mH+2;y++)
    {
      for(size_t x=0;x<mW+2;x++)
	{
	  osh<<get(x,y)<<" ";
	  osg<<getGrass(x,y)<<" ";
	}
      osh<<std::endl;
      osg<<std::endl;
    }
  node.newChild("height").setContent(osh.str());
  node.newChild("grass").setContent(osg.str());
}


std::vector<float> genSomeHeights(int mW,int mH,float mMaxHeight)
{
  std::vector<float> h(mW*mH);
  int randMax=10000;
  
  for(int i=0;i<mW*mH;i++)
    h[i]=(rand()%randMax)*mMaxHeight/randMax;
  
  int x,y;
  // blur
  
  for(int j=0;j<2;j++) // once
    for(x=0;x<mW-1;x++)
      for(y=0;y<mH-1;y++)
	h[x+y*mW]=(h[x+y*mW]+h[x+y*mW+1]+h[x+(y+1)*mW]+h[x+1+(y+1)*mW])/4;
  
  
  return h;
}

float HeightMap::get(size_t x,size_t y) const
{
  assert(x>=0);
  assert(y>=0);
  assert(x<mW+2);
  assert(y<mH+2);
  return mHeights[x+y*(mW+2)];
}
float HeightMap::getGrass(size_t x,size_t y) const
{
  assert(x>=0);
  assert(y>=0);
  assert(x<mW+2);
  assert(y<mH+2);
  return mGrass[x+y*(mW+2)];
}

AGVector4 HeightMap::getVertex(int x,int y)
{
  return AGVector4(x,y,getHeight(x,y),1);
}

float HeightMap::getHeight(float x,float y) const
{
  int mx=(int)(x);
  int my=(int)(y);

  float dx=x-mx;
  float dy=y-my;

  float a0=get(mx,my);
  float a1=get(mx+1,my);
  float a2=get(mx,my+1);
  float a3=get(mx+1,my+1);

  float h=(a0*(1-dx)+a1*dx)*(1-dy)+(a2*(1-dx)+a3*dx)*dy;
  return h;

}
AGVector3 HeightMap::getNormal(int x,int y) const
{
  AGVector4 v1=AGVector4(1,0,get(x+1,y)-get(x,y),0);
  AGVector4 v2=AGVector4(0,1,get(x,y+1)-get(x,y),0);
  
  AGVector4 v3=v1%v2;
  v3.normalize3();
  //  v3*=-1;
  return AGVector3(v3[0],v3[1],v3[2]);
}

AGVector2 HeightMap::truncPos(const AGVector2 &p) const
{
  AGVector2 maxPos(mW+1,mH+1);
  return AGVector2(std::max(0.0f,std::min(maxPos[0],p[0])),
		   std::max(0.0f,std::min(maxPos[1],p[1])));
}
