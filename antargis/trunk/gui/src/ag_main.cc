/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_main.cc
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "Antargis" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */

#include <iostream>

#include "ag_tools.h"
#include "ag_main.h"
#include "ag_surface.h"
#include "ag_texture.h"
#include "ag_sdlsurface.h"
#include "ag_glsurface.h"
#include "ag_fs.h"
#include "ag_debug.h"
#include "ag_surfacemanager.h"
#include "privates.h"
#include <SDL.h>
#include "SDL_image.h"
#include <SDL_mixer.h>

int lastWidth=0;
int lastHeight=0;
int lastDepth=0;
bool fullScreen=false;
bool lastGL=false;

AGMain *mAGMain=0;

bool quited=false;

// singleton-management
void newInstanceKiller();
void deleteInstanceKiller();

bool hasQuit()
{
  return quited;
}

/**
   creates an AGMain object.
   @param pw screen-width
   @param ph screen height.
   @param pd color depth in bits - typically 32
   @param fs bool fullscreen. true for fullscreen, false for window
   @param opengl use opengl or not
*/
AGMain::AGMain(int pw,int ph,int pd,bool fs,bool openGL)
{
  assert(mAGMain==0);
  mAGMain=this;

  newInstanceKiller();

  int w=pw;
  int h=ph;

  // Initialize SDL
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO)<0)
    {
      cdebug("SDL could not be initialized!");
      exit(1);
    }

  videoInfo = SDL_GetVideoInfo();
  if(!videoInfo)
    {
      cdebug("SDL could not get video-info");
      exit(1);
    }

  int videoFlags=0;

  if(openGL)
    videoFlags|=SDL_OPENGL;
  else
    videoFlags|=SDL_SWSURFACE;

  if(fs)
    videoFlags|=SDL_FULLSCREEN;

  lastWidth=w;
  lastHeight=h;
  lastDepth=pd;
  fullScreen=fs;
  lastGL=openGL;

  // set video mode

  SDL_Surface *ms=SDL_SetVideoMode(w,h,videoInfo->vfmt->BitsPerPixel,videoFlags);

  if(openGL)
    setScreen(mScreen=new AGGLScreen(w,h));
  else
    setScreen(mScreen=new AGSDLScreen(ms));
  //atexit(SDL_Quit);

  initFS("");
  // FIXME: set $main in ruby to this ???
  
}

extern std::map<std::pair<AGFont,std::string>,AGTexture> fontCache;
extern std::map<std::string,Mix_Chunk*> mSounds;

AGMain::~AGMain()
{
  fontCache.clear();
  mSounds.clear();
  CTRACE;
  delete mScreen;
  deleteInstanceKiller();
  mAGMain=0;
  cdebug("QUIT");
  SDL_Quit();
  quited=true;
}

void AGMain::flip()
{
  getScreen().flip();
}

void AGMain::changeRes(int w,int h,int d,bool fs,bool gl)
{
  fontCache.clear();
  getSurfaceManager()->clear();

  lastWidth=w;
  lastHeight=h;
  lastDepth=d;
  fullScreen=fs;

  if(!videoInfo)
    {
      throw int();
      videoInfo = SDL_GetVideoInfo();
      if(!videoInfo)
	{
	  std::cerr<<"SDL could not get video-info"<<std::endl;
	  exit(1);
	}
    }
  int videoFlags=0;

  if(gl)
    {
      videoFlags|=SDL_OPENGL;
      cdebug("initing opengl");
    }
  else
    videoFlags |= SDL_SWSURFACE;

  if(fs)
    videoFlags|=SDL_FULLSCREEN;

  // set video mode
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface *ms=SDL_SetVideoMode(w,h,videoInfo->vfmt->BitsPerPixel,videoFlags);
  if(mScreen)
    delete mScreen;
  if(gl)
    setScreen(mScreen=new AGGLScreen(w,h));
  else
    setScreen(mScreen=new AGSDLScreen(ms));

  SDL_WM_SetCaption("Antargis","Antargis");
  
}

void AGMain::toggleFull()
{
  changeRes(lastWidth,lastHeight,lastDepth,!fullScreen,lastGL);
}

AGMain *getMain()
{
  if(!mAGMain)
    {
      std::cerr<<"AGMain not initialized!"<<std::endl;
      assert(0==1);
      throw std::runtime_error("AGMain not initialized");
      exit(1);
    }
  return mAGMain;
}

int AGMain::width() const
{
  return lastWidth;
}
int AGMain::height() const
{
  return lastHeight;
}

void AGMain::setIcon(const std::string &pFile)
{
  std::string file=loadFile(pFile);
  
  SDL_Surface *s=IMG_Load_RW(SDL_RWFromMem(const_cast<char*>(file.c_str()),file.length()),false);
  SDL_WM_SetIcon(s,0);
}

void AGMain::setCaption(const std::string &pCaption)
{
  SDL_WM_SetCaption(pCaption.c_str(),0);
}

bool AGMain::fullscreen() const
{
  return fullScreen;
}

long AGMain::getTicks() const
{
  return SDL_GetTicks();
}


void AGMain::mark()
{
}
