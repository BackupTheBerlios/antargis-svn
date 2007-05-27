#include "ag_projection.h"
#include "ag_debug.h"


AGProjection2D::AGProjection2D(const AGRect2 &from, const AGRect2 &to)
{
  assert(from.content()>0 && to.content()>0);

  float sx=to.w()/from.w();
  float sy=to.h()/from.h();
  AGMatrix3 m1(-from.getV0());
  AGMatrix3 m2(sx,sy);
  AGMatrix3 m3(to.getV0());

  m=m3*m2*m1;
  /*  cdebug("m1:"<<m1.toString());
  cdebug("m2:"<<m2.toString());
  cdebug("m3:"<<m3.toString());
  

  cdebug("sx:"<<sx);
  cdebug("sy:"<<sy);
  cdebug("M:"<<m.toString());*/
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
