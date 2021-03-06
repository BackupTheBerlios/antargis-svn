#include "height_map.h"
#include "terrain.h"
#include "ant_serial.h"


void serialTest()
{
  BinaryStringOut os;
  os<<3.4f;
  BinaryStringIn is(os.getString());
  float f;
  is>>f;
  assert(f==3.4f);
}

void serialTest2()
{
  size_t i=9874;
  size_t a=i;
  TRACE;
  {
    
    BinaryFileOut os("muh");
    os<<i;
    os<<3.4f;
  }
  BinaryFileIn is("muh");
  float f;
  is>>i;
  is>>f;
  assert(i==a);
  assert(f==3.4f);
}



//////////////////////////////////////////////////////////////////////////
// HeightMap
//////////////////////////////////////////////////////////////////////////

char TerrainNames[][20]={"water","sand","earth","grass","grass2","forest","rock","rock2"};

std::vector<float> genSomeHeights(int mW,int mH,float mMaxHeight);

HeightMap::HeightMap(int w,int h):
  sigMapChanged(this,"mapChanged"),
  sigMapChangedComplete(this,"mapChangedComplete"),
  mW(w),mH(h),mChangeRect(AGVector3(),AGVector3()),mChanges(0)
{
  mHeights=genSomeHeights(w+2,h+2,5);

  for(int t=FIRSTTERRAIN;t<LASTTERRAIN; t++)
    mTerrainTypes[TerrainType(t)]=genSomeHeights(w+2,h+2,1);

  mTerrain=new Terrain(*this);

  setTerrainScale(WATER,0);
  setTerrainScale(SAND,8/32.0);
  setTerrainScale(EARTH,12/32.0);
  setTerrainScale(GRASS,16/32.0);
  setTerrainScale(GRASS2,20/32.0);
  setTerrainScale(FOREST,23/32.0);
  setTerrainScale(ROCK,1);
  setTerrainScale(ROCK2,1);
  serialTest();
  serialTest2();
}

HeightMap::~HeightMap()
{
  delete mTerrain;
}

void HeightMap::setHeight(float height)
{
  for(size_t y=0;y<mH+2;y++)
    for(size_t x=0;x<mW+2;x++)
      {
	set(x,y,height);
	setTerrain(x,y,EARTH,1);
      }

  mapChanged();
}

void HeightMap::addChange(const AGVector2 &v)
{
  mChanges++;
  if(mChanges==1)
    {
      mChangeRect=AGRect2(AGVector3(v,1),AGVector3(v,1));
    }
  else
    mChangeRect.include(v);
}

AGRect2 HeightMap::getChangeRect() const
{
  return mChangeRect;
}


void HeightMap::setTerrain(size_t x,size_t y,TerrainType t,float v)
{
  size_t p=x+y*(mW+2);

  //  cdebug(p<<"  "<<EARTH<<" "<<mTerrainTypes[EARTH].size());
  assert(mTerrainTypes[EARTH].size()>p);
  mTerrainTypes[t][p]=v;
  
  float sum=0;
  for(int i=FIRSTTERRAIN;i<LASTTERRAIN;i++)
    {
      assert(mTerrainTypes[TerrainType(i)].size()>p);
      if(t!=i)
	sum+=mTerrainTypes[TerrainType(i)][p];
    }

  for(int i=FIRSTTERRAIN;i<LASTTERRAIN;i++)
    if(t!=i)
      {
	if(sum==0 || v==1)
	  mTerrainTypes[TerrainType(i)][p]=0;
	else
	  mTerrainTypes[TerrainType(i)][p]=mTerrainTypes[TerrainType(i)][p]/sum*(1-v); // so sum(mTerraintypes[x][.]) == 1
      }
  addChange(AGVector2(x,y));
}

float HeightMap::getTerrain(size_t x,size_t y,TerrainType t) const
{
  size_t p=x+y*(mW+2);
  return const_cast<HeightMap*>(this)->mTerrainTypes[t][p];
}




void HeightMap::set(size_t x,size_t y,float height)
{
  assert(x>=0);
  assert(y>=0);
  assert(x<mW+2);
  assert(y<mH+2);
  mHeights[x+y*(mW+2)]=height;
  addChange(AGVector2(x,y));
}

