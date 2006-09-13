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
#include "ag_main.h"

// AGEvent
AGEvent::AGEvent(AGListener *pCaller,const std::string &pName,const SDL_Event *e):mCaller(pCaller),mName(pName),mEvent(e)
{
}
AGEvent::~AGEvent()
{
}


void AGEvent::setVector(const AGVector2 &v)
{
  mVector=v;
}
/// returns the drag-vector in case of a drag-event
AGVector2 AGEvent::getVector() const
{
  return mVector;
}


/// sets the caller of this event - shouldn't be used outside of AGWidget
void AGEvent::setCaller(AGListener *pCaller)
{
  mCaller=pCaller;
}

/// return the name of the event
std::string AGEvent::getName() const
{
  return mName;
}


void AGEvent::setName(const std::string &n)
{
  mName=n;
}

bool AGEvent::isSDLEvent() const
{
  return mEvent;
}


/// returns the caller of this event
AGListener *AGEvent::getCaller() const
{
  return mCaller;
}

// AGSDLEvent

const SDL_Event *AGEvent::get() const
{
  assert(mEvent);
  return mEvent;
}

AGVector2 AGEvent::getMousePosition() const
{
  assert(mEvent);
  AGVector2 p;
  switch(mEvent->type) {
  case SDL_MOUSEMOTION:
    p[0]=mEvent->motion.x*getMain()->width()/getMain()->realWidth();
    p[1]=mEvent->motion.y*getMain()->height()/getMain()->realHeight();
    break;
    
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    p[0]=mEvent->button.x*getMain()->width()/getMain()->realWidth();
    p[1]=mEvent->button.y*getMain()->height()/getMain()->realHeight();
    break;
    
  }
  return p;
}

SDLKey AGEvent::getKey() const
{
  assert(mEvent);
  return mEvent->key.keysym.sym;
}

SDLMod AGEvent::getMod() const
{
  assert(mEvent);
  return mEvent->key.keysym.mod;
}

