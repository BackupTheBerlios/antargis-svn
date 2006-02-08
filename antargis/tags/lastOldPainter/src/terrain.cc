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
  // first check, if this piece was really affected
  if(mBBox.dir[0]>=0) // is bbox valid ?
    {
      AGRect2 r=AGRect2(AGVector3(mBBox.base[0],mBBox.base[1],1),AGVector3(mBBox.base[0]+mBBox.dir[0],mBBox.base[1]+mBBox.dir[1],1));

      if(!r.collide(mMap->getChangeRect()))
	{
	  return;
	}
    }


  mBBox=AGBox3();
  m3dArray.clear();

  AGVector4 white(1,1,1,1);
  AGVector4 v;
  AGVector3 n;
  AGVector2 tp;
  AGVector3 tp3;
  float texFactor3w=0.5;

  // add Vertices
  size_t x,y;
  for(x=mXs;x<=mXs+mW;x++)
    for(y=mYs;y<=mYs+mH;y++)
      {
	v=mMap->getVertex(x,y);
	//	cdebug("V:"<<v.toString());

	n=mMap->getNormal(x,y);

	float texHeight=mMap->getTerrainScale(x,y);

        tp3=AGVector3(-v[0]*texFactor3w,-v[1]*texFactor3w,texHeight);
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

AGBox3 TerrainPieceVA::bbox() const
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
  mGrass(getTextureCache()->get("data/textures/terrain/grass4.png")),
  mMap(&map)
{
  init();

  map.sigMapChanged.connect(slot(this,&Terrain::slotMapChanged));
  map.sigMapChangedComplete.connect(slot(this,&Terrain::slotMapChangedComplete));
}

void Terrain::init()
{
  size_t y,x;
  int tilesize=16;
  size_t tiles=0;

  for(y=0; y<mMap->getH();y+=tilesize)
    for(x=0;x<mMap->getW();x+=tilesize)
      {
	TerrainPieceVA *t=new TerrainPieceVA(this,*mMap,x,y,tilesize,tilesize,AGVector4(x,y,0,0));
	WaterPiece *w=new WaterPiece(*mMap,x,y,tilesize,tilesize,AGVector4(x,y,0,0));
	pieces.push_front(t); // at least it's correct at the beginning
	water.push_front(w);
	mNodes.push_back(w);
	mNodes.push_back(t);
	tiles++;
      }

  cdebug("TILES:"<<tiles);
  w=mMap->getW();
  h=mMap->getH();
}

void Terrain::mapChangedComplete()
{
  for(Nodes::iterator j=mNodes.begin();j!=mNodes.end();j++)
      removeFromAllScenes(*j);

  for(Pieces::iterator i=pieces.begin();i!=pieces.end();i++)
    delete *i;
  for(WPieces::iterator i=water.begin();i!=water.end();i++)
    delete *i;

  pieces.clear();
  water.clear();
  mNodes.clear();


  init();
  addToScenes();
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

bool Terrain::slotMapChanged(AGEvent *e)
{
  mapChanged();
  return false;
}
bool Terrain::slotMapChangedComplete(AGEvent *e)
{
  mapChangedComplete();
  return false;
}
