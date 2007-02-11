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

#include "ag_base.h"
#include "ag_debug.h"
#include "ag_tools.h"
#include "ag_main.h"
#include "ag_surface.h"
#include "ag_texture.h"
#include "ag_sdlsurface.h"
#include "ag_glscreen.h"
#include "ag_fs.h"
#include "ag_layout.h"
#include "ag_debug.h"
#include "ag_mixer.h"
#include "ag_surfacemanager.h"
#include "ag_fontengine.h"
#include "privates.h"
#include <SDL.h>
#include "SDL_image.h"
#include <SDL_mixer.h>

#include <ruby.h>

int lastWidth=0;
int lastHeight=0;
int lastVWidth=0;
int lastVHeight=0;
int lastDepth=0;
bool fullScreen=false;
bool lastGL=false;

AGMain *mAGMain=0;

bool quited=false;

// singleton-management
void newInstanceKiller();
void deleteInstanceKiller();

void initSoundEngine();

#ifdef DRMUSER
std::string gUserName=DRMUSER;
bool gDRMok=false;
#endif

bool hasQuit()
{
  return quited;
}

bool glMode()
{
  return lastGL;
}


/**
   creates an AGMain object.
*/
AGMain::AGMain():
  mCollector(0)
{
  assert(mAGMain==0);
  mAGMain=this;

  newInstanceKiller();

  // Initialize SDL
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE|SDL_INIT_AUDIO)<0)
    {
      cdebug("SDL could not be initialized!");
      exit(1);
    }
  SDL_EnableUNICODE(1);
		  
  videoInfo=0;

  initSoundEngine();

  initFS("");

  mScreen=0;
  AGLayout::registerLayouts();
}

extern std::map<std::string,Mix_Chunk*> mSounds;

AGMain::~AGMain()
{
  mSounds.clear();
  CTRACE;
  delete mScreen;
  deleteInstanceKiller();
  mAGMain=0;
  cdebug("QUIT");
  SDL_Quit();
  quited=true;


  setRubyRaising(true);
}

void AGMain::flip()
{
  getScreen().flip();
}

AGCollector *AGMain::getCollector()
{
  if(!mCollector)
    mCollector=new AGCollector;
  return mCollector;
}




std::string myHash(const std::string &p)
{
  // FIXME: TRY USING ruby's Digest::MD5::digest("xy")
  rb_eval_string("require 'digest/md5'");

  //  VALUE l=rb_gv_get("Digest::MD5");
  //  VALUE r=rb_funcall(l,rb_intern("digest"),1,rb_str_new2(p.c_str()));

  std::ostringstream os;
  os<<"s=''; Digest::MD5::digest('"<<p<<"').each_byte{|b|s+=sprintf('%X',b)};s";

  VALUE r=rb_eval_string(os.str().c_str());

  std::string s;

  s=STR2CSTR(r);

  std::cout<<"p:"<<p<<std::endl;
  std::cout<<"s:"<<s<<std::endl;

  return s;
}

/**
   initializes video mode
   @param pw screen-width
   @param ph screen height.
   @param pd color depth in bits - typically 32
   @param fs bool fullscreen. true for fullscreen, false for window
   @param opengl use opengl or not
*/
void AGMain::initVideo(int w,int h,int d,bool fs,bool gl,int vw,int vh)
{
  if(mScreen)
    {
      getSurfaceManager()->clear();
    }

  lastGL=gl;
  lastWidth=w;
  lastHeight=h;
  lastDepth=d;
  fullScreen=fs;

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

  if(!ms)
    {
      std::cerr<<"Initing video mode failed!"<<std::endl;
      exit(1);
    }


  if(mScreen)
    delete mScreen;

  if(vw<w)
    vw=w;
  if(vh<h)
    vh=h;

  lastVWidth=vw;
  lastVHeight=vh;

  cdebug("gl:"<<gl);

  if(gl)
    setScreen(mScreen=new AGGLScreen(w,h,vw,vh));
  else
    setScreen(mScreen=new AGSDLScreen(ms));

  cdebug("mscreen:"<<mScreen);

  SDL_WM_SetCaption("Antargis","Antargis");



#ifdef DRMMD5

  std::string mhash(DRMMD5);
  AGFont f("Arial.ttf",23);

  if(myHash(gUserName.substr(32,std::string::npos))==mhash.substr(32,std::string::npos))
    {
      std::cout<<"drm ok"<<std::endl;
      AGTexture *t=AGFontEngine::renderText(0,0,AGStringUtf8("Registriert für :"+gUserName.substr(32,std::string::npos)),f);
      getScreen().blit(*t,AGRect2(50,100,t->width(),t->height()),t->getRect());
      gDRMok=true;
      getScreen().flip();
      SDL_Delay(3000);
    }
  else
    {
      std::cout<<"drm falied"<<std::endl;
      AGTexture *t=AGFontEngine::renderText(0,0,AGStringUtf8("Registriert für :"+gUserName.substr(32,std::string::npos)),f);
      getScreen().blit(*t,AGRect2(50,100,t->width(),t->height()),t->getRect());
      //      getScreen().blit(*t);
      gDRMok=true;
      getScreen().flip();
      SDL_Delay(4000);
      exit(1);
    }

#endif








  
}

void AGMain::toggleFull()
{
  CTRACE;
  cdebug("lastGL:"<<lastGL);
  initVideo(lastWidth,lastHeight,lastDepth,!fullScreen,lastGL,lastVWidth,lastVHeight);
}

bool hasMain()
{
  return mAGMain;
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
  return lastVWidth;
}
int AGMain::height() const
{
  return lastVHeight;
}

int AGMain::realWidth() const
{
  return lastWidth;
}
int AGMain::realHeight() const
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
  markObject(getCollector());
}


std::string gAppName="Antargis";
void setAppName(const std::string &pName)
{
  gAppName=pName;
}
std::string getAppName()
{
  return gAppName;
}
