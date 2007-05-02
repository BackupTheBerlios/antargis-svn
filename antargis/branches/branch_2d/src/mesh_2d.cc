#include "mesh_2d.h"
#include <ag_debug.h>
#include <ag_painter.h>

Mesh2D::Mesh2D(Scene2D *pScene):
  SceneNode(pScene,AGVector4(),AGBox3())
{
}
Mesh2D::Mesh2D(Scene2D *pScene,Mesh2DData &data,const AGVector4 &pPos,float pRot):
  SceneNode(pScene,pPos,data.bbox()),
  mData(&data)
{
}
Mesh2D::~Mesh2D()
{
}

void Mesh2D::draw()
{
  //FIXME: drawing
  Scene2D *s=dynamic_cast<Scene2D*>(getScene());
  assert(s);
  AGPainter *painter=s->getPainter();

  assert(painter);
  AGTexture *t=mData->getTexture();
  AGVector2 middle(s->getPosition(getPos()));
  float w=t->width();
  float h=t->height();
  AGRect2 r(middle.getX()-w/2,middle.getY()-h/2,w,h);

  //  cdebug("r:"<<r);
  painter->blit(*t,r);
}

AGVector4 Mesh2D::lineHit(const AGLine3 &pLine) const
{
  //FIXME
  return AGVector4();
}

/// set rotation around y-axis (e.g. turn people)
void Mesh2D::setRotation(float r)
{
}
/// set color (for rings)
void Mesh2D::setColor(const AGVector4 &pColor)
{
}
void Mesh2D::setVisible(bool v)
{
}

Mesh2DData *Mesh2D::getData()
{
  return mData;
}

void Mesh2D::mark()
{
  markObject(mData);
}
