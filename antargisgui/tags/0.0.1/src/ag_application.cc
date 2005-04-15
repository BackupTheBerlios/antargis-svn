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
#include "ag_surface.h"
#include "ag_main.h"
#include "ag_debug.h"

AGApplication::AGApplication():mRunning(true),mIdleCalls(true),mainWidget(0)

{
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
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
  
  flushEventQueue();
  last=now=SDL_GetTicks();
  
  while(mRunning)
    {
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
      eventFrame((now-last)/1000.0);
{
  //      TRACE;
      draw();
      }
      
      drawCursor();
      last=now;
    }
  return true;
}


bool AGApplication::doEvent(const SDL_Event* event) 
{
  //  TRACE;
  SDL_Event e;
  
  // eat up old mouse motion events
  while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK) > 0);
  
  AGEvent *message=newEvent(this,event);
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
}

void AGApplication::flushEventQueue()
{
}
 
bool AGApplication::eventQuit(const AGEvent *m)
{
  mRunning=false;
  return false;
}

bool AGApplication::eventIdle()
{
  return false;
}

void AGApplication::draw()
{
{
  //  CTRACE;
  if(mainWidget)
    mainWidget->drawAll(getScreen().getRect());
    }
    {
      //    CTRACE;
  getScreen().flip();
}
  //  SDL_Flip(getScreen().surface());
}

void AGApplication::tryQuit()
{
  mRunning=false;
  
}

bool AGApplication::eventKeyDown(const AGEvent *m2)
{
  const AGSDLEvent *m=reinterpret_cast<const AGSDLEvent*>(m2);
  if(m)
    {
      SDLKey k=m->getKey();
      SDLMod mod=m->getMod();
      //      char ins=0;
      //      bool doInsert=false;
      if(k==SDLK_RETURN && ((mod&KMOD_LALT)||(mod&KMOD_RALT)))
	getMain()->toggleFull();
      else if(k==SDLK_ESCAPE)
	tryQuit();
    }
  return false;
}

bool AGApplication::eventFrame(float pTime)
{
  return false;
}
