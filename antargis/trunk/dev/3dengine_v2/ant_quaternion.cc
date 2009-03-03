#include "ant_quaternion.h"

#include <cmath>

// no rotation
AntQuaternion::AntQuaternion()
  {
    w=1;
    x=y=z=0;
  }

AntQuaternion::AntQuaternion(const AGVector3 &pAxis,float pRotation)
  {
    float c=cos(pRotation/2);
    float s=sin(pRotation/2);
    w   =   c;
    x   =   pAxis[0] * s;
    y   =   pAxis[1] * s;
    z   =   pAxis[2] * s;
  }

AntQuaternion::AntQuaternion(float rx,float ry,float rz)
  {
    rx/=2;
    ry/=2;
    rz/=2;
    (*this)=AntQuaternion(cos(rx),sin(rx),0,0)*
    AntQuaternion(cos(ry),0,sin(ry),0)*
    AntQuaternion(cos(rz),0,0,sin(rz));
  }

AntQuaternion::AntQuaternion(float pW,float pX,float pY,float pZ):
  w(pW),
  x(pX),
  y(pY),
  z(pZ)
        {
        }



AntQuaternion AntQuaternion::operator*(const AntQuaternion &q) const
{
  return AntQuaternion(
      w*q.w - x*q.x - y*q.y - z*q.z,
      w*q.x + x*q.w + y*q.z - z*q.y,
      w*q.y + y*q.w + z*q.x - x*q.z,
      w*q.z + z*q.w + x*q.y - y*q.x);
}

std::vector<float> AntQuaternion::angleAxis() const
{
  std::vector<float> v;
  float scale = sqrt (x*x + y*y + z*z);
  v.push_back(2 * acos(w)); // angle
  if(scale==0)
    {
      v.push_back(0);
      v.push_back(0);
      v.push_back(0);
    }
  else
    {
      v.push_back(x / scale); // x
      v.push_back(y / scale); // y
      v.push_back(z / scale); // z
    }
  return v;
}
