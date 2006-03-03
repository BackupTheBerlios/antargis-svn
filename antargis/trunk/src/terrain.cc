#include "terrain.h"
#include <ag_texturecache.h>
#include <ag_rendercontext.h>

//////////////////////////////////////////////////////////////////////////
// TerrainPieceVA
//////////////////////////////////////////////////////////////////////////
TerrainPieceVA::TerrainPieceVA(Scene *pScene,Terrain *t,HeightMap &map,int xs,int ys,int w,int h,const AGVector4 &pPos):
  SceneNode(pScene,AGVector4(),AGBox3()),//AGVector3(xs,ys,-10),AGVector3(xs+w,ys+h,30))),
  mXs(xs),mYs(ys),mW(w),mH(h),
  mMap(&map)
{
  mTerrain=t;
  mapChanged();
  setOrder(TERRAIN_Z);
}

void TerrainPieceVA::mapChanged()
{
  AGBox3 bb=bbox();

  // first check, if this piece was really affected
  if(bb.valid())
    {
      AGRect2 r=AGRect2(AGVector2(bb.base[0],bb.base[1]),AGVector2(bb.base[0]+bb.dir[0],bb.base[1]+bb.dir[1]));

      if(!r.collide(mMap->getChangeRect()))
	{
	  return;
	}
    }


  bb=AGBox3();
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

	n=mMap->getNormal(x,y);

	float texHeight=mMap->getTerrainScale(x,y);

        tp3=AGVector3(-v[0]*texFactor3w,-v[1]*texFactor3w,texHeight);
        m3dArray.addVertex(v,white,n,tp3);

	bb.include(v.dim3());
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
  setBBox(bb);
}

void TerrainPieceVA::drawShadow()
{
  m3dArray.setColors(false);
  m3dArray.draw();
  m3dArray.setColors(true);
}
void TerrainPieceVA::drawDepth()
{
  return;
  glDepthMask(true);
  m3dArray.setColors(false);
  m3dArray.draw();
  m3dArray.setColors(true);
}

void TerrainPieceVA::drawPick()
{
  m3dArray.drawPick();
}


void TerrainPieceVA::draw()
{
  AGRenderContext c;
  c.setLighting(true);
  c.setTexture(mTerrain->get3dTexture()->glTexture());
  mTerrain->get3dTexture()->setFilter(GL_LINEAR,GL_LINEAR);

  c.begin();

  m3dArray.draw();
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


Terrain::Terrain(Scene *pScene,HeightMap &map):
  m3D(getTextureCache()->get3D("data/textures/terrain/new3d.png")),
  mGrass(getTextureCache()->get("data/textures/terrain/grass4.png")),
  mMap(&map),
  mScene(pScene)
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
	TerrainPieceVA *t=new TerrainPieceVA(getScene(),this,*mMap,x,y,tilesize,tilesize,AGVector4(x,y,0,0));
	WaterPiece *w=new WaterPiece(getScene(),*mMap,x,y,tilesize,tilesize,AGVector4(x,y,0,0));
	pieces.push_front(t); // at least it's correct at the beginning
	water.push_front(w);
	mNodes.push_back(w);
	mNodes.push_back(t);
	tiles++;
      }

  w=mMap->getW();
  h=mMap->getH();
}

void Terrain::mapChangedComplete()
{
  for(Pieces::iterator i=pieces.begin();i!=pieces.end();i++)
    delete *i;
  for(WPieces::iterator i=water.begin();i!=water.end();i++)
    delete *i;

  pieces.clear();
  water.clear();
  mNodes.clear();


  init();
}


Terrain::~Terrain()
{
  for(Nodes::iterator i=mNodes.begin();i!=mNodes.end();i++)
    saveDelete(*i);
}

void Terrain::mapChanged()
{
  for(Pieces::iterator i=pieces.begin();i!=pieces.end();i++)
    {
      (*i)->mapChanged();
    }
  for(WPieces::iterator i=water.begin();i!=water.end();i++)
    {
      (*i)->mapChanged();
    }
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

Scene *Terrain::getScene()
{
  return mScene;
}
