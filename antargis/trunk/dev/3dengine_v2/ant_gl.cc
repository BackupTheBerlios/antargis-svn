#include <iostream>

#include "ant_gl.h"
#include "ant_sdl.h"

#include <SDL.h>
#include <SDL_opengl.h>

AntGL::AntGL()
  {
    needsSDL();
    std::cout<<"ANTGL"<<std::endl;
    mScreenwidth=0;
    mScreenheight=0;
    std::cout<<"ANTGL"<<std::endl;
  }


void AntGL::initVideoMode(int w,int h,bool fs)
  {
    static const SDL_VideoInfo *videoInfo=0;
    if(!videoInfo)
      {
        videoInfo = SDL_GetVideoInfo();
        if(!videoInfo)
          {
            std::cerr<<"SDL could not get video-info"<<std::endl;
            exit(1);
          }
      }
    int videoFlags=0;

    videoFlags|=SDL_OPENGL;

    if(fs)
      videoFlags|=SDL_FULLSCREEN;

    SDL_Surface *ms=SDL_SetVideoMode(w,h,videoInfo->vfmt->BitsPerPixel,videoFlags);
    if(!ms)
      {
        std::cerr<<"Initing video mode failed!"<<std::endl;
        std::cerr<<"SDL:Error:"<<SDL_GetError()<<std::endl;
        std::cerr<<"If you experience errors here, it may be that one following is true:"<<std::endl;
        std::cerr<<"* SDL is compiled without OpenGL support"<<std::endl;
        std::cerr<<"* Your display has no support for GLX"<<std::endl;
        std::cerr<<"* You're running SDL with framebuffer - instead of X"<<std::endl;
        exit(1);
      }


    mScreenwidth=w;
    mScreenheight=h;


    SDL_WM_SetCaption("Antargis","Antargis");
    
    
    
    /*
    // Enable smooth shading 
    glShadeModel( GL_SMOOTH );

    // Set the background black 
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    // Depth buffer setup 
    glClearDepth( 1.0f );

    // Enables Depth Testing 
    glEnable( GL_DEPTH_TEST );

    // The Type Of Depth Test To Do 
    glDepthFunc( GL_LEQUAL );

    // Really Nice Perspective Calculations 
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    */
    
  }

void AntGL::clearBuffer()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }


void AntGL::swapBuffers()
  {
    SDL_GL_SwapBuffers();
  }
