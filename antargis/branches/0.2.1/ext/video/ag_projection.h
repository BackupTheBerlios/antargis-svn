#ifndef AG_PROJECTION
#define AG_PROJECTION

#include <ag_geometry.h>


class AGEXPORT AGProjection2D
{
  AGMatrix3 m;
 public:
  AGProjection2D(const AGRect2 &from, const AGRect2 &to);
  
  AGRect2 project(const AGRect2 &r);
  AGVector2 project(const AGVector2 &p);

  void pushProjection(const AGProjection2D &p);
};



#endif
