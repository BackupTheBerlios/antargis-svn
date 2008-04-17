#ifndef ANT_STATIC_TRANSFORM
#define ANT_STATIC_TRANSFORM

#include "ag_geometry.h"

class AntRigidTransform
  {
    AGMatrix4 mMatrix;

public:
    AntRigidTransform(const AGVector3&pPos,const AGVector3 &pTo,const AGVector3 &pUp);
    void setup();
  };

#endif
