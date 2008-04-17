#ifndef ANT_QUATERNION_H
#define ANT_QUATERNION_H

#include "ag_geometry.h"

class AntQuaternion
  {
    float w,x,y,z;
    float buf[4];
public:
  AntQuaternion();
  /// from axis-angle
  AntQuaternion(const AGVector3 &pAxis,float pRotation);
  /// from euler
  AntQuaternion(float rx,float ry,float rz);
  /// direct
  AntQuaternion(float pW,float pX,float pY,float pZ);
  
  AGMatrix4 rotationMatrix();
  
  AntQuaternion operator*(const AntQuaternion &q) const;
  
  std::vector<float> angleAxis() const;
  
  };

#endif
