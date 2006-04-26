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
#include "ag_screen.h"
#include "ag_glsurface.h"
#include "ag_main.h"
#include "ag_debug.h"
#include "ag_mixer.h"
#include "ag_texturecache.h"

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


AGApplication::AGApplication():mRunning(true),mIdleCalls(true),mainWidget(0),mTooltip(0)

{
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
  setCursor(getTextureCache()->get("blue_cursor.png"));
  setNormalCursor();
}

AGApplication::~AGApplication()
{
  CTRACE;
  delete mCursor;
}


void AGApplication::setMainWidget(AGWidget *w)
{
  mainWidget=w;
}


bool AGApplication::run() 
{
  Uint32 last,now;
  SDL_Event event;
  mRunning=true;

  gApplication=this;
  
  flushEventQueue();
  last=now=SDL_GetTicks();
  
  while(mRunning)
    {
      gApplication=this;

      // check for finished music
      getSoundManager()->checkFinished();

      //                 TRACE;
      now=SDL_GetTicks();
      //      TRACE;
    
      // pull motion events (may flood the eventqueue)
      while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK) > 0)
	;

      clearOldMousePosition();
      
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
	}
      float t=(now-last)/1000.0;
      if(mainWidget)
	mainWidget->sigTick(t);

      eventFrame(t);
      {
	prepareDraw();
	draw();
      }
      
      eventFrameEnd(t);
      last=now;
    }
  gApplication=0;

  return true;
}


bool AGApplication::doEvent(const SDL_Event* event) 
{
  SDL_Event e;
  
  // eat up old mouse motion events
  while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK) > 0);
  
  AGEvent *message=newEvent(this,"",event);
  bool processed=false;
  if(mainWidget)
    processed=mainWidget->processEvent(message);
  if(!processed)
    processed=processEvent(message);

  delete message;
  return processed;
}

void AGApplication::clearOldMousePosition()
{
}
void AGApplication::drawCursor()
{
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
  mRunning=false;
  return false;
}

bool AGApplication::eventIdle()
{
  return false;
}


void AGApplication::prepareDraw()
{
  if(mainWidget)
    {
      mainWidget->prepareDraw();
      mainWidget->useTexturesRecursive();
    }
}  


AGWidget *pLastDrawn=0;

void AGApplication::draw()
{
  beginRender();
  if(mainWidget)
    {
      getScreen().begin();
      AGPainter p;
      if(pLastDrawn==mainWidget && !opengl())
	{
	  AGRect2 r=mainWidget->getChangeRect();
	  r+=mCursorOld;
	  //	  cdebug(r);
	  p.clip(r);
	  //	  p.fillRect(AGRect2(0,0,getMain()->width(),getMain()->height()),AGColor(0,0,0));
	  //	  getScreen().flip();
	  //	  delay(500);
	}
      mainWidget->drawAll(p);

      if(mTooltip)
	{
	  AGPainter p;
	  mTooltip->drawAll(p);
	}

      pLastDrawn=mainWidget;
    }
  drawCursor();
  getScreen().flip();
  endRender();
}

void AGApplication::tryQuit()
{
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
	}
      else if(k==SDLK_F10)
	tryQuit();
    }
  return false;
}

void AGApplication::eventChangedRes()
{
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
