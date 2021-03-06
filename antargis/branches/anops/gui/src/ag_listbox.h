/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_listbox.h
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

#ifndef __AG_LISTBOX_H
#define __AG_LISTBOX_H

#include <string>
#include <vector>
#include "ag_widget.h"
#include "ag_background.h"

class AGEdit;
class AGScroller;

struct AGListBoxItem
{
  AGListBoxItem(AGString pID,AGStringUtf8 pValue);
  AGString id;
  AGStringUtf8 value;
};


// Single selection for a start
class AGListBox:public AGWidget
{
 public:
  AGListBox(AGWidget *pParent,const AGRect2 &pRect);

  void insertItem(AGString pID,AGStringUtf8 pValue);
  void selectItem(AGString pID);

  AGString getSelectedID() const;
  AGString getSelectedValue() const;

  virtual bool eventKeyDown(AGEvent *m);

  AGSignal sigSelect;
  AGSignal sigDoubleClick;

  virtual void draw(AGPainter &p);
  virtual bool eventMouseClick(AGEvent *m);
  virtual bool eventMouseButtonDown(AGEvent *m);
  virtual bool eventMouseButtonUp(AGEvent *m);

  bool eventScroller(AGEvent *e);

  void clearList();

  void updateScroller();

 private:

  void arrange();

  int mY;
  std::vector<AGListBoxItem> mItems;
  int mSelected;
  int mHeight;
  int mItemHeight;
  std::vector<AGEdit*> mEdits;
  AGBackground mBackground,mHilight;

  AGScroller *mScroller;
};


AGListBox &toAGListBox(AGWidget &w);

#endif
