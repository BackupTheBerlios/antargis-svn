#include <iostream>

#include "ag_main.h"
#include "ag_gsurface.h"
#include "ag_surface.h"
#include "ag_glsurface.h"
#include "privates.h"
#include <SDL.h>

#define FULLSCREEN false
#define SCREEN_DEPTH 32


int lastWidth=0;
int lastHeight=0;
int lastDepth=0;
bool openGL=true;
bool fullScreen=false;

AGMain *mAGMain=0;

AGMain::AGMain()
{
  SDL_Surface *mDisplay;

  mAGMain=this;

  int w=800;//640;
  int h=600;//480;

  // Initialize SDL
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO)<0)
    {
      std::cerr<<"SDL could not be initialized!"<<std::endl;
    }
  const SDL_VideoInfo *videoInfo;

  videoInfo = SDL_GetVideoInfo();
  if(!videoInfo)
    {
      std::cerr<<"SDL could not get video-info"<<std::endl;
    }

  int videoFlags=SDL_HWPALETTE;

  if(openGL)
    {
      videoFlags|=SDL_OPENGL;
      std::cerr<<"initing opengl"<<std::endl;
    }

  if(FULLSCREEN)
    videoFlags|=SDL_FULLSCREEN;

  if ( videoInfo->hw_available )
    {
      videoFlags |= SDL_HWSURFACE;
    }
  else
    {
      videoFlags |= SDL_SWSURFACE;
    }

  if ( videoInfo->blit_hw )
    {
      videoFlags |= SDL_HWACCEL;
    }

  lastWidth=w;
  lastHeight=h;
  lastDepth=SCREEN_DEPTH;
  fullScreen=FULLSCREEN;

  // set video mode

  SDL_Surface *ms=SDL_SetVideoMode(w,h,SCREEN_DEPTH,videoFlags);
  if(openGL)
    setScreen(new AGGLScreen(w,h));
  else
    setScreen(new AGSDLScreen(ms));
  //  setScreen(new AGScreen(mDisplay=SDL_SetVideoMode(w,h,SCREEN_DEPTH,
  //						   videoFlags)));

  if(!mDisplay)
    {
      std::cerr<<"Video mode could not be set!"<<std::endl;
    }

  atexit(SDL_Quit);
  //  initSDL_net();



}
AGMain::~AGMain()
{
}

void AGMain::flip()
{
  getScreen().flip();
  //  SDL_Flip(ScreenSurface);
}

void AGMain::changeRes(int w,int h,int d,bool fs,bool gl)
{
  const SDL_VideoInfo *videoInfo;
  lastWidth=w;
  lastHeight=h;
  lastDepth=d;
  fullScreen=fs;

  videoInfo = SDL_GetVideoInfo();
  if(!videoInfo)
    {
      std::cerr<<"SDL could not get video-info"<<std::endl;
    }

  int videoFlags=SDL_HWPALETTE;

  if(gl)
    {
      videoFlags|=SDL_OPENGL;
      std::cerr<<"initing opengl"<<std::endl;
    }

  openGL=gl;

  if(fs)
    videoFlags|=SDL_FULLSCREEN;

  if ( videoInfo->hw_available )
    {
      videoFlags |= SDL_HWSURFACE;
    }
  else
    {
      videoFlags |= SDL_SWSURFACE;
    }

  if ( videoInfo->blit_hw )
    {
      videoFlags |= SDL_HWACCEL;
    }


  // set video mode
  SDL_Surface *ms=SDL_SetVideoMode(w,h,SCREEN_DEPTH,videoFlags);
  if(openGL)
    setScreen(new AGGLScreen(w,h));
  else
    setScreen(new AGSDLScreen(ms));
  
}

void AGMain::toggleFull()
{
  changeRes(lastWidth,lastHeight,lastDepth,!fullScreen,openGL);
}

AGMain *getMain()
{
  return mAGMain;
}
