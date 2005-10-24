#include "terrain.h"
#include <ag_texturecache.h>

const GLenum baseTexUnit= GL_TEXTURE0;
const GLenum addTexUnit= GL_TEXTURE1;

//#define FINE


//////////////////////////////////////////////////////////////////////////
// TerrainPieceVA
//////////////////////////////////////////////////////////////////////////
TerrainPieceVA::TerrainPieceVA(HeightMap &map,int xs,int ys,int w,int h,const AGVector4 &pPos):
  //  mPosition(pPos+AGVector4(1.5,1.5,0,0)),
  mXs(xs),mYs(ys),mW(w),mH(h),
  mMap(&map),
  mEarth(getTextureCache()->get("data/textures/terrain/rough_earth.png")),
  mGrass(getTextureCache()->get("data/textures/terrain/big_grass.png")),
  mGrassShadow(getTextureCache()->get("data/textures/terrain/big_grass_shadow.png"))
{
  mapChanged();
}

void TerrainPieceVA::mapChanged()
{
  CTRACE;
  mEarthArray.clear();
  mGrassArray.clear();
#ifdef THREE_LAYERS
  mGrass2Array.clear();
#endif
  AGVector4 white(1,1,1,1);
  AGVector4 v;
  AGVector3 n;
  AGVector2 tp;

  float texFactor=0.5;
  // add Vertices
  size_t x,y;
  for(x=mXs;x<=mXs+mW;x++)
    for(y=mYs;y<=mYs+mH;y++)
      {
	float grassHeight=mMap->getGrass(x,y);
	AGVector4 c(1,1,1,grassHeight);

	v=mMap->getVertex(x,y);
	n=mMap->getNormal(x,y);
	v+=AGVector4(0,0,0,0);

	tp=AGVector2(-v[0]*texFactor,-v[1]*texFactor);

#ifdef THREE_LAYERS
	mEarthArray.addVertex(v,white,n,tp);
#else
	mEarthArray.addVertex(v,c,n,tp);
#endif

	v+=AGVector4(0,0,0.07,0);
	mGrassArray.addVertex(v,c,n,tp);
#ifdef THREE_LAYERS
	v+=AGVector4(0,0,0.07,0);

	mGrass2Array.addVertex(v,c,n,tp);
#endif
      }

  for(x=mXs;x<mXs+mW;x++)
    for(y=mYs;y<mYs+mH;y++)
      {
	int p0=x-mXs+(y-mYs)*(mW+1);
	int p1=p0+1;
	int p2=p0+(mW+1);
	int p3=p2+1;
	if(((x+y)%2)==0)
	  {
	    mEarthArray.addTriangle(p0,p1,p2);
	    mEarthArray.addTriangle(p2,p1,p3);
	    
	    mGrassArray.addTriangle(p0,p1,p2);
	    mGrassArray.addTriangle(p2,p1,p3);
#ifdef THREE_LAYERS	    
	    mGrass2Array.addTriangle(p0,p1,p2);
	    mGrass2Array.addTriangle(p2,p1,p3);
#endif
	  }
	else
	  {
	    mEarthArray.addTriangle(p0,p1,p3);
	    mEarthArray.addTriangle(p2,p0,p3);
	    
	    mGrassArray.addTriangle(p0,p1,p3);
	    mGrassArray.addTriangle(p2,p0,p3);
	    
#ifdef THREE_LAYERS	    
	    mGrass2Array.addTriangle(p0,p1,p3);
	    mGrass2Array.addTriangle(p2,p0,p3);
#endif
	  }

      }
}

void TerrainPieceVA::drawShadow()
{
#ifdef THREE_LAYERS	    
  mGrassArray.setColors(false);
  mGrassArray.draw();
  mGrassArray.setColors(true);
#else
  mEarthArray.setColors(false);
  mEarthArray.draw();
  mEarthArray.setColors(true);
#endif
}
void TerrainPieceVA::drawDepth()
{
  glDepthMask(true);
  mEarthArray.setColors(false);
  mEarthArray.draw();
  mEarthArray.setColors(true);
}

