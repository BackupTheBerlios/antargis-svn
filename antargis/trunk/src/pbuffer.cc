#include "pbuffer.h"
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

//#include <X11/Xlib.h>

PBuffer::PBuffer(int width,int height)
    {

      glxContextWin=glXGetCurrentContext ();

   int scrnum;
   GLXFBConfig *fbconfig;
   XVisualInfo *visinfo;
   int nitems;

   int attrib[] = {
      GLX_DOUBLEBUFFER,  False,
      GLX_RED_SIZE,      1,
      GLX_GREEN_SIZE,    1,
      GLX_BLUE_SIZE,     1,
      GLX_DEPTH_SIZE,    1,
      GLX_RENDER_TYPE,   GLX_RGBA_BIT,
      GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT | GLX_WINDOW_BIT,
      None
   };
   int pbufAttrib[] = {
      GLX_PBUFFER_WIDTH,   width,
      GLX_PBUFFER_HEIGHT,  height,
      GLX_LARGEST_PBUFFER, False,
      None
   };


      mDisplay = glXGetCurrentDisplay();
   scrnum   = DefaultScreen(mDisplay);

   fbconfig = glXChooseFBConfig(mDisplay,
                                scrnum,
                                attrib,
                                &nitems);
   if (NULL == fbconfig) {
      fprintf(stderr,"Error: couldn't get fbconfig\n");
      exit(1);
   }

   mPBuffer = glXCreatePbuffer(mDisplay, fbconfig[0], pbufAttrib);

   visinfo = glXGetVisualFromFBConfig(mDisplay, fbconfig[0]);
   if (!visinfo) {
      fprintf(stderr, "Error: couldn't get an RGBA, double-buffered visual\n");
      exit(1);
   }

   glxContextBuf = glXCreateContext(mDisplay, visinfo, glxContextWin, GL_TRUE );
   if (!glxContextBuf) {
      fprintf(stderr, "Error: glXCreateContext failed\n");
      exit(1);
   }

   XFree(fbconfig);
   XFree(visinfo);
}

PBuffer::~PBuffer()
{
  glXDestroyContext(mDisplay, glxContextBuf);
  glXDestroyPbuffer(mDisplay, mPBuffer);
}



