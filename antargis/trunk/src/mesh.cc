#include "mesh.h"

#include <map>
#include <vector>
#include <math.h>
#include <ag_debug.h>
#include <ag_texturecache.h>

//////////////////////////////////////////////////////////////////////
// Mesh-optimizing
//////////////////////////////////////////////////////////////////////

// sorting
bool MeshVertex::operator<(const MeshVertex &p) const
{
  int i;
  for(i=0;i<3;i++)
    if(v[i]<p.v[i])
      return true;
  
  for(i=0;i<4;i++)
    if(c[i]<p.c[i])
      return true;
  
  for(i=0;i<3;i++) 
    if(n[i]<p.n[i]) 
      return true; 
  
  for(i=0;i<2;i++)
    if(t[i]<p.t[i]) 
      return true;
  
    return false;
}


MeshOptimizer::MeshOptimizer()
{
  saved=0;
}
void MeshOptimizer::add(const MeshVertex &v)
{
  int index=mVertices.size();
  std::map<MeshVertex,int>::iterator i=mMap.find(v);
  if(i!=mMap.end())
    {
      saved++;
      index=i->second;
    }
  else
    {
      mVertices.push_back(v);
      mMap[v]=index;
    }
  mIndices.push_back(index);
  return;
}
VertexArray MeshOptimizer::getArray()
{
  VertexArray a;
  for(size_t i=0;i<mIndices.size();i++)
    {
      MeshVertex v=mVertices[i];
      a.addVertex(v.v,v.c,v.n,v.t);
    }
  
  
  for(size_t i=0;i<mIndices.size();i+=3)
    {
      a.addTriangle(mIndices[i],mIndices[i+1],mIndices[i+2]);
    }
  cdebug("SAVED:"<<1.0f-float(saved)/mIndices.size());
  return a;
}


//////////////////////////////////////////////////////////////////////
// MeshData
//////////////////////////////////////////////////////////////////////

MeshData::MeshData(const VertexArray &va,const std::string &pTexture,bool pShadow)
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
  
}


MeshData::MeshData(const std::string &filename,float zoom,const std::string &pTexture,bool pShadow)
{
  Uint16 faces,meshes,vertices;

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
}

MeshData::~MeshData()
{
  TRACE;
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

void MeshData::draw()
{
  if(!mShadow)
    glDepthMask(false);


  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER,0.9);

  glEnable(GL_LIGHTING);
  glBindTexture(GL_TEXTURE_2D,0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  glColor4f(1,1,1,1);
  
  if(mWithTexture)
    glBindTexture(GL_TEXTURE_2D,mTexture.getTextureID());
  mArray.draw();
  glBindTexture(GL_TEXTURE_2D,0);
  if(!mShadow)
    glDepthMask(true);

  glDisable(GL_ALPHA_TEST);
}
size_t MeshData::getTriangles() const
{
  return mArray.getTriangles();
}

void MeshData::drawShadow()
{
  drawDepth();
}
void MeshData::drawDepth()
{
  if(mShadow)
    {
      mArray.setColors(false);
      mArray.draw();
      mArray.setColors(true);
    }
}



//////////////////////////////////////////////////////////////////////
// Mesh
//////////////////////////////////////////////////////////////////////

Mesh::Mesh()
{
  mData=0;
  mRotation=0;
}

Mesh::Mesh(MeshData &data,const AGVector4 &pPos,float pRot)
{
  mData=&data;
  mPos=pPos;
  mRotation=pRot;
}

void Mesh::draw()
{
  begin();
  if(mData)
    mData->draw();
  end();
}
void Mesh::drawDepth()
{
  begin();
  if(mData)
    mData->drawDepth();
  end();
}
void Mesh::drawShadow()
{
  begin();
  if(mData)
    mData->drawShadow();
  end();
}

size_t Mesh::getTriangles() const
{
  if(!mData)
    return 0;
  return mData->getTriangles();
}

void Mesh::begin()
{
  glPushMatrix();
  glTranslatef(mPos[0],mPos[1],mPos[2]);
  glRotatef(mRotation,0.0,0.0,1.0);
}
void Mesh::end()
{
  glPopMatrix();
}


AGVector4 Mesh::lineHit(const AGLine3 &pLine) const
{
  if(!mData)
    return AGVector4(0,0,0,0);
  AGMatrix4 rot(-mRotation*M_PI/180.0,AGVector3(0,0,1));
  AGMatrix4 tr(-mPos);
  AGMatrix4 complete=rot*tr;
  
  AGVector4 p0(pLine.getV0(),1);
  AGVector4 p1(pLine.getV1(),1);
  
  p0=complete*p0;
  p1=complete*p1;
  return mData->lineHit(AGLine3(AGVector3(p0[0],p0[1],p0[2]),
				AGVector3(p1[0],p1[1],p1[2])))+AGVector4(mPos.dim3(),0);
}


void Mesh::setPos(const AGVector3&pPos)
{
  mPos=AGVector4(pPos,1);
}
void Mesh::setRotation(float r)
{
  mRotation=r;
}


Mesh *toMesh(SceneNode *node)
{
  return dynamic_cast<Mesh*>(node);
}
