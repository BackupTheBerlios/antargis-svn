/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_application.h
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
