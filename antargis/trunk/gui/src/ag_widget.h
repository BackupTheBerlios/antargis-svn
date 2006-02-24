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

#include <list>

#include "ag_painter.h"

class AGTooltip;

class AGWidget:public AGMessageObject
{
 public:
  AGWidget(AGWidget *pParent,const AGRect2 &r);
  virtual ~AGWidget();
  
  virtual void draw(AGPainter &p);
  virtual void drawAfter(AGPainter &p);
  virtual void drawAll(AGPainter &p);
  AGRect2 getRect() const;
  virtual AGRect2 getClientRect() const;
  void setRect(const AGRect2 &pRect);
  
  void setParent(AGWidget *pParent);
  AGWidget *getParent();

  virtual bool eventShow();
  virtual bool eventHide();

  virtual bool eventMouseMotion(AGEvent *m);

  virtual bool eventMouseEnter();
  virtual bool eventMouseLeave();
  virtual bool eventMouseClick(AGEvent *m);

  virtual bool eventMouseButtonDown(AGEvent *m);
  virtual bool eventMouseButtonUp(AGEvent *m);

  virtual bool eventGotFocus();
  virtual bool eventLostFocus();

  virtual bool canFocus() const;

  bool processEvent(AGEvent *event);

  AGSignal sigMouseEnter;
  AGSignal sigMouseLeave;
  AGSignal sigClick;

  virtual float minWidth() const;
  virtual float minHeight() const;

  float width() const;
  float height() const;
  float top() const;
  float left() const;
  float bottom() const;
  float right() const;

  bool visible() const;

  virtual void setWidth(float w);
  virtual void setHeight(float w);
  virtual void setTop(float y);
  virtual void setLeft(float x);

  bool fixedWidth() const;
  bool fixedHeight() const;

  void show();
  void hide();

  virtual void mark();

  virtual void addChild(AGWidget *w);
  virtual void addChildBack(AGWidget *w);

  virtual void removeChild(AGWidget *w);

  // Functions for caching appearance
  virtual bool redraw() const;
  virtual void prepareDraw();
  void setCaching(bool pEnable);
  bool checkRedraw() const;
  void setDrawn();
  void queryRedraw();
  AGRect2 getChangeRect();

  // when caching, "normal" textures are not used, so they get discarded
  // to prevent this, use this function to "use" them in each frame
  virtual void useTextures(); // when caching
  void useTexturesRecursive(); // when caching


  // clear children
  void clear();

  AGRect2 getScreenRect() const;
  AGVector2 getScreenPosition() const;
  AGVector2 fromScreen(const AGVector2 &p) const;

  // focus

  /** should only be called by a child */
  void gainFocus(AGWidget *pWidget=0);
  void gainCompleteFocus(AGWidget *pWidget=0);


  virtual bool eventDragBy(AGEvent *event,const AGVector2 &pDiff);

  bool getFocus() const;
  bool hasFocus(const AGWidget *pWidget=0);


  std::string getName() const;
  void setName(const std::string &pName);
  AGWidget *getChild(const std::string &pName);

  void setModal(bool pModal);

  void erase(AGWidget *w);

  void eventChildrenDeleted(AGWidget *pWidget);

  void setTooltip(const std::string &pTooltip);

 private:

  void regChange();

  void drawCache();

  void delObjects();

  void gainFocusDown(AGWidget *pWidget);

  void checkFocus();

  std::list<AGWidget*> mToClear;

  AGRect2 mr;
  AGWidget *mParent;
  bool mChildrenEventFirst;
  bool mChildrenDrawFirst;
  bool mMouseIn;
  bool mButtonDown;
  bool mFixedWidth,mFixedHeight;
  bool mVisible;
  bool mCaching;
  AGTexture *mCache;
  bool mCacheTouched;
  //  AGMenu *mMenu;

  bool mHasFocus;
  AGWidget *mFocus;

  AGVector2 mOldMousePos;

  std::string mName;
  bool mModal;

  AGRect2 mChangeRect;

  std::string mTooltip;
  AGTooltip *mTooltipWidget;

 protected:
  std::list<AGWidget*> mChildren;

};

AGWidget *toAGWidget(AGMessageObject *o);

extern AGWidget *agNoParent;

#endif
