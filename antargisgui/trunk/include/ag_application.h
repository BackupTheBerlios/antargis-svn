#ifndef AG_APPLICATION_H
#define AG_APPLICATION_H

#include "ag_messageobject.h"
#include "ag_widget.h"

class AGApplication:public AGMessageObject
{
 public:
  AGApplication();

  bool run();

  virtual bool eventIdle();
  virtual bool eventFrame(float pTime);

  virtual bool eventQuit(const AGEvent *m);
  virtual bool eventKeyDown(const AGEvent *m2);

  void setMainWidget(AGWidget *w);

  void draw();

  void tryQuit();
  
 private:
  void clearOldMousePosition();
  void drawCursor();

  void flushEventQueue();
  bool doEvent(const SDL_Event *e);

  bool mRunning;
  bool mIdleCalls;
  AGWidget *mainWidget;
};

#endif
