/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_application.cc
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

#include "ag_application.h"
#include "ag_main.h"
#include "ag_screen.h"
#include "ag_glscreen.h"
#include "ag_main.h"
#include "ag_debug.h"
#include "ag_mixer.h"
#include "ag_texturecache.h"
#include "ag_profiler.h"

#include <ruby.h>

void disableKeyrepeat()
{
  SDL_EnableKeyRepeat(0,0);
}

AGApplication *gApplication=0;

AGVector2 gAppCursorPos;

AGApplication *getApplication()
{
  return gApplication;
}


AGApplication::AGApplication():mRunning(true),mIdleCalls(true),mainWidget(0),mTooltip(0),mOverlay(0)
{
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
  if(videoInited())
    {
      setCursor(getTextureCache()->get("blue_cursor.png"));
      setNormalCursor();
    }
  mDemoTime=-1;
}

AGApplication::~AGApplication()
{
  CTRACE;
  delete mCursor;
}

void AGApplication::setKeyRepeat(bool enable)
{
  if(enable)
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
  else
    SDL_EnableKeyRepeat(0,0);
}


void AGApplication::setMainWidget(AGWidget *w)
{
  mainWidget=w;
  setOverlay(0);
  if(w)
    w->redraw();
}

AGWidget *AGApplication::getMainWidget()
{
  return mainWidget;
}

/**
 * \brief the main-loop of each application
 *
 * Each program has one or more application-objects.
 * An application normally represents a single UI-screen. So it has
 */

bool AGApplication::run() 
{
  STACKTRACE; 
  Uint32 last,now;
  SDL_Event event;
  float t;
  mRunning=true;

  gApplication=this;
  
  flushEventQueue();
  last=now=SDL_GetTicks();

  size_t loopCount=0;
  
  while(mRunning)
    {
      STACKTRACE; 
      
      gApplication=this;

      {
	// event handling
	STACKTRACE;
	// check for finished music
	getSoundManager()->checkFinished();
	
	now=SDL_GetTicks();
	/*
	// pull motion events (may flood the eventqueue)
		while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK) > 0)
	  ;
	*/
	clearOldMousePosition();
	dbout(2,"loop pre-event:"<<loopCount);
	event=getNewEvent();
	if(eventOk(event))
	  {
	    do
	      {
		dbout(2,"eventok  "<<toString(&event));
		doEvent(event);
		if(mIdleCalls)
		  {
		    dbout(2,"getNewEvent...  (idlecalls:"<<mIdleCalls<<")");
		    event=getNewEvent();
		  }
		else
		  resetEvent(event);
	      }while(eventOk(event));
	  } 
	dbout(2,"loop post-event:"<<loopCount);
	/*
	if(mIdleCalls) 
	  {
	    if (SDL_PollEvent(&event) == 0) 
	      eventIdle();
	    else
	      {
		do
		  {
		    doEvent(&event);
		  }while(SDL_PollEvent(&event)!=0);
	      }
	    
	  } 
	else 
	  {
	    SDL_WaitEvent(&event);
	    doEvent(&event);
	    }*/

	if(mDemoTime>=0)
	  {
	    t=mDemoTime;
	    mDemoTime=-1;
	    dbout(2,"demo time:"<<t);
	  }
	else
	  {
	    t=(now-last)/1000.0;
	  }
	if(mainWidget)
	  mainWidget->sigTick(t);
	
	dbout(2,"frame events:"<<t);
	eventPrepareFrame(t);

	eventFrame(t);
      }
      {
	// drawing
	STACKTRACE;
	{
	  STACKTRACE;
	  prepareDraw();
	}
	{
	  STACKTRACE;
	  draw();
	}
      }
      
      eventFrameEnd(t);
      last=now;

      loopCount++;
      dbout(2,"Running:"<<mRunning);
    }
  gApplication=0;

  return true;
}

void AGApplication::setDemoTime(float t)
{
  dbout(2,"demoTime:"<<t);
  mDemoTime=t;
}


SDL_Event AGApplication::getNewEvent()
{
  SDL_Event mEvent;
  resetEvent(mEvent);
  // pull motion events (may flood the eventqueue)
  while(SDL_PeepEvents(&mEvent, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK) > 0)
    ;
  
  if(mIdleCalls) 
    {
      if (SDL_PollEvent(&mEvent) == 0) 
	{
	  resetEvent(mEvent);
	  return mEvent;
	}
    } 
  else 
    SDL_WaitEvent(&mEvent);
  return mEvent;
}



bool AGApplication::doEvent(const SDL_Event &event) 
{
  STACKTRACE;
  SDL_Event e;
  
  // eat up old mouse motion events
  while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK) > 0);
  
  AGEvent *message=newEvent(this,"",event);
  bool processed=false;
  if(mOverlay)
    processed=mOverlay->processEvent(message);

  if(!processed)
    {
      if(!processed && mainWidget)
	processed=mainWidget->processEvent(message);

      if(!processed)
	processed=processEvent(message);
    }
  
  delete message;
  return processed;
}

void AGApplication::clearOldMousePosition()
{
}
void AGApplication::drawCursor()
{
  if(!videoInited())
    return;
  AGPainter p;
  if(mCursor)
    {
      mCursorOld=AGRect2(gAppCursorPos[0],gAppCursorPos[1],mCursor->width(),mCursor->height());
      p.blit(*mCursor,mCursorOld);
    }

}

void AGApplication::flushEventQueue()
{
}
 
bool AGApplication::eventQuit(AGEvent *m)
{
  CTRACE;
  mRunning=false;
  return false;
}

