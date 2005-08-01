/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_messageobject.cc
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

#include "ag_messageobject.h"
#include "ag_debug.h"

// AGEvent
AGEvent::AGEvent(AGListener *pCaller):mCaller(pCaller)
{
}
AGEvent::~AGEvent()
{
}

AGListener *AGEvent::getCaller() const
{
  return mCaller;
}

// AGSDLEvent

AGSDLEvent::AGSDLEvent(AGListener *pCaller,const SDL_Event *pEvent):AGEvent(pCaller),mEvent(pEvent)
{
}
const SDL_Event *AGSDLEvent::get() const
{
  return mEvent;
}

AGPoint AGSDLEvent::getMousePosition() const
{
  AGPoint p;
  switch(mEvent->type) {
  case SDL_MOUSEMOTION:
    p.x=mEvent->motion.x;
    p.y=mEvent->motion.y;
    break;
    
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    p.x=mEvent->button.x;
    p.y=mEvent->button.y;
    break;
    
  }
  return p;
}

SDLKey AGSDLEvent::getKey() const
{
  return mEvent->key.keysym.sym;
}

SDLMod AGSDLEvent::getMod() const
{
  return mEvent->key.keysym.mod;
}

int AGSDLEvent::getButton() const
{
  switch(mEvent->type) {
  case SDL_MOUSEMOTION:
    return mEvent->motion.state;
    break;
    
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    return mEvent->button.button;
    break;
    
  }
  return 0;
}



// AGMouseEvent

AGMouseEvent::AGMouseEvent(AGListener *pCaller,SDL_Event *pEvent):AGEvent(pCaller)
{
}
AGMouseEvent::~AGMouseEvent()
{
}

// AGListener

AGListener::AGListener()
{
}

AGListener::~AGListener()
{
}

bool AGListener::signal(const std::string &pName,const AGEvent *m,AGMessageObject *pCaller)
{
  return false;
}

AGCPPListener::~AGCPPListener()
{
}



// AGSignal

AGSignal::AGSignal():mCaller(0)
{
}

//AGSignal::AGSignal(const AGSignal &s):mListeners(s.mListeners



AGSignal::AGSignal(AGMessageObject *pCaller):mCaller(pCaller)
{
}

AGSignal::AGSignal(AGMessageObject *pCaller,const std::string &pName):
  mName(pName),mCaller(pCaller)
{
}

AGSignal::~AGSignal()
{
  std::set<AGListener*>::iterator i=mListeners.begin();
  for(;i!=mListeners.end();i++)
    {
      AGMessageObject *o=dynamic_cast<AGMessageObject*>(*i);
      if(o)
	o->popSignal(this);
    }
}


void AGSignal::connect(AGListener &pListener)
{
  mListeners.insert(&pListener);
  AGMessageObject *o=dynamic_cast<AGMessageObject*>(&pListener);
  if(o)
    o->pushSignal(this);
}
void AGSignal::disconnect(AGListener &pListener)
{
  mListeners.erase(&pListener);
}

void AGSignal::connect(AGCPPListener *pListener)
{
  mSimpleListeners.insert(pListener);
}
void AGSignal::disconnect(AGCPPListener *pListener)
{
  mSimpleListeners.erase(pListener);
}

bool AGSignal::signal(const AGEvent *m)
{
  std::set<AGListener*>::iterator i=mListeners.begin();
  bool value=false;
  for(;i!=mListeners.end();i++)
    {
      if((*i)->signal(mName.c_str(),m,mCaller))
	value=true;
    }

  std::set<AGCPPListener*>::iterator j=mSimpleListeners.begin();
  for(;j!=mSimpleListeners.end();j++)
    {
      if((*j)->signal(mName.c_str(),m,mCaller))
	value=true;
    }

  return value;
}

bool AGSignal::operator()(const AGEvent *m)
{
  return signal(m);
}

// AGMessageObject


AGMessageObject *AGMessageObject::captureObject=0;


