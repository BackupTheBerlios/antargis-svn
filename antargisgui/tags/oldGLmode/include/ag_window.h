/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_window.h
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

#ifndef AG_WINDOW_H
#define AG_WINDOW_H

#include "ag_table.h"

class AGWindow:public AGTable
{
 public:
  AGWindow(AGWidget *pWidget,const AGRect &pRect,const std::string &pTitle="");

  void addChild(AGWidget *w);

  AGWidget *getClient();

  virtual bool eventMouseButtonDown(const AGEvent *m);
  bool eventDragBy(const AGEvent *event,const AGPoint &pDiff);

  void close();

  // slot
  bool tryClose(const std::string&pName,const AGEvent *m);

  AGRect getClientRect() const;

  AGSignal sigClose;

 private:

  AGWidget *getTitleBar();

  std::string mTitle;
  AGWidget *mClient;
};

AGWindow &toAGWindow(AGWidget &w);

#endif
