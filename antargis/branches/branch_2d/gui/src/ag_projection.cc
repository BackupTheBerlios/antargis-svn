#include "ag_projection.h"
#include "ag_debug.h"


AGProjection2D::AGProjection2D(const AGRect2 &from, const AGRect2 &to)
{
  assert(from.content()>0 && to.content()>0);

  float sx=to.w()/from.w();
  float sy=to.h()/from.h();

  m=AGMatrix3(-from.getV0())*AGMatrix3(sx,sy)*AGMatrix3(to.getV0());
}
  
AGRect2 AGProjection2D::project(const AGRect2 &r)
{
  return AGRect2((m*r.getV0()).dim2(),(m*r.getV1()).dim2());
}
AGVector2 AGProjection2D::project(const AGVector2 &p)
{
  return (m*p).dim2();
}

void AGProjection2D::pushProjection(const AGProjection2D &p)
{
  m*=p.m;
}
