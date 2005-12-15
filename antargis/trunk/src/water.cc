#include "water.h"
#include "ag_debug.h"

AntWaterShader *shader=0;

#ifdef SLOW_WATER
WaterPiece::WaterPiece(const AGVector4 &pos):mPos(pos)
{
  tex=getTextureCache()->get("data/textures/terrain/water.png");
  mW=mH=16;
  mLast=SDL_GetTicks();
  if(!shader)
    shader=new AntWaterShader;
}
WaterPiece::~WaterPiece()
{
  //  CTRACE;
  delete shader;
  shader=0;
}
void WaterPiece::tick()
{
  mFrameCount=SDL_GetTicks();
}

void WaterPiece::draw()
{
    glColor4f(1.0f,1.0f,1.0f,1.0f);
  shader->enable();
  drawPrivate(true);
  shader->disable();
}

void WaterPiece::drawPick()
{
  drawPrivate(false);
}

void WaterPiece::advance(float t)
{
  shader->update(t,getScene());
  //tick();
}

void WaterPiece::drawPrivate(bool texture)
{
  glPushMatrix();
  glTranslatef(mPos[0],mPos[1],mPos[2]);
  int x,y;
  if(texture)
    {
      glEnable(GL_COLOR_MATERIAL);
      
      glBindTexture(GL_TEXTURE_2D,tex.getTextureID());
    }
  
  if(texture)
    glColor4f(1.0f,1.0f,1.0f,1.0f);
  glBegin(GL_TRIANGLES);
  for(x=0;x<mW;x++)
    for(y=0;y<mH;y++)
      {

	drawVertex(getV(x+1,y),getNormal(x+1,y),1,0,texture);
	drawVertex(getV(x,y+1),getNormal(x,y+1),0,1,texture);
	drawVertex(getV(x,y),getNormal(x,y),0,0,texture);
	
	drawVertex(getV(x+1,y+1),getNormal(x+1,y+1),1,1,texture);
	drawVertex(getV(x,y+1),getNormal(x,y+1),0,1,texture);
	drawVertex(getV(x+1,y),getNormal(x+1,y),1,0,texture);
      }
  glEnd();
  if(texture)
    glDisable(GL_COLOR_MATERIAL);

  glPopMatrix();
}

AGVector4 WaterPiece::calcBaseVertex(int x,int y)
{
  return AGVector4(x*2,y*2,0);
  //  return AGVector4(x*2.0*gW/mW-gW, y*gH*2.0/mH-gH,0);
}

float WaterPiece::getArcX(int x,int y)
{
  float anim=mFrameCount*2.0*M_PI/4000.0;
  return x*2.0f*M_PI/3.0-2*anim;
}

float WaterPiece::getArcY(int x,int y)
{
  float anim=mFrameCount/4000.0*2.0*M_PI;
  return y*2.0f*M_PI/3.0+anim;
}

AGVector4 WaterPiece::getV(int x,int y)
{
  return calcBaseVertex(x,y);
  return getHeight(x,y);+calcBaseVertex(x,y);
}

AGVector4 WaterPiece::getHeight(int x,int y)
{
  return AGVector4(0.0f,0.0f,(sin(getArcX(x,y))/2+sin(getArcX(x,y)+getArcY(x,y)))*0.1);
}

AGVector4 WaterPiece::getNormal(int x,int y)
{
  //  return AGVector4(0,0,1,0);
  AGVector4 v1,v2;
  v1=getV(x,y+1)-getV(x,y);
  v2=getV(x+1,y)-getV(x,y);
  
  
  AGVector4 n=v2%v1;
  
  n[2]/=1;
  n.normalize3();
  return n;
}

void WaterPiece::drawVertex(AGVector4 v,AGVector4 n,float tx,float ty, bool texture)
{
  if(texture)
    glColor4f(1.0f,1.0f,1.0f,0.7f);
  glNormal3fv(n);
  glTexCoord2f(tx,ty);
  glVertex3fv(v);
}


size_t WaterPiece::getTriangles() const
{
  return mW*mH*2;
}

#else
WaterPiece::WaterPiece(HeightMap &map,int x,int y,int w,int h,const AGVector4 &pos):
  mX(x),mY(y),mW(w),mH(h),mMap(&map)
{
  step=2;
  tex=getTextureCache()->get("data/textures/terrain/water.png");
  mPos=pos;
  if(!shader)
    shader=new AntWaterShader;

  mapChanged();
  setOrder(WATER_Z);
}


WaterPiece::~WaterPiece()
{
  CTRACE;
  delete shader;
  shader=0;
}

void WaterPiece::mapChanged()
{
  CTRACE;
  mBBox=AGBox3();
  mArray.clear();

  setVisible(false);
  bool vis=false;
  int x,y;
  int step=2;
  for(x=0;x<mW+step;x+=step)
    for(y=0;y<mH+step;y+=step)
      {
	AGVector4 p(x,y,0);
	p+=mPos;
	AGVector4 c(1,1,1,0.7);
	AGVector3 n(0,0,1);
	AGVector2 t(x,y);
	
	mArray.addVertex(p,c,n,t);

	mBBox.include(p.dim3()-AGVector3(0,0,1));
	mBBox.include(p.dim3()+AGVector3(0,0,1));

	if(mMap->get(mX+x,mY+y)<0.1 && !vis)
	  {
	    vis=true;
	    cdebug(mX+x<<" "<<mY+y<<":"<<mMap->get(x,y));
	  }
      }
  for(x=0;x<mW/step;x++)
    for(y=0;y<mH/step;y++)
      {
	addTriangle(x,y, x+1,y, x,y+1);

	addTriangle(x+1,y, x+1,y+1, x,y+1);
      }

  if(vis)
    setVisible(true);
    

  //  mBBox=AGBox3(mPos.dim3()+AGVector3(0,0,-2),AGVector3(mW+1,mH+1,4));

}

void WaterPiece::addTriangle(int x0,int y0,int x1,int y1,int x2, int y2)
{
  int w=(mW/step)+1;
  mArray.addTriangle(x2+y2*w,x1+y1*w,x0+y0*w);
}

void WaterPiece::draw()
{
  glEnable(GL_COLOR_MATERIAL);
  
  glBindTexture(GL_TEXTURE_2D,tex.getTextureID());
  glColor4f(1,1,1,1);

  shader->enable();
  mArray.draw();
  shader->disable();

  glDisable(GL_COLOR_MATERIAL);

}

void WaterPiece::drawPick()
{
  mArray.drawPick();
}

void WaterPiece::advance(float t)
{
  shader->update(t);
}

size_t WaterPiece::getTriangles() const
{
  return mArray.getTriangles();
}

bool WaterPiece::transparent()
{
  return true;
}

AGBox3 WaterPiece::bbox()
{
  return mBBox;
}


#endif
