#include "terrain.h"
#include <ag_texturecache.h>

//////////////////////////////////////////////////////////////////////////
// TerrainPieceVA
//////////////////////////////////////////////////////////////////////////
TerrainPieceVA::TerrainPieceVA(Terrain *t,HeightMap &map,int xs,int ys,int w,int h,const AGVector4 &pPos):
  mXs(xs),mYs(ys),mW(w),mH(h),
  mMap(&map)
{
  mTerrain=t;
  mapChanged();
  setOrder(TERRAIN_Z);
}

void TerrainPieceVA::mapChanged()
{
  mBBox=AGBox3();
  m3dArray.clear();
  mGrassArray.clear();

  AGVector4 white(1,1,1,1);
  AGVector4 v;
  AGVector3 n;
  AGVector2 tp;
  AGVector3 tp3;
  float texFactor3w=0.5;
  float texFactor3=1;

  // add Vertices
  size_t x,y;
  for(x=mXs;x<=mXs+mW;x++)
    for(y=mYs;y<=mYs+mH;y++)
      {
	float grassHeight=mMap->getGrass(x,y);

	float height=mMap->get(x,y);
	if(height<0.5)
	  {
	    grassHeight*=height*2;
	  }

	AGVector4 c(1,1,1,grassHeight);

	v=mMap->getVertex(x,y);
	n=mMap->getNormal(x,y);
	v+=AGVector4(0,0,0,0);

        tp3=AGVector3(-v[0]*texFactor3w,-v[1]*texFactor3w,grassHeight*texFactor3);
        m3dArray.addVertex(v,white,n,tp3);

	mBBox.include(v.dim3());

	v+=AGVector4(0,0,0.05,0);

	float grassHeight2=std::min((1-grassHeight)*1.9f,1.0f);

	AGVector4 alpha(1,1,1,grassHeight2);
	AGVector2 tp(-v[0]*texFactor3w,-v[1]*texFactor3w);
	
	mGrassArray.addVertex(v,alpha,n,tp);


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

	    mGrassArray.addTriangle(p2,p1,p0);
	    mGrassArray.addTriangle(p3,p1,p2);


	  }
	else
	  {
	    m3dArray.addTriangle(p3,p1,p0);
	    m3dArray.addTriangle(p3,p0,p2);

	    mGrassArray.addTriangle(p3,p1,p0);
	    mGrassArray.addTriangle(p3,p0,p2);
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
  glEnable(GL_LIGHTING);

  glDisable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_3D);
  glBindTexture(GL_TEXTURE_3D,mTerrain->get3dTexture()->getTextureID());
  glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

  m3dArray.draw();

  glDisable(GL_TEXTURE_3D);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,0);


  // draw grass


#ifdef DRAW_NICE_GRASS

  assertGL;

  glBindTexture(GL_TEXTURE_2D,mTerrain->getGrassTexture()->getTextureID());
  //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  assertGL;
  mGrassArray.draw();

  assertGL;


#endif


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


Terrain::Terrain(HeightMap &map):
  m3D(getTextureCache()->get3D("data/textures/terrain/new3d.png")),
  mGrass(getTextureCache()->get("data/textures/terrain/grass4.png"))
{
  size_t y,x;

  int tilesize=16;
  size_t tiles=0;

  for(y=0; y<map.getH();y+=tilesize)
    for(x=0;x<map.getW();x+=tilesize)
      {
	TerrainPieceVA *t=new TerrainPieceVA(this,map,x,y,tilesize,tilesize,AGVector4(x,y,0,0));
	WaterPiece *w=new WaterPiece(map,x,y,tilesize,tilesize,AGVector4(x,y,0,0));
	pieces.push_front(t); // at least it's correct at the beginning
	water.push_front(w);
	mNodes.push_back(w);
	mNodes.push_back(t);
	tiles++;
      }

  cdebug("TILES:"<<tiles);
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

AGTexture *Terrain::get3dTexture()
{
  return &m3D;
}

AGTexture *Terrain::getGrassTexture()
{
  return &mGrass;
}
