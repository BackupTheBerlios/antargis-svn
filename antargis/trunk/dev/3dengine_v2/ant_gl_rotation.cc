#include "ant_gl_rotation.h"
#include "ant_gl.h"
#include "ag_debug.h"
#include <cmath>

AntGLRotation::AntGLRotation(const AntQuaternion &pRotation)
  {
    glPushMatrix();
    std::vector<float> v=pRotation.angleAxis();
    cdebug(v[0]);
    cdebug(v[1]);
    cdebug(v[2]);
    cdebug(v[3]);
    glRotatef(v[0]*180.0/M_PI,v[1],v[2],v[3]);
  }
AntGLRotation::~AntGLRotation()
  {
    glPopMatrix();
  }

