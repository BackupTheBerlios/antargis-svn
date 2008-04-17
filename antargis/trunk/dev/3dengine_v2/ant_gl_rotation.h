#ifndef ANT_GL_ROTATION_H
#define ANT_GL_ROTATION_H

#include "ant_quaternion.h"

class AntGLRotation
  {
public:
  AntGLRotation(const AntQuaternion &pRotation);
  ~AntGLRotation();
  };

#endif
