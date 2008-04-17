#ifndef ANT_GL_H
#define ANT_GL_H

#include "GLee.h"
#ifdef GL_GLEXT_VERSION
#undef GL_GLEXT_VERSION
#endif
#include <SDL_opengl.h>

class AntGL
  {
    int mScreenwidth;
    int mScreenheight;
public:
  AntGL();
  void initVideoMode(int w,int h,bool fs=false);
  void swapBuffers();
  void clearBuffer();
  
  
  };


#endif
