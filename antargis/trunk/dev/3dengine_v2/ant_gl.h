#ifndef ANT_GL_H
#define ANT_GL_H

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
