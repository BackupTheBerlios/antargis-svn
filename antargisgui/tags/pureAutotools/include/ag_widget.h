/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_widget.h
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

#ifndef AG_WIDGET_H
#define AG_WIDGET_H

#include "ag_geometry.h"
#include "ag_messageobject.h"

#ifdef USE_RUBY
#include "ruby.h"
#undef connect
#endif
#include <list>

#include "ag_painter.h"

//class AGMenu;

class AGWidget:public AGMessageObject
{
 public:
  AGWidget(AGWidget *pParent,const AGRect &r);
  virtual ~AGWidget();
  
  virtual void draw(AGPainter &p);
  virtual void drawAll(AGPainter &p);
  AGRect getRect() const;
  virtual AGRect getClientRect() const;
  void setRect(const AGRect &pRect);
  
  void setParent(AGWidget *pParent);
  AGWidget *getParent();

  virtual bool eventShow();
  virtual bool eventHide();

  virtual bool eventMouseMotion(const AGEvent *m);

  virtual bool eventMouseEnter();
  virtual bool eventMouseLeave();
  virtual bool eventMouseClick(const AGEvent *m);

  virtual bool eventMouseButtonDown(const AGEvent *m);
  virtual bool eventMouseButtonUp(const AGEvent *m);

  virtual bool eventGotFocus();
  virtual bool eventLostFocus();

  virtual bool canFocus() const;

  //  void setMenu(AGMenu *pMenu);

  bool processEvent(const AGEvent *event);

  AGSignal sigMouseEnter;
  AGSignal sigMouseLeave;
  AGSignal sigClick;

  virtual int minWidth() const;
  virtual int minHeight() const;

  int width() const;
  int height() const;
  int top() const;
  int left() const;

  virtual void setWidth(int w);
  virtual void setHeight(int w);
  virtual void setTop(int y);
  virtual void setLeft(int x);

  bool fixedWidth() const;
  bool fixedHeight() const;

  void show();
  void hide();


  virtual void addChild(AGWidget *w);
  virtual void addChildBack(AGWidget *w);

  virtual void removeChild(AGWidget *w);

  void clear();

  AGRect getScreenRect() const;
  AGPoint getScreenPosition() const;
  AGPoint fromScreen(const AGPoint &p) const;


  // focus

  /** should only be called by a child */
  void gainFocus(AGWidget *pWidget=0);
  void gainCompleteFocus(AGWidget *pWidget=0);


  virtual bool eventDragBy(const AGEvent *event,const AGPoint &pDiff);

  bool getFocus() const;
  bool hasFocus(const AGWidget *pWidget=0);


  std::string getName() const;
  void setName(const std::string &pName);
  AGWidget *getChild(const std::string &pName);

  void setModal(bool pModal);

  void erase(AGWidget *w);

  void eventChildrenDeleted(AGWidget *pWidget);

 private:

  void gainFocusDown(AGWidget *pWidget);

  void checkFocus();

  std::list<AGWidget*> mToClear;

  AGRect mr;
  AGWidget *mParent;
  bool mChildrenEventFirst;
  bool mChildrenDrawFirst;
  bool mMouseIn;
  bool mButtonDown;
  bool mFixedWidth,mFixedHeight;
  bool mVisible;
  //  AGMenu *mMenu;

  bool mHasFocus;
  AGWidget *mFocus;

  AGPoint mOldMousePos;

  std::string mName;
  bool mModal;

 protected:
  std::list<AGWidget*> mChildren;

  friend void AGWidget_markfunc(void *ptr);

 public:
#ifdef USE_RUBY
  VALUE mRUBY;
  bool mRubyObject;
#endif
};

AGWidget *toAGWidget(AGMessageObject *o);


void AGWidget_markfunc(void *ptr);

extern AGWidget *agNoParent;

#endif
