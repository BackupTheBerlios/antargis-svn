#ifndef AG_PROJECTION
#define AG_PROJECTION

#include <ag_base.h>
#include <ag_geometry.h>


class AGEXPORT AGProjection2D
  {
public:
  AGProjection2D();
  AGProjection2D(const AGProjection2D &p);
  AGProjection2D(const AGRect2 &from, const AGRect2 &to);
  AGProjection2D(const AGMatrix3 &pMatrix);

  AGRect2 project(const AGRect2 &r) const;
  AGVector2 project(const AGVector2 &p) const;
  
  AGProjection2D inverse() const;

  void pushProjection(const AGProjection2D &p);
private:
  bool mInited;
  AGMatrix3 m;
  };




#endif
