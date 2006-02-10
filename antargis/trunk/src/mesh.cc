#include "mesh.h"

#include <map>
#include <vector>
#include <math.h>
#include <ag_debug.h>
#include <ag_texturecache.h>




//////////////////////////////////////////////////////////////////////
// Mesh
//////////////////////////////////////////////////////////////////////

Mesh::Mesh()
{
  mData=0;
  mRotation=0;
  setOrder(MESH_Z);
  mColor=AGVector4(1,1,1,1);
  mVisible=true;
}

Mesh::Mesh(MeshData &data,const AGVector4 &pPos,float pRot)
{
  mData=&data;
  mPos=pPos;
  mRotation=pRot;
  setOrder(MESH_Z);
  mColor=AGVector4(1,1,1,1);
  mVisible=true;
}

Mesh::~Mesh()
{
  //  TRACE;
}

void Mesh::draw()
{
  if(!mVisible)
    return;

  /*  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  glColor4fv(mColor);*/
  begin();
  if(mData)
    mData->draw(mColor);
  end();
}
void Mesh::drawDepth()
{
  if(!mVisible)
    return;
  begin();
  if(mData)
    mData->drawDepth();
  end();
}
void Mesh::drawShadow()
{
  if(!mVisible)
    return;
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
  getScene()->prepareUpdate(this);
  mPos=AGVector4(pPos,1);
  getScene()->updatePos(this);
}
void Mesh::setRotation(float r)
{
  mRotation=r;
}

MeshData *Mesh::getData()
{
  return mData;
}

AGBox3 Mesh::bbox() const
{
  // FIXME: ignore rotation for now
  return mData->bbox()+mPos.dim3();
}

void Mesh::drawPick()
{
  if(!mVisible)
    return;
  begin();

  if(mData)
    mData->drawPick();
  end();
}


Mesh *toMesh(SceneNode *node)
{
  return dynamic_cast<Mesh*>(node);
}


bool Mesh::transparent()
{
  assert(mData);
  return mData->transparent();
}


void Mesh::mark()
{
  markObject(getData());
}


void Mesh::setColor(const AGVector4 &pColor)
{
  mColor=pColor;
}
void Mesh::setVisible(bool v)
{
  mVisible=v;
}