int AGEvent::getButton() const
{
  assert(mEvent);
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

/*
AGMouseEvent::AGMouseEvent(AGListener *pCaller,SDL_Event *pEvent):AGEvent(pCaller)
{
}
AGMouseEvent::~AGMouseEvent()
{
}*/

// AGListener

AGListener::AGListener()
{
}

AGListener::~AGListener()
{
}

bool AGListener::signal(AGEvent *m)
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
#undef connect


/**
   This is the simple connect version. You have to override signal(.) in AGListener and
   provide a reasonable functionality there. For simple usage use AGSignal::connect( AGCPPListener *pListener);

*/
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


/**
   When connecting a signal to a slot you call this function.
   For instance you have a class:
   <pre>
   class A
   {
     public:
     AGSignal sigSomething;
   };
   class B
   {
     public:
     bool eventSomething( AGEvent *e);
   };
   A a;
   B b;
   // you can connect them like this;
   A.sigSomething.connect(slot(&b,&B::eventSomething));
   </pre>
*/

void AGSignal::connect(AGCPPListener *pListener)
{
  mSimpleListeners.insert(pListener);
}
void AGSignal::disconnect(AGCPPListener *pListener)
{
  mSimpleListeners.erase(pListener);
}

bool AGSignal::signal(AGEvent *m)
{
  m->setName(mName);
  std::set<AGListener*>::iterator i=mListeners.begin();
  bool value=false;
  for(;i!=mListeners.end();i++)
    {
      if((*i)->signal(m))
	value=true;
    }

  std::set<AGCPPListener*>::iterator j=mSimpleListeners.begin();
  for(;j!=mSimpleListeners.end();j++)
    {
      if((*j)->signal(m))
	value=true;
    }

  return value;
}

bool AGSignal::operator()(AGEvent *m)
{
  m->setName(mName);
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

bool AGMessageObject::processEvent(AGEvent* agEvent) 
{

  //  cdebug("typeid:"<<typeid(this).name());
  //  TRACE;
  //  SDL_Event e;
  bool rc=false;

  //  const AGSDLEvent *agEvent=reinterpret_cast<const AGSDLEvent*>(pEvent);
  if(agEvent->isSDLEvent())
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



bool AGMessageObject::eventActive(AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventKeyDown(AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventKeyUp(AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventMouseMotion(AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventMouseButtonDown(AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventMouseButtonUp(AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventQuit(AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventQuitModal(AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventSysWM(AGEvent *m)
{
  return false;
}
bool AGMessageObject::eventResize(AGEvent *m)
{
  return false;
}

Uint8 AGMessageObject::getButtonState() const
{
  int x,y;
  Uint8 b=SDL_GetMouseState(&x,&y);
  return b;
}
AGVector2 AGMessageObject::getMousePosition() const
{
  int x,y;
  SDL_GetMouseState(&x,&y);
  return AGVector2(x,y);
}


bool AGMessageObject::acceptEvent(const SDL_Event *pEvent)
{
  return true;
}

AGEvent *newEvent(AGListener *pCaller,const std::string &pName,const SDL_Event*s)
{
  return new AGEvent(pCaller,pName,s);
}


std::string toString(SDL_keysym k)
{
  std::ostringstream os;
  os<<(int)k.scancode<<":"<<(int)k.sym<<":"<<(int)k.mod<<":"<<k.unicode;

  return os.str();
}

Uint8 toUint8(const std::string &s)
{
  long i;
  std::istringstream is;
  is.str(s);
  is>>i;
  return i;
}
Sint16 toSint16(const std::string &s)
{
  long i;
  std::istringstream is;
  is.str(s);
  is>>i;
  return i;
}

std::string getUntil(std::string &b,const std::string &f)
{
  size_t i=b.find(f);
  std::string s=b.substr(0,i);
  if(i!=std::string::npos)
    b=b.substr(i+1,std::string::npos);
  return s;
}

SDL_keysym toKeysym(const std::string &s)
{
  SDL_keysym k;
  std::string b=s;
  k.scancode=toUint8(getUntil(b,":"));
  k.sym=(SDLKey)toUint8(getUntil(b,":"));
  k.mod=(SDLMod)toUint8(getUntil(b,":"));
  k.unicode=toSint16(getUntil(b,":"));
  

  return k;
}

std::string toString(SDL_Event *pEvent)
{
  std::ostringstream os;
  if(pEvent)
    {
      switch(pEvent->type)
	{
	case SDL_ACTIVEEVENT:
	  os<<"SDL_ACTIVEEVENT:"<<(int)pEvent->active.gain<<":"<<(int)pEvent->active.state;
	  break;
	case SDL_KEYDOWN:
	  os<<"SDL_KEYDOWN:"<<(int)pEvent->key.which<<":"<<(int)pEvent->key.state<<":"<<toString(pEvent->key.keysym);
	  break;
	case SDL_KEYUP:
	  os<<"SDL_KEYUP:"<<(int)pEvent->key.which<<":"<<(int)pEvent->key.state<<":"<<toString(pEvent->key.keysym);
	  break;
	case SDL_MOUSEMOTION:
	  os<<"SDL_MOUSEMOTION:"<<(int)pEvent->motion.which<<":"<<(int)pEvent->motion.state<<":"<<pEvent->motion.x<<":"<<pEvent->motion.y<<":"<<pEvent->motion.xrel<<":"<<pEvent->motion.yrel;
	  break;
	case SDL_MOUSEBUTTONDOWN:
	  os<<"SDL_MOUSEBUTTONDOWN:"<<(int)pEvent->button.which<<":"<<(int)pEvent->button.button<<":"<<(int)pEvent->button.state<<":"<<pEvent->button.x<<":"<<pEvent->button.y;
	  break;
	case SDL_MOUSEBUTTONUP:
	  os<<"SDL_MOUSEBUTTONUP:"<<(int)pEvent->button.which<<":"<<(int)pEvent->button.button<<":"<<(int)pEvent->button.state<<":"<<pEvent->button.x<<":"<<pEvent->button.y;
	  break;
	case SDL_JOYAXISMOTION:
	  os<<"SDL_JOYAXISMOTION:"<<(int)pEvent->jaxis.which<<":"<<(int)pEvent->jaxis.axis<<":"<<pEvent->jaxis.value;
	  break;
	case SDL_JOYBALLMOTION:
	  os<<"SDL_JOYBALLMOTION:"<<(int)pEvent->jball.which<<":"<<(int)pEvent->jball.ball<<":"<<pEvent->jball.xrel<<":"<<pEvent->jball.yrel;
	  break;
	case SDL_JOYHATMOTION:
	  os<<"SDL_JOYHATMOTION:"<<(int)pEvent->jhat.which<<":"<<(int)pEvent->jhat.hat<<":"<<(int)pEvent->jhat.value;
	  break;
	case SDL_JOYBUTTONDOWN:
	  os<<"SDL_JOYBUTTONDOWN:"<<(int)pEvent->jbutton.which<<":"<<(int)pEvent->jbutton.button<<":"<<(int)pEvent->jbutton.state;
	  break;
	case SDL_JOYBUTTONUP:
	  os<<"SDL_JOYBUTTONUP:"<<(int)pEvent->jbutton.which<<":"<<(int)pEvent->jbutton.button<<":"<<(int)pEvent->jbutton.state;
	  break;
	case SDL_QUIT:
	  os<<"SDL_QUIT:";
	  break;
	}
      if(os.str().length())
	return os.str();
      
    }
  return "nil";
}



SDL_Event *toSDLEvent(const std::string &p)
{
  std::string b=p;
  std::string t=getUntil(b,":");
  static SDL_Event event;

  event.type=0;

  if(t=="SDL_ACTIVEEVENT")
    {
      event.type=SDL_ACTIVEEVENT;
      event.active.gain=toUint8(getUntil(b,":"));
      event.active.state=toUint8(getUntil(b,":"));
    }
  else if(t=="SDL_KEYDOWN")
    {
      event.type=SDL_KEYDOWN;
      event.key.which=toUint8(getUntil(b,":"));
      event.key.state=toUint8(getUntil(b,":"));
      event.key.keysym=toKeysym(b);
    }
  else if(t=="SDL_KEYUP")
    {
      event.type=SDL_KEYUP;
      event.key.which=toUint8(getUntil(b,":"));
      event.key.state=toUint8(getUntil(b,":"));
      event.key.keysym=toKeysym(b);

    }
  else if(t=="SDL_MOUSEMOTION")
    {
      event.type=SDL_MOUSEMOTION;
      event.motion.which=toUint8(getUntil(b,":"));
      event.motion.state=toUint8(getUntil(b,":"));
      event.motion.x=toSint16(getUntil(b,":"));
      event.motion.y=toSint16(getUntil(b,":"));
      event.motion.xrel=toSint16(getUntil(b,":"));
      event.motion.yrel=toSint16(getUntil(b,":"));
    }
  else if(t=="SDL_MOUSEBUTTONDOWN")
    {
      event.type=SDL_MOUSEBUTTONDOWN;
      event.button.which=toUint8(getUntil(b,":"));
      event.button.button=toUint8(getUntil(b,":"));
      event.button.state=toUint8(getUntil(b,":"));
      event.button.x=toSint16(getUntil(b,":"));
      event.button.y=toSint16(getUntil(b,":"));
    }
  else if(t=="SDL_MOUSEBUTTONUP")
    {
      event.type=SDL_MOUSEBUTTONUP;
      event.button.which=toUint8(getUntil(b,":"));
      event.button.button=toUint8(getUntil(b,":"));
      event.button.state=toUint8(getUntil(b,":"));
      event.button.x=toSint16(getUntil(b,":"));
      event.button.y=toSint16(getUntil(b,":"));
    }
  else if(t=="SDL_JOYAXISMOTION")
    {
      event.type=SDL_JOYAXISMOTION;
      event.jaxis.which=toUint8(getUntil(b,":"));
      event.jaxis.axis=toUint8(getUntil(b,":"));
      event.jaxis.value=toUint8(getUntil(b,":"));
    }
  else if(t=="SDL_JOYBALLMOTION")
    {
      event.type=SDL_JOYBALLMOTION;
      event.jball.which=toUint8(getUntil(b,":"));
      event.jball.ball=toUint8(getUntil(b,":"));
      event.jball.xrel=toSint16(getUntil(b,":"));
      event.jball.yrel=toSint16(getUntil(b,":"));
    }
  else if(t=="SDL_JOYHATMOTION")
    {
      event.type=SDL_JOYHATMOTION;
      event.jhat.which=toUint8(getUntil(b,":"));
      event.jhat.hat=toUint8(getUntil(b,":"));
      event.jhat.value=toUint8(getUntil(b,":"));
    }
  else if(t=="SDL_JOYBUTTONDOWN")
    {
      event.type=SDL_JOYBUTTONDOWN;
      event.jbutton.which=toUint8(getUntil(b,":"));
      event.jbutton.button=toUint8(getUntil(b,":"));
      event.jbutton.state=toUint8(getUntil(b,":"));
    }
  else if(t=="SDL_JOYBUTTONUP")
    {
      event.type=SDL_JOYBUTTONUP;
      event.jbutton.which=toUint8(getUntil(b,":"));
      event.jbutton.button=toUint8(getUntil(b,":"));
      event.jbutton.state=toUint8(getUntil(b,":"));
    }
  else if(t=="SDL_QUIT")
    {
      event.type=SDL_QUIT;
    }
  if(event.type)
    return &event;

  return 0;
}
