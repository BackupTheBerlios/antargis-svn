#include "water.h"

WaterPiece::WaterPiece(const AGVector4 &pos):mPos(pos)
{
  tex=getTextureCache()->get("data/textures/terrain/water.png");
  mW=mH=16;
  mLast=SDL_GetTicks();
}
WaterPiece::~WaterPiece()
{
}
void WaterPiece::tick()
{
  mFrameCount=SDL_GetTicks();
}

void WaterPiece::draw()
{
  shader.enable();
  drawPrivate(true);
    shader.disable();
}
void WaterPiece::advance(float t)
{
  shader.update(t);
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
  return getHeight(x,y)+calcBaseVertex(x,y);
}

AGVector4 WaterPiece::getHeight(int x,int y)
{
  return AGVector4(0.0f,0.0f,(sin(getArcX(x,y))/2+sin(getArcX(x,y)+getArcY(x,y)))*0.1);
}

AGVector4 WaterPiece::getNormal(int x,int y)
{
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
