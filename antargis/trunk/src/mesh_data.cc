#include "mesh_data.h"
#include <ag_texturecache.h>
#include "mesh_optimizer.h"


//////////////////////////////////////////////////////////////////////
// MeshData
//////////////////////////////////////////////////////////////////////

MeshData::MeshData(const VertexArray &va,const std::string &pTexture,bool pShadow):mBBox(AGVector3(0,0,0),AGVector3(-1,0,0))
{
  mArray=va;
  mWithTexture=false;
  if(pTexture!="")
    {
      mTexture=getTextureCache()->get(pTexture);
      mWithTexture=true;
    }
  mShadow=pShadow;
  //  mAlpha=false;
  mTransparent=false;
  overdraw=false;
  drawColors=true;
  mPickable=true;
  mLighting=true;
}


MeshData::MeshData(const std::string &filename,float zoom,const std::string &pTexture,bool pShadow):mBBox(AGVector3(0,0,0),AGVector3(-1,0,0))
{
  Uint16 faces,meshes,vertices;
  overdraw=false;

  mTransparent=false;
  mLighting=true;

  //  mAlpha=false;

  mWithTexture=false;
  if(pTexture!="")
    {
      mTexture=getTextureCache()->get(pTexture);
      mWithTexture=true;
    }
  
  MeshVertex mVertices[4];
  MeshOptimizer opt;
  bool withTex=filename.find(".ant2")!=filename.npos;
  
  FILE *f=fopen(filename.c_str(),"rb");
  assert(f);
  fread(&meshes,sizeof(Uint16),1,f);
  
  for(;meshes>0;meshes--)
    {
      fread(&faces,sizeof(Uint16),1,f);
      
      for(Uint16 i=0;i<faces;i++)
	{
	  fread(&vertices,sizeof(Uint16),1,f);
	  assert(vertices<=4);
	  for(Uint16 j=0;j<vertices;j++)
	    {
	      fread(mVertices[j].v,sizeof(float),3,f);
	      fread(mVertices[j].n,sizeof(float),3,f);
	      fread(mVertices[j].c,sizeof(float),3,f);
	      if(withTex)
		fread(mVertices[j].t,sizeof(float),2,f);
	      mVertices[j].v*=zoom;
	      mVertices[j].v[3]=1;

	      mBBox.include(mVertices[j].v.dim3());
	    }
	  if(vertices==3)
	    {
	      opt.add(mVertices[0]);
	      opt.add(mVertices[1]);
	      opt.add(mVertices[2]);
	    }
	  else
	    {
	      opt.add(mVertices[0]);
	      opt.add(mVertices[1]);
	      opt.add(mVertices[2]);
	      opt.add(mVertices[0]);
	      opt.add(mVertices[2]);
	      opt.add(mVertices[3]);
	    }
	}
    }
  fclose(f);
  mShadow=pShadow;
  mArray=opt.getArray();
  drawColors=true;
  mPickable=true;
}

MeshData::~MeshData()
{
  //  TRACE;
}

bool MeshData::transparent()
{
  return mTransparent;
}

void MeshData::setTransparent(bool p)
{
  mTransparent=p;
}

void MeshData::setLighting(bool l)
{
  mLighting=l;
}

AGBox3 MeshData::bbox() const
{
  return mBBox;
}


void MeshData::save(const std::string &pFilename)
{
  FILE *f=fopen(pFilename.c_str(),"wb");

  size_t meshes=1;
  size_t vertices=3;
  size_t faces=mArray.getTriangles();

  // meshes
  fwrite(&meshes,sizeof(Uint16),1,f);

  // faces
  fwrite(&faces,sizeof(Uint16),1,f);
  for(size_t i=0;i<faces;i++)
    {
      fwrite(&vertices,sizeof(Uint16),1,f);
      for(size_t k=0;k<vertices;k++)
	{
	  size_t j=mArray.getIndex(i*vertices+k);
	  fwrite(mArray.getVertex(j),sizeof(float),3,f);
	  fwrite(mArray.getNormal(j),sizeof(float),3,f);
	  fwrite(mArray.getColor(j),sizeof(float),3,f);
	  fwrite(mArray.getTexCoord(j),sizeof(float),2,f);
	}
    }

  fclose(f);
}


void MeshData::setOverdraw(bool o)
{
  overdraw=o;
}

/*void MeshData::setAlpha(bool pAlpha)
{
  mAlpha=pAlpha;
  if(mAlpha)
    {
      mArray.setBuffers(false);
    }
}
bool MeshData::getAlpha() const
{
  return mAlpha;
  }*/

void MeshData::drawPick()
{
  if(mPickable)
    mArray.drawPick();
}

void MeshData::draw()
{
  if(!mShadow)
    glDepthMask(false);

  if(mTransparent)
    glDisable(GL_CULL_FACE);


  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER,0.9);

  if(mLighting)
    glEnable(GL_LIGHTING);
  else
    glDisable(GL_LIGHTING);

  glBindTexture(GL_TEXTURE_2D,0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  //  glColor4f(1,1,1,1);
  
  if(mWithTexture)
    glBindTexture(GL_TEXTURE_2D,mTexture.getTextureID());

  if(mTransparent)
    {
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//_MIPMAP_LINEAR);//NEAREST);//GL_LINEAR);

      //      glDisable(GL_LIGHTING);
      glAlphaFunc(GL_GREATER,0.4f);
      glEnable(GL_ALPHA_TEST);
    }
  if(overdraw)
    {
      glDisable(GL_ALPHA_TEST);
      glDisable(GL_DEPTH_TEST);
    }
  mArray.setColors(drawColors);
  mArray.draw();
  if(overdraw)
    {
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_ALPHA_TEST);
    }
  if(mTransparent)
    {
      glAlphaFunc(GL_GREATER,0.9f);
      glEnable(GL_LIGHTING);
    }
  //    glEnable(GL_ALPHA_TEST);


  glBindTexture(GL_TEXTURE_2D,0);
  if(!mShadow)
    glDepthMask(true);

  glDisable(GL_ALPHA_TEST);
  if(mTransparent)
    glEnable(GL_CULL_FACE);
}

void MeshData::setColors(bool c)
{
  drawColors=c;
}

size_t MeshData::getTriangles() const
{
  return mArray.getTriangles();
}

void MeshData::drawShadow()
{
  if(mTransparent)
    glDisable(GL_CULL_FACE);

  drawDepth();

  if(mTransparent)
    glEnable(GL_CULL_FACE);
}
void MeshData::drawDepth()
{
  if(mTransparent)
    {
      glDisable(GL_CULL_FACE);
      //      throw int();

      glBindTexture(GL_TEXTURE_2D,mTexture.getTextureID());
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER,0.9f);

    }
  if(mShadow)
    {
      mArray.setColors(false);
      mArray.draw();
      mArray.setColors(true);
    }
  if(mTransparent)
    {
      glEnable(GL_CULL_FACE);
      glBindTexture(GL_TEXTURE_2D,0);
    }
}

void MeshData::texCoordFromPos(float scale)
{
  for(size_t i=0;i<mArray.getTriangles()*3;i++)
    {
      size_t j=mArray.getIndex(i);

      AGVector4 v=mArray.getVertex(j);
      AGVector2 t(v[0]+v[1],v[2]);
      mArray.setTexCoord(j,t*scale);
    }
}

void MeshData::setPickable(bool p)
{
  mPickable=p;
}
