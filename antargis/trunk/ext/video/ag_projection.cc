#include "ag_projection.h"
#include "ag_debug.h"
#include "ag_algebra.h"


AGProjection2D::AGProjection2D():mInited(false)
    {
    }

AGProjection2D::AGProjection2D(const AGMatrix3 &pMatrix):mInited(true),m(pMatrix)
    {
      assert(isInvertable(m));
    }


AGProjection2D::AGProjection2D(const AGProjection2D &p):mInited(p.mInited),m(p.m)
    {
    }

AGProjection2D::AGProjection2D(const AGRect2 &from, const AGRect2 &to):
  mInited(true)
    {
      assert(from.content()>0 && to.content()>0);

      float sx=to.w()/from.w();
      float sy=to.h()/from.h();
      AGMatrix3 m1(-from.getV0());
      AGMatrix3 m2(sx,sy);
      AGMatrix3 m3(to.getV0());

      m=m3*m2*m1;
    }

AGRect2 AGProjection2D::project(const AGRect2 &r) const
  {
    assert(mInited);
    return AGRect2((m*r.getV0()).dim2(),(m*r.getV1()).dim2());
  }
AGVector2 AGProjection2D::project(const AGVector2 &p) const
  {
    assert(mInited);
    return (m*p).dim2();
  }

void AGProjection2D::pushProjection(const AGProjection2D &p)
  {
    assert(mInited);
    m*=p.m;
  }

AGProjection2D AGProjection2D::inverse() const
{
  assert(isInvertable(m));
  return AGProjection2D(m.inverted());
}
