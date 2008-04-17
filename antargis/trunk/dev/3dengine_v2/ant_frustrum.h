#ifndef ANT_FRUSTRUM
#define ANT_FRUSTRUM

#include "ag_geometry.h"

class AntFrustrum
  {
    AGMatrix4 mFrustrum;
public:
  AntFrustrum(float openAngle,float aspect,float pNear,float pFar);
  
  void select();
  };

#endif
