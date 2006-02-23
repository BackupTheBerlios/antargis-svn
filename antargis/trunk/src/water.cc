#include "water.h"
#include "ag_debug.h"
#include "ag_rendercontext.h"

AntWaterShader *shader=0;

WaterPiece::WaterPiece(Scene *pScene,HeightMap &map,int x,int y,int w,int h,const AGVector4 &pos):
  SceneNode(pScene),
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
  delete shader;
  shader=0;
}

void WaterPiece::mapChanged()
{
  //  CTRACE;
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
	    //	    cdebug(mX+x<<" "<<mY+y<<":"<<mMap->get(x,y));
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
  AGRenderContext c;
  c.setAlpha(0,GL_NONE);
  c.setCulling(false);
  c.setTexture(tex.glTexture());
  //  tex.setClamp(GL_REPEAT,GL_REPEAT,GL_REPEAT);//CLAMP_TO_EDGE);
  //  c.setColor(AGVector4(0,0,1,1));
  c.begin();

  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_CULL_FACE);
  glDisable(GL_ALPHA_TEST);
  glColor4f(1,1,1,1);
  shader->enable();
  mArray.draw();
  shader->disable();
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

AGBox3 WaterPiece::bbox() const
{
  return mBBox;
}