void HeightMap::loadBinary(const std::string &pName)
{
  BinaryFileIn is(pName);
  float f;

  is>>mW>>mH;

  cdebug("mw:"<<mW<<"  "<<mH);
  cdebug("pos:"<<is.pos());

  assert(mW<10000 && mH<10000);
  
  
  mHeights=std::vector<float>(mW*mH*4);
  for(size_t y=0;y<mH+2;y++)
    {
      for(size_t x=0;x<mW+2;x++)
	{
	  is>>f;
	  //	  cdebug(f);
	  mHeights[x+y*(mW+2)]=f;
	}
    }

  for(int i=FIRSTTERRAIN;i<LASTTERRAIN;i++)
    {
      mTerrainTypes[TerrainType(i)]=std::vector<float>(mW*mH*4);
      
      for(size_t y=0;y<mH+2;y++)
	{
	  for(size_t x=0;x<mW+2;x++)
	    {
	      is>>f;
	      mTerrainTypes[TerrainType(i)][x+y*(mW+2)]=f;
	      addChange(AGVector2(x,y));
	    }
	}
    }
}

void HeightMap::saveBinary(const std::string &pName) const
{
  CTRACE;
  BinaryFileOut os(pName);

  os<<mW<<mH;
  cdebug("pos:"<<os.pos());

  for(size_t y=0;y<mH+2;y++)
    {
      for(size_t x=0;x<mW+2;x++)
	{
	  os<<mHeights[x+y*(mW+2)];
	}
    }
  
  
  for(int i=FIRSTTERRAIN;i<LASTTERRAIN;i++)
    {
      for(size_t y=0;y<mH+2;y++)
	{
	  for(size_t x=0;x<mW+2;x++)
	    {
	      os<<const_cast<HeightMap*>(this)->mTerrainTypes[TerrainType(i)][x+y*(mW+2)]; // trust me - I'm const
	    }
	}
    }
}

void HeightMap::loadXML(const Node &node)
{
  std::string filename=node.get("filename");
  if(filename.length())
    loadBinary(filename);
  else
    {
      mW=toInt(node.get("width"));
      mH=toInt(node.get("height"));
      
      cdebug("mW:"<<mW);
      cdebug("mH:"<<mH);
      mHeights=std::vector<float>(mW*mH*4);
      
      for(int i=FIRSTTERRAIN;i<LASTTERRAIN;i++)
	{
	  mTerrainTypes[TerrainType(i)]=std::vector<float>(mW*mH*4);
	  cdebug(mW<<"   "<<mH);
	  cdebug(mTerrainTypes[TerrainType(i)].size());
	  Node::NodeVector gv=node.getChildren(TerrainNames[i]);
	  if(gv.size()==0)
	    continue;
	  Node &g=**gv.begin();
	  
	  std::istringstream is(g.getContent());
	  
	  float f;
	  for(size_t y=0;y<mH+2;y++)
	    {
	      for(size_t x=0;x<mW+2;x++)
		{
		  is>>f;
		  mTerrainTypes[TerrainType(i)][x+y*(mW+2)]=f;
		  addChange(AGVector2(x,y));
		}
	    }
	}
      
      
      Node::NodeVector hv=node.getChildren("height");
      
      if(hv.size()==0)// || gv.size()==0)
	return;
      assert(hv.size()==1);
      Node &h=**hv.begin();
      
      
      std::istringstream ish(h.getContent());
      
      float f;
      for(size_t y=0;y<mH+2;y++)
	{
	  for(size_t x=0;x<mW+2;x++)
	    {
	      ish>>f;
	      mHeights[x+y*(mW+2)]=f;
	    }
	}
    }
  
  // compete change
  mTerrain->mapChangedComplete();
  mTerrain->addToScenes();
  mChanges=0;
  mChangeRect=AGRect2(AGVector3(),AGVector3());
}

void HeightMap::newMap(int w,int h)
{
  mW=w;
  mH=h;

  mHeights=genSomeHeights(w+2,h+2,5);

  for(int t=FIRSTTERRAIN;t<LASTTERRAIN; t++)
    mTerrainTypes[TerrainType(t)]=genSomeHeights(w+2,h+2,1);
  
  // compete change
  mTerrain->mapChangedComplete();
  mTerrain->addToScenes();
  mChanges=0;
  mChangeRect=AGRect2(AGVector3(),AGVector3());

}


void HeightMap::mapChanged()
{
  sigMapChanged(new AGEvent(this,"mapChanged"));
  mTerrain->addToScenes();
  mTerrain->mapChanged();
  mChanges=0;
  mChangeRect=AGRect2(AGVector3(),AGVector3());
}