bool AGApplication::eventIdle()
{
  return false;
}


/**
   \brief prepares the actual drawing process

   prepareDraw runs prepareDraw for the main-widget, which itself runs
   prepareDraw recursively. This functions are meant to prepare possible
   texture contents and other things, that might be updated in each frame.
*/
void AGApplication::prepareDraw()
{
  STACKTRACE; 

  if(mainWidget)
    {
      mainWidget->prepareDrawAll();
      mainWidget->useTexturesRecursive();
    }
  if(mOverlay)
    mOverlay->prepareDrawAll();
}  


AGWidget *pLastDrawn=0;

/**
 * \brief query redraw
 *
 * Queries a complete screen redraw. This makes sense in SDL-mode only.
 * There a dirty rectangle kind of painting is used.
 */

void AGApplication::redraw()
{
  pLastDrawn=0;

  if(mainWidget)
    mainWidget->redraw();
}

/**
 * \brief calls the draw-function of the main-widget

 * AGApplication::draw() calls the draw-function of the mainWidget and cares
 * about drawing a custom mouse-cursor, the tooltip and possible overlays.
 * At the end the screen-buffer is flipped.
 */

void AGApplication::draw()
{
  if(delCue.size()>0)
    {
      for(std::list<AGWidget*>::iterator i=delCue.begin();i!=delCue.end();i++)
	if(*i)
	  saveDelete(*i);
      delCue.clear();
    }

  if(!videoInited())
    return;

  bool oldClippingTechnique=false;

  STACKTRACE;
  beginRender();
  if(mainWidget)
    {
      getScreen().begin();
      AGPainter p;
      if(pLastDrawn==mainWidget && !opengl())
	{
	  if(oldClippingTechnique)
	    {
	      AGRect2 r=mainWidget->getChangeRect();
	      if(mCursor)
		r+=mCursorOld;
	      
	      p.clip(r);
	    }
	  else
	    {
	      // FIXME: do some advanced clipping
	    }
	}
      mainWidget->drawAll(p);

      if(mTooltip)
	{
	  AGPainter p;
	  mTooltip->drawAll(p);
	}
      if(mOverlay)
	{
	  AGPainter p;
	  mOverlay->drawAll(p);
	}

      pLastDrawn=mainWidget;
    }
  drawCursor();

  std::list<AGRect2> changeList;
  if(mainWidget)
    {
      changeList=mainWidget->aquireChanges();
      mainWidget->clearChangeRects();
    }
  if(changeList.size())
    getScreen().update(changeList);
  else
    getScreen().flip();
  endRender();
  cdebug("end render");
}

/**
 * \brief quit the application
 *
 * This sets the private mRunning to false. One might add a function, that
 * checks if quitting is ok or reset mRunning to true. But this is not yet
 * part of AGApplication.
 */

void AGApplication::tryQuit()
{
  CTRACE;
  mRunning=false;
}

bool AGApplication::eventKeyDown(AGEvent *m)
{
  if(m->isSDLEvent())
    {
      SDLKey k=m->getKey();
      SDLMod mod=m->getMod();

      if(k==SDLK_RETURN && ((mod&KMOD_LALT)||(mod&KMOD_RALT)))
	{
	  getMain()->toggleFull();
	  eventChangedRes();
	  redraw();
	}
      else if(k==SDLK_F10)
	tryQuit();
    }
  return false;
}

void AGApplication::eventChangedRes()
{
}

bool AGApplication::eventPrepareFrame(float pTime)
{
  return false;
}


bool AGApplication::eventFrame(float pTime)
{
  return false;
}
bool AGApplication::eventFrameEnd(float pTime)
{
  return false;
}

long AGApplication::getTicks() const
{
  return SDL_GetTicks();
}

/// delays execution for ms milliseconds. This can be used to decrease framerate and cpu-load.
void AGApplication::delay(int ms)
{
  dbout(2,"delay:"<<ms);
  SDL_Delay(ms);
}


/// mark my mainWidget and my tooltip, as they can be ruby-objects
void AGApplication::mark()
{
  //  CTRACE;
  if(mainWidget)
    markObject(mainWidget);
  if(mTooltip)
    markObject(mTooltip);
}

/// this function sets the current tooltip, which is display above all widgets
void AGApplication::setTooltip(AGTooltip *pTooltip)
{
  delete mTooltip;
  mTooltip=pTooltip;
    
}

/// this functions resets the tooltip pTooltip.
/// @param pTooltip a tooltip of a widget
void AGApplication::resetTooltip(AGTooltip *pTooltip)
{
  if(pTooltip==mTooltip)
    {
      delete mTooltip;
      mTooltip=0;
    }
}

AGWidget *AGApplication::getOverlay()
{
  return mOverlay;
}


void AGApplication::setOverlay(AGWidget *pOverlay)
{
  CTRACE;
  if(mOverlay)
    delCue.push_back(mOverlay);
  mOverlay=pOverlay;
}


bool AGApplication::eventMouseMotion(AGEvent *m)
{
  gAppCursorPos=m->getMousePosition();
  return AGMessageObject::eventMouseMotion(m);
}

/// this function disables hardware cursor and shows software-cursor instead
void AGApplication::setCursor(const AGTexture &pTexture)
{
  mCursor=new AGTexture(pTexture);
  SDL_ShowCursor(0);
}

/// this function enables hardware-cursor
void AGApplication::setNormalCursor()
{
  SDL_ShowCursor(1);
  delete mCursor;
  mCursor=0;
}

bool AGApplication::hardwareCursor() const
{
  return mCursor==0;
}
