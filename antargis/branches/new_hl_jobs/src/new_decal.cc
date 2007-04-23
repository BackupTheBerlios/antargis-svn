
#include "new_decal.h"
#include <ag_texturecache.h>
#include <ag_rendercontext.h>


NewDecal::NewDecal(Scene *pScene,AGVector2 pos,float size, HeightMap *pMap,const std::string &pTexture):
  SceneNode(pScene,AGVector4(),AGBox3()),
  mPos(pos),mSize(size),mMap(pMap),mTexture(getTextureCache()->get(pTexture))
{
  setOrder(DECAL_Z);

  inited=false;
}

void NewDecal::init()
{
  AGBox3 bb;
  //  bb=AGBox3();
  mArray.clear();
  // tesselate along map
  float x,y;

  AGVector4 p=getPos();

  float s2=mSize/2;
  float x0=-s2+p[0];
  float x1=s2+p[0];
  float y0=-s2+p[1];
  float y1=s2+p[1];


  size_t xw,yw;

  xw=yw=0;

  cdebug("x0:"<<x0<<"  "<<x1);
  cdebug("y0:"<<y0<<"  "<<y1);

  // first insert vertices
  for(x=std::max(0.0f,x0);x<=x1;)
    {
      for(y=std::max(y0,0.0f);y<=y1;)
	{
	  cdebug("c:"<<x<<"  "<<y);
	  AGVector4 v(x,y,mMap->getHeight(x,y)+0.01,1);
	  AGVector4 c(1,1,1,1);
	  AGVector3 n(mMap->getNormal(x,y));
	  AGVector2 t((x-x0)/(x1-x0),(y-y0)/(y1-y0));
	  
	  
	  mArray.addVertex(v,c,n,t);
	  bb.include(v.dim3());
	  
	  
	  
	  if(y==y1)
	    y+=1;
	  else if((int)(y)!=(int)(y1))
	    {
	      if(y!=y0)
		y+=1;
	      else
		y=int(y0)+1;
	    }
	  else
	    y=y1;
	  yw++;
	}
      if(x==x1)
	x+=1;
      else if((int)(x)!=(int)(x1))
	{
	  if(x!=x0)
	    x+=1;
	  else
	    x=int(x0)+1;
	}
      else
	x=x1;

      xw++;
    }
  assert(xw);
  yw/=xw;
  for(size_t i=0;i<xw-1;i++)
    for(size_t j=0;j<yw-1;j++)
      {
	size_t i0=i*yw+j;
	size_t i1=(i+1)*yw+j;
	size_t i2=(i+1)*yw+j+1;
	size_t i3=i*yw+j+1;

	int x,y;
	AGVector4 v=mArray.getVertex(i0);
	x=v[0];
	y=v[1];
	if(((x+y)%2)!=0)
	  {
	    mArray.addTriangle(i0,i1,i2);
	    mArray.addTriangle(i0,i2,i3);
	  }
	else
	  {
	    mArray.addTriangle(i0,i1,i3);
	    mArray.addTriangle(i1,i2,i3);
	  }
      }
  cdebug("XW:"<<xw);
  cdebug("YW:"<<yw);
  inited=true;

  setBBox(bb);
}

void NewDecal::draw()
{
  if(!inited)
    init();
  glPushMatrix();


  AGRenderContext c;
  c.setTexture(mTexture.glTexture());

  mArray.draw();

  glPopMatrix();
}

void NewDecal::setPos(const AGVector3&pPos)
{
  SceneNode::setPos(pPos);
  //  mPos3=pPos;
  inited=false;
  init();
}


void NewDecal::mapChanged()
{
  inited=false;
  init();
}