void HeightMap::saveXML(Node &node) const
{
  node.set("width",toString(mW));
  node.set("height",toString(mH));
  
  if(false) //(mW<=64 && mH<=64) || mName.length()==0)
    {
      std::ostringstream osh;
      osh.precision(2);
      
      for(size_t y=0;y<mH+2;y++)
	{
	  for(size_t x=0;x<mW+2;x++)
	    {
	      osh<<get(x,y)<<" ";
	    }
	  osh<<std::endl;
	}
      node.addChild("height").setContent(osh.str());
      
      for(int i=FIRSTTERRAIN;i<LASTTERRAIN;i++)
	{
	  std::ostringstream os;
	  os.precision(2);
	  for(size_t y=0;y<mH+2;y++)
	    {
	      for(size_t x=0;x<mW+2;x++)
		{
		  os<<getTerrain(x,y,TerrainType(i))<<" ";
		}
	      os<<std::endl;
	    }
	  node.addChild(TerrainNames[i]).setContent(os.str());
	  
	}
    }
  else
    {
      std::string name=replace(mName,".antlvl",".hmap");
      saveBinary(name);
      node.set("filename",name);
    }
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
  if(y>=mH+2)
    cdebug("y:"<<y);
  assert(y<mH+2);
  return mHeights[x+y*(mW+2)];
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

AGVector3 HeightMap::getNormalF(float x,float y) const
{
  AGVector4 v1,v2;
  if(x>2)
    v1=AGVector4(1,0,getHeight(x,y)-getHeight(x-1,y),0);
  else
    v1=AGVector4(1,0,getHeight(x+1,y)-getHeight(x,y),0);

  if(y>2)
    v2=AGVector4(0,1,getHeight(x,y)-getHeight(x,y-1),0);
  else
    v2=AGVector4(0,1,getHeight(x,y+1)-getHeight(x,y),0);
  
  AGVector4 v3=v1%v2;
  v3.normalize3();

  return AGVector3(v3[0],v3[1],v3[2]);
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

void HeightMap::setTerrainScale(TerrainType t,float s)
{
  mTerrainScale[t]=s;
}

float HeightMap::getTerrainValue(float x,float y,TerrainType t)
{
  int mx=(int)(x);
  int my=(int)(y);

  float dx=x-mx;
  float dy=y-my;

  float a0=getTerrain(mx,my,t);
  float a1=getTerrain(mx+1,my,t);
  float a2=getTerrain(mx,my+1,t);
  float a3=getTerrain(mx+1,my+1,t);

  float h=(a0*(1-dx)+a1*dx)*(1-dy)+(a2*(1-dx)+a3*dx)*dy;
  return h;
}

TerrainType HeightMap::getTerrain(float x,float y)
{
  TerrainType t=FIRSTTERRAIN;
  float mmax=0.0f;
  for(int i=FIRSTTERRAIN;i<LASTTERRAIN;i++)
    {
      float v=getTerrainValue(x,y,TerrainType(i));
      if(v>mmax)
	{
	  t=TerrainType(i);
	  mmax=v;
	}
    }
  return t;
}

// mean value
float HeightMap::getTerrainWeight(float x,float y)
{
  float mean=0;
  for(int i=FIRSTTERRAIN;i<LASTTERRAIN;i++)
    {
      float v=getTerrainValue(x,y,TerrainType(i));
      mean+=v*i;
    }
  return mean/(LASTTERRAIN-FIRSTTERRAIN);
}

float HeightMap::getMean(float x,float y)
{
  float mean=0;
  for(int i=FIRSTTERRAIN;i<LASTTERRAIN;i++)
    {
      float v=getTerrainValue(x,y,TerrainType(i));
      mean+=v*mTerrainScale[TerrainType(i)];
    }
  return mean;

}


float HeightMap::getTerrainScale(float x,float y)
{
  TerrainType t=getTerrain(x,y),t2;

  float mean=getTerrainWeight(x,y);

  if(mean<t)
    {
      t2=TerrainType(t-1);
    }
  else
    t2=TerrainType(t+1);

  float s1=mTerrainScale[t];
  float s2=mTerrainScale[t2];

  mean=fabs(std::max(std::min(mean,1.0f),-1.0f));

  //  cdebug(mean);
  float w=0;
  return getMean(x,y)*(1-w)+s1*w;

  return s1*(1-mean)+s2*mean;
}
