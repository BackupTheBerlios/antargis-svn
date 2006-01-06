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

class AGEvent
{
 public:
  AGEvent(AGListener *pCaller,const std::string &pName,const SDL_Event *pEvent=0);
  virtual ~AGEvent();

  AGListener *getCaller() const;
  void setCaller(AGListener *pCaller);
  std::string getName() const;

  AGPoint getMousePosition() const;
  SDLKey getKey() const;
  SDLMod getMod() const;
  int getButton() const;

  const SDL_Event *get() const;

  bool isSDLEvent() const;

  void setName(const std::string &n);

 private:
  AGListener *mCaller;
  std::string mName;

  const SDL_Event *mEvent;
};

/*
class AGMouseEvent:public AGEvent
{
 public:
  AGMouseEvent(AGListener *pCaller,SDL_Event *pEvent);
  ~AGMouseEvent();
};
*/
class AGMessageObject;


class AGListener:public AGRubyObject
{
 public:
  AGListener();
  virtual ~AGListener();
  virtual bool signal(AGEvent *m);
  
};

class AGCPPListener
{
 public:
  virtual ~AGCPPListener();
  virtual bool signal(AGEvent *m) const=0;
};


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

/*
template<class T>
class AGSlot:public AGCPPListener
{
 public:
  typedef bool (T::*FKT)(const std::string&pName,const AGEvent *m);
  T *base;
  FKT f;
  
  AGSlot(T *pBase,FKT pF):
    base(pBase),f(pF)
    {
    }
    virtual ~AGSlot()
      {
      }

    virtual bool signal(AGEvent *m) const
    {
      return (base->*f)(pName,m);
    }
};

template<class T>
class AGSlot2:public AGCPPListener
{
 public:
  typedef bool (T::*FKT)(const std::string&pName,const AGEvent *m,AGMessageObject *pCaller);
  T *base;
  FKT f;

  
  AGSlot2(T *pBase,FKT pF):
    base(pBase),f(pF)
    {
    }
    virtual ~AGSlot2()
      {
      }

    virtual bool signal(const std::string &pName,const AGEvent *m,AGMessageObject *pCaller) const
    {
      return (base->*f)(pName,m,pCaller);
    }
};
*/

class AGSignal
{
 public:
  AGSignal();
  //  AGSignal(const AGSignal &s);
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
  virtual AGPoint getMousePosition() const;


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
/*
template<class T>
AGCPPListener *slot(T *base,bool (T::*f)(const std::string&,const AGEvent *))
{
  return new AGSlot<T>(base,f);
}

template<class T>
AGCPPListener *slot(T *base,bool (T::*f)(const std::string&,const AGEvent *,AGMessageObject *))
{
  return new AGSlot2<T>(base,f);
}
*/

template<class T>
AGCPPListener *slot(T *base,bool (T::*f)(AGEvent *))
{
  return new AGSlot0<T>(base,f);
}
#endif
