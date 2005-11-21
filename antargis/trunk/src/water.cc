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
  CTRACE;
  delete shader;
  shader=0;
}
void WaterPiece::tick()
{
  mFrameCount=SDL_GetTicks();
}

void WaterPiece::draw()
{
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
  shader->update(t);
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
  
  glPushMatrix();
  if(texture)
    glColor4f(1.0f,1.0f,1.0f,1.0f);
  glBegin(GL_TRIANGLES);
  for(x=0;x<mW;x++)
    for(y=0;y<mH;y++)
      {
	drawVertex(getV(x,y),getNormal(x,y),0,0,texture);
	drawVertex(getV(x+1,y),getNormal(x+1,y),1,0,texture);
	drawVertex(getV(x,y+1),getNormal(x,y+1),0,1,texture);
	
	drawVertex(getV(x+1,y),getNormal(x+1,y),1,0,texture);
	drawVertex(getV(x,y+1),getNormal(x,y+1),0,1,texture);
	drawVertex(getV(x+1,y+1),getNormal(x+1,y+1),1,1,texture);
      }
  glEnd();
  if(texture)
    glDisable(GL_COLOR_MATERIAL);
  glPopMatrix();

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
WaterPiece::WaterPiece(const AGVector4 &pos)
{
  mW=mH=16;
  tex=getTextureCache()->get("data/textures/terrain/water.png");


  int x,y;
  for(x=0;x<mW+1;x++)
    for(y=0;y<mH+1;y++)
      {
	AGVector4 p(x*2,y*2,0);
	p+=pos;
	AGVector4 c(1,1,1,0.7);
	AGVector3 n(0,0,1);
	AGVector2 t(x,y);
	
	mArray.addVertex(p,c,n,t);
      }
  for(x=0;x<mW;x++)
    for(y=0;y<mH;y++)
      {
	addTriangle(x,y, x+1,y, x,y+1);

	addTriangle(x+1,y, x+1,y+1, x,y+1);
      }
    
  if(!shader)
    shader=new AntWaterShader;

}

WaterPiece::~WaterPiece()
{
  CTRACE;
  delete shader;
  shader=0;
}

void WaterPiece::addTriangle(int x0,int y0,int x1,int y1,int x2, int y2)
{
  int w=mW+1;
  mArray.addTriangle(x0+y0*w,x1+y1*w,x2+y2*w);
}

void WaterPiece::draw()
{
  glEnable(GL_COLOR_MATERIAL);
  
  glBindTexture(GL_TEXTURE_2D,tex.getTextureID());

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


#endif