void TerrainPieceVA::draw()
{
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  glDepthMask(true);
  glEnable(GL_COLOR_MATERIAL);
  //glColorMaterial(GL_FRONT,GL_DIFFUSE);
  glEnable(GL_BLEND);
  
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glColor4f(1,1,1,1);
  glEnable(GL_LIGHTING);

#ifdef THREE_LAYERS

  glBindTexture(GL_TEXTURE_2D,mEarth.getTextureID());

#ifdef FINE
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
  mEarthArray.draw();
  
  glDepthMask(false);
  glBindTexture(GL_TEXTURE_2D,mGrass.getTextureID());
#ifdef FINE
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
  mGrassArray.draw();


  glBindTexture(GL_TEXTURE_2D,mGrassShadow.getTextureID());
#ifdef FINE
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
  mGrass2Array.draw();

#else
  


  // MULTITEXTURING
  glActiveTexture(addTexUnit); // 2nd stage
  glBindTexture(GL_TEXTURE_2D,mGrass.getTextureID());
  glEnable(GL_TEXTURE_2D);
  
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_INTERPOLATE_EXT);

  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);

  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);

  //  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_EXT, GL_PRIMARY_COLOR_EXT);
  //  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_EXT, GL_CONSTANT_ALPHA);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_EXT, GL_PRIMARY_COLOR_EXT);
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_EXT, GL_ONE_MINUS_SRC_COLOR);


  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  


  glActiveTexture(baseTexUnit);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,mEarth.getTextureID());
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
  glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    mEarthArray.draw();
  
  glDepthMask(false);

  // MULTITEXTURING
  glActiveTexture(addTexUnit);
  glBindTexture(GL_TEXTURE_2D,0); // reset Texture unit 1
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(baseTexUnit);

  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D,mGrassShadow.getTextureID());
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  mGrassArray.draw();

#endif
  glDepthMask(true);
  
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
}


AGVector4 TerrainPieceVA::lineHit(const AGLine3 &pLine) const
{
  return mEarthArray.lineHit(pLine);
  /*
  AGMatrix4 tr(-mPosition);
  AGMatrix4 complete=tr;
  
  AGVector4 p0(pLine.getV0(),1);
  AGVector4 p1(pLine.getV1(),1);
  
  p0=complete*p0;
  p1=complete*p1;
  return mEarthArray.lineHit(AGLine3(AGVector3(p0[0],p0[1],p0[2]),
				     AGVector3(p1[0],p1[1],p1[2])))+AGVector4(mPosition.dim3(),0);
  */
}

size_t TerrainPieceVA::getTriangles() const
{
  return mEarthArray.getTriangles()*3;
}


////////////////////////////////////////////////////////////////////////////
// TerrainMesh
////////////////////////////////////////////////////////////////////////////


TerrainMesh::TerrainMesh(HeightMap &map)
{
  size_t y,x;
  for(y=0; y<map.getH();y+=32)
    for(x=0;x<map.getW();x+=32)
      {
	pieces.push_front(new TerrainPieceVA(map,x,y,32,32,AGVector4(x,y,0,0))); // at least it's correct at the beginning
	water.push_front(new WaterPiece(AGVector4(x,y,0,0)));
      }
}

TerrainMesh::~TerrainMesh()
{
}
void TerrainMesh::advance(float t)
{
  for(std::list<WaterPiece*>::iterator i=water.begin();i!=water.end();i++)
    (*i)->advance(t);
}
void TerrainMesh::draw()
{
  for(std::list<TerrainPieceVA*>::iterator i=pieces.begin();i!=pieces.end();i++)
    (*i)->draw();
  for(std::list<WaterPiece*>::iterator i=water.begin();i!=water.end();i++)
    (*i)->draw();
}

void TerrainMesh::drawShadow()
{
  // already sorted in draw()
  for(std::list<TerrainPieceVA*>::iterator i=pieces.begin();i!=pieces.end();i++)
    (*i)->drawShadow();
}
void TerrainMesh::drawDepth()
{
  // already sorted in draw()
  for(std::list<TerrainPieceVA*>::iterator i=pieces.begin();i!=pieces.end();i++)
    (*i)->drawDepth();
}

void TerrainMesh::sort(const AGVector4 &pCamera)
{
  // FIXME: sort along distance to camera position
}
AGVector4 TerrainMesh::lineHit(const AGLine3 &pLine) const
{
  AGVector4 r(0,0,0,0);
  for(std::list<TerrainPieceVA*>::const_iterator i=pieces.begin();i!=pieces.end();i++)
    {
      r=(*i)->lineHit(pLine);
      if(r[3]==1)
	return r;
    }
  return r;
}

size_t TerrainMesh::getTriangles() const
{
  size_t t=0;
  for(std::list<TerrainPieceVA*>::const_iterator i=pieces.begin();i!=pieces.end();i++)
    t+=(*i)->getTriangles();
  for(std::list<WaterPiece*>::const_iterator i=water.begin();i!=water.end();i++)
    t+=(*i)->getTriangles();
  return t;
}  

void TerrainMesh::mapChanged()
{
  CTRACE;
  for(Pieces::iterator i=pieces.begin();i!=pieces.end();i++)
    {
      (*i)->mapChanged();
    }
}


TerrainMesh *toTerrainMesh(SceneNode *n)
{
  return dynamic_cast<TerrainMesh*>(n);
}
