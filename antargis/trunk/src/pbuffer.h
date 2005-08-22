#ifndef __pbuffer_h
#define __pbuffer_h

#include <SDL_opengl.h>

#define GLX_GLXEXT_PROTOTYPES

#include <GL/glx.h>
#include <GL/glxext.h>

class PBuffer
{
  Display *mDisplay;
  GLXPbuffer mPBuffer;
  GLXContext glxContextWin,glxContextBuf;

 public:
  PBuffer(int w,int h);
  ~PBuffer();
  void select();
};


#endif
