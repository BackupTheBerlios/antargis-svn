#include "terrain.h"
#include <ag_texturecache.h>

const GLenum baseTexUnit= GL_TEXTURE0;
const GLenum addTexUnit= GL_TEXTURE2;

//#define FINE


//////////////////////////////////////////////////////////////////////////
// TerrainPieceVA
//////////////////////////////////////////////////////////////////////////
TerrainPieceVA::TerrainPieceVA(HeightMap &map,int xs,int ys,int w,int h,const AGVector4 &pPos):
  //  mPosition(pPos+AGVector4(1.5,1.5,0,0)),
  mXs(xs),mYs(ys),mW(w),mH(h),
  mMap(&map),
  m3D(getTextureCache()->get3D("data/textures/terrain/new3d.png"))
{
  mapChanged();
  setOrder(TERRAIN_Z);
}

void TerrainPieceVA::mapChanged()
{
  mBBox=AGBox3();
  CTRACE;
  m3dArray.clear();

  AGVector4 white(1,1,1,1);
  AGVector4 v;
  AGVector3 n;
  AGVector2 tp;
  AGVector3 tp3;
  float texFactor3w=1;
  float texFactor3=1;

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

        tp3=AGVector3(-v[0]*texFactor3w,-v[1]*texFactor3w,1-mMap->getGrass(x,y)*texFactor3);
        m3dArray.addVertex(v,white,n,tp3);


	mBBox.include(v.dim3());
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
	    m3dArray.addTriangle(p2,p1,p0);
	    m3dArray.addTriangle(p3,p1,p2);
	  }
	else
	  {
	    m3dArray.addTriangle(p3,p1,p0);
	    m3dArray.addTriangle(p3,p0,p2);
	  }

      }
}

void TerrainPieceVA::drawShadow()
{
  m3dArray.setColors(false);
  m3dArray.draw();
  m3dArray.setColors(true);
}
void TerrainPieceVA::drawDepth()
{
  glDepthMask(true);
  m3dArray.setColors(false);
  m3dArray.draw();
  m3dArray.setColors(true);
}

void TerrainPieceVA::drawPick()
{
  m3dArray.drawPick();
}

AGBox3 TerrainPieceVA::bbox()
{
  return mBBox;
}


void TerrainPieceVA::draw()
{
  //  cdebug(getOrder());
  /*  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  glDepthMask(true);
  glEnable(GL_COLOR_MATERIAL);
  //glColorMaterial(GL_FRONT,GL_DIFFUSE);
  glEnable(GL_BLEND);
  
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glColor4f(1,1,1,1);
  glEnable(GL_LIGHTING);
  
#ifndef OLD_TEX*/


  glEnable(GL_LIGHTING);


  glDisable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_3D);
  glBindTexture(GL_TEXTURE_3D,m3D.getTextureID());
glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
  //  glEnable(GL_TEXTURE_3D);

  m3dArray.draw();

  glDisable(GL_TEXTURE_3D);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,0);


  /*
#else
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
#endif
  glDepthMask(true);
  
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

  */
}


AGVector4 TerrainPieceVA::lineHit(const AGLine3 &pLine) const
{
  return m3dArray.lineHit(pLine);
}

size_t TerrainPieceVA::getTriangles() const
{
  return m3dArray.getTriangles();
}


////////////////////////////////////////////////////////////////////////////
// TerrainMesh
////////////////////////////////////////////////////////////////////////////


Terrain::Terrain(HeightMap &map)
{
  size_t y,x;

  int tilesize=16;

  for(y=0; y<map.getH();y+=tilesize)
    for(x=0;x<map.getW();x+=tilesize)
      {
	TerrainPieceVA *t=new TerrainPieceVA(map,x,y,tilesize,tilesize,AGVector4(x,y,0,0));
	WaterPiece *w=new WaterPiece(map,x,y,tilesize,tilesize,AGVector4(x,y,0,0));
	pieces.push_front(t); // at least it's correct at the beginning
	water.push_front(w);
	mNodes.push_back(w);
	mNodes.push_back(t);
      }
  w=map.getW();
  h=map.getH();
}

Terrain::~Terrain()
{
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    saveDelete(*i);
}

void Terrain::mapChanged()
{
  CTRACE;
  for(Pieces::iterator i=pieces.begin();i!=pieces.end();i++)
    {
      (*i)->mapChanged();
    }
  for(WPieces::iterator i=water.begin();i!=water.end();i++)
    {
      (*i)->mapChanged();
    }
}

void Terrain::addToScenes()
{
  CTRACE;
  for(Nodes::iterator j=mNodes.begin();j!=mNodes.end();j++)
      addToAllScenes(*j);
}