AGMessageObject::AGMessageObject():
  sigActive(this,"sigActive"),
  sigKeyDown(this,"sigKeyDown"),
  sigKeyUp(this,"sigKeyUp"),
  sigMouseMotion(this,"sigMouseMotion"),
  sigMouseButtonDown(this,"sigMouseButtonDown"),
  sigMouseButtonUp(this,"sigMouseButtonUp"),
  sigQuit(this,"sigQuit"),
  sigSysWM(this,"sigSysWM"),
  sigVideoResize(this,"sigVideoResize"),
  mCanReceiveMessages(true)
{
}

AGMessageObject::~AGMessageObject()
{
  std::set<AGSignal*> sigs=mSignals;
  std::set<AGSignal*>::iterator i=sigs.begin();
  for(;i!=sigs.end();i++)
    (*i)->disconnect(*this);
}

void AGMessageObject::pushSignal(AGSignal *pSignal)
{
  mSignals.insert(pSignal);
}

void AGMessageObject::popSignal(AGSignal *pSignal)
{
  mSignals.erase(pSignal);
}


// 
// exchange with a processEvent(const AGEvent &event)

bool AGMessageObject::processEvent(const AGEvent* pEvent) 
{
  //  cdebug("typeid:"<<typeid(this).name());
  //  TRACE;
  //  SDL_Event e;
  bool rc=false;

  const AGSDLEvent *agEvent=reinterpret_cast<const AGSDLEvent*>(pEvent);
  if(agEvent)
    {
      const SDL_Event *event=agEvent->get();
      switch(event->type) {
      case SDL_ACTIVEEVENT:
	rc = eventActive(agEvent) || sigActive(agEvent);
	break;
	
      case SDL_KEYDOWN:
	rc = eventKeyDown(agEvent) || sigKeyDown(agEvent);
    break;
    
  case SDL_KEYUP:
    rc = eventKeyUp(agEvent) || sigKeyUp(agEvent);
    break;
    
  case SDL_MOUSEMOTION:
    rc = eventMouseMotion(agEvent) || sigMouseMotion(agEvent);
    break;
    
  case SDL_MOUSEBUTTONDOWN:
    rc = eventMouseButtonDown(agEvent) || sigMouseButtonDown(agEvent);
    break;
    
  case SDL_MOUSEBUTTONUP:
    rc = eventMouseButtonUp(agEvent) || sigMouseButtonUp(agEvent);
    break;
    
  case SDL_QUIT:
    rc = eventQuit(agEvent) || sigQuit(agEvent);
    break;
    
  case SDL_SYSWMEVENT:
    rc = eventSysWM(agEvent) || sigSysWM(agEvent);
    break;
    
  case SDL_VIDEORESIZE:
    rc = eventResize(agEvent) || sigVideoResize(agEvent);
    break;
    
  default:
    rc = false;
    break;
  }
    }
  
  return rc;
}



bool AGMessageObject::eventActive(const AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventKeyDown(const AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventKeyUp(const AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventMouseMotion(const AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventMouseButtonDown(const AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventMouseButtonUp(const AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventQuit(const AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventQuitModal(const AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventSysWM(const AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventResize(const AGEvent *m)
{
  return false;
}

Uint8 AGMessageObject::getButtonState() const
{
  int x,y;
  Uint8 b=SDL_GetMouseState(&x,&y);
  return b;
}
AGPoint AGMessageObject::getMousePosition() const
{
  int x,y;
  SDL_GetMouseState(&x,&y);
  return AGPoint(x,y);
}


bool AGMessageObject::acceptEvent(const SDL_Event *pEvent)
{
  return true;
}

AGEvent *newEvent(AGListener *pCaller,const SDL_Event*s)
{
  return new AGSDLEvent(pCaller,s);
}

AGSDLEvent &toAGSDLEvent(AGEvent &e)
{
  return dynamic_cast<AGSDLEvent&>(e);
}
