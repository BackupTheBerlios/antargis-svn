#ifndef AG_MESSAGEOBJECT_H
#define AG_MESSAGEOBJECT_H

#include <SDL.h>
#include <string>
#include <set>

#include "ag_geometry.h"

class AGListener;

class AGEvent
{
 public:
  AGEvent(AGListener *pCaller);
  ~AGEvent();

  AGListener *getCaller() const;
 private:
  AGListener *mCaller;
};

class AGSDLEvent:public AGEvent
{
  const SDL_Event *mEvent;
 public:
  AGSDLEvent(AGListener *pCaller,const SDL_Event *pEvent);

  AGPoint getMousePosition() const;
  SDLKey getKey() const;
  SDLMod getMod() const;
  Uint8 getButton() const;

  const SDL_Event *get() const;
};

class AGMouseEvent:public AGEvent
{
 public:
  AGMouseEvent(AGListener *pCaller,SDL_Event *pEvent);
  ~AGMouseEvent();
};


class AGListener
{
 public:
  AGListener();
  virtual ~AGListener();
  virtual bool signal(const char*pName,const AGEvent *m);
  
  
};

class AGCPPListener
{
 public:
  virtual bool signal(const char*pName,const AGEvent *m) const=0;
};


template<class T>
class AGSlot:public AGCPPListener
{
 public:
  typedef bool (T::*FKT)(const char*pName,const AGEvent *m);
  T *base;
  //  bool (T::*f)(const char*pName,const AGEvent *m);
  FKT f;
  
  AGSlot(T *pBase,FKT pF):
    base(pBase),f(pF)
    {
    }
    virtual ~AGSlot()
      {
      }

    virtual bool signal(const char*pName,const AGEvent *m) const
    {
      return (base->*f)(pName,m);
    }
};

template<class T>
AGCPPListener *slot(T *base,bool (T::*f)(const char*pName,const AGEvent *m))
{
  return new AGSlot<T>(base,f);
}

class AGSignal
{
 public:
  AGSignal();
  AGSignal(const std::string &pName);
  void connect(AGListener &pListener);
  void disconnect(AGListener &pListener);

  void connect(AGCPPListener *pListener);
  void disconnect(AGCPPListener *pListener);

  bool signal(const AGEvent *m);

  bool operator()(const AGEvent *m);
 private:
  std::set<AGListener*> mListeners;

  std::set<AGCPPListener*> mSimpleListeners;

  std::string mName;
};

class AGMessageObject:public AGListener
{
 public:
  AGMessageObject();

  bool processEvent(const AGEvent *pEvent);

  virtual bool acceptEvent(const SDL_Event *pEvent);


  // event handler
  virtual bool eventActive(const AGEvent *m);
  virtual bool eventKeyDown(const AGEvent *m);
  virtual bool eventKeyUp(const AGEvent *m);
  virtual bool eventMouseMotion(const AGEvent *m);
  virtual bool eventMouseButtonDown(const AGEvent *m);
  virtual bool eventMouseButtonUp(const AGEvent *m);
  virtual bool eventQuit(const AGEvent *m);
  virtual bool eventQuitModal(const AGEvent *m);
  virtual bool eventSysWM(const AGEvent *m);
  virtual bool eventResize(const AGEvent *m);

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

 private:
  bool mCanReceiveMessages;

  static AGMessageObject *captureObject;
};


AGEvent *newEvent(AGListener *pCaller,const SDL_Event*s);

#endif
