/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_messageobject.h
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

#ifndef AG_MESSAGEOBJECT_H
#define AG_MESSAGEOBJECT_H

#include <SDL.h>
#include <string>
#include <set>

#include "ag_geometry.h"
#include "ag_rubyobj.h"

class AGListener;
class AGSignal;


/** AGEvent is the typical event which is passed when the user does anything.
    You can however derive from it and pass it through a signal.
    But you shouldn't delete it for yourself.
 */
class AGEvent
{
 public:
  AGEvent(AGListener *pCaller,const std::string &pName,const SDL_Event *pEvent=0);
  virtual ~AGEvent();

  AGListener *getCaller() const;
  
  void setCaller(AGListener *pCaller);
  std::string getName() const;

  AGVector2 getMousePosition() const;
  SDLKey getKey() const;
  SDLMod getMod() const;
  int getButton() const;

  const SDL_Event *get() const;

  bool isSDLEvent() const;

  void setName(const std::string &n);

  void setVector(const AGVector2 &v);
  AGVector2 getVector() const;

 private:
  AGListener *mCaller;
  std::string mName;

  const SDL_Event *mEvent;

  AGVector2 mVector;
};


/**
   AGListener is the base for receiving an event. Any object which is derived from AGListener can
   receive an event. But you should use AGMessageObject for this, as it holds basic "listening"
   facilities.
   @see AGEvent
*/
class AGListener:public AGRubyObject
{
 public:
  AGListener();
  virtual ~AGListener();
  virtual bool signal(AGEvent *m);
  
};

/**
   some syntactic sugar for simple connecting of slots and signals.
   @see AGSignal
   @see AGMessageObject
*/
class AGCPPListener
{
 public:
  virtual ~AGCPPListener();
  virtual bool signal(AGEvent *m) const=0;
};

/**
   some syntactic sugar for simple connecting of slots and signals.
   @see AGSignal
   @see AGMessageObject
*/
template<class T>
class AGSlot0:public AGCPPListener
{
 public:
  typedef bool (T::*FKT)(AGEvent *m);
  T *base;
  FKT f;
  
  AGSlot0(T *pBase,FKT pF):
    base(pBase),f(pF)
    {
    }
    virtual ~AGSlot0()
      {
      }

    virtual bool signal(AGEvent *m) const
    {
      return (base->*f)(m);
    }
};

class AGMessageObject;

/** AGSignal is a placeholder-class for a function which calls all the Slots, which 
    are connected to this signal.
    For instance a button named "close" holds a sigClick signal and a dialog box has a slotClose.
    You call sigClick(event) in the button and the connected slot is automatically called.
    @see connect()
*/
class AGSignal
{
 public:
  AGSignal();
  AGSignal(AGMessageObject *pCaller);
  AGSignal(AGMessageObject *pCaller,const std::string &pName);

  virtual ~AGSignal();

  void connect(AGListener &pListener);
  void disconnect(AGListener &pListener);

  void connect(AGCPPListener *pListener);
  void disconnect(AGCPPListener *pListener);

  bool signal(AGEvent *m);

  bool operator()(AGEvent *m);
 private:
  std::set<AGListener*> mListeners;

  std::set<AGCPPListener*> mSimpleListeners;

  std::string mName;
  AGMessageObject *mCaller;
};

/**
   AGMessageObject handles libSDL-events and provides virtual handlers.
*/
class AGMessageObject:public AGListener
{
 public:
  AGMessageObject();
  virtual ~AGMessageObject();

  bool processEvent(AGEvent *pEvent);

  virtual bool acceptEvent(const SDL_Event *pEvent);


  // event handler
  virtual bool eventActive(AGEvent *m);
  virtual bool eventKeyDown(AGEvent *m);
  virtual bool eventKeyUp(AGEvent *m);
  virtual bool eventMouseMotion(AGEvent *m);
  virtual bool eventMouseButtonDown(AGEvent *m);
  virtual bool eventMouseButtonUp(AGEvent *m);
  virtual bool eventQuit(AGEvent *m);
  virtual bool eventQuitModal(AGEvent *m);
  virtual bool eventSysWM(AGEvent *m);
  virtual bool eventResize(AGEvent *m);

  virtual Uint8 getButtonState() const;
  virtual AGVector2 getMousePosition() const;


  AGSignal sigActive;
  AGSignal sigKeyDown;
  AGSignal sigKeyUp;
  AGSignal sigMouseMotion;
  AGSignal sigMouseButtonDown;
  AGSignal sigMouseButtonUp;
  AGSignal sigQuit;
  AGSignal sigSysWM;
  AGSignal sigVideoResize;

  void pushSignal(AGSignal *pSignal);
  void popSignal(AGSignal *pSignal);

 private:
  bool mCanReceiveMessages;

  std::set<AGSignal*> mSignals;

  static AGMessageObject *captureObject;
};


AGEvent *newEvent(AGListener *pCaller,const std::string &pName,const SDL_Event*s);

/**
 */   

template<class T>
AGCPPListener *slot(T *base,bool (T::*f)(AGEvent *))
{
  return new AGSlot0<T>(base,f);
}


std::string toString(SDL_Event *pEvent);
SDL_Event *toSDLEvent(const std::string &p);

#endif
