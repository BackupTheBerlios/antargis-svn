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

struct AGListBoxItem
{
  AGListBoxItem(std::string pID,std::string pValue);
  std::string id,value;
};


// Single selection for a start
class AGListBox:public AGWidget
{
 public:
  AGListBox(AGWidget *pParent,const AGRect &pRect);

  void insert(std::string pID,std::string pValue);
  void select(std::string pID);

  std::string getSelectedID() const;
  std::string getSelectedValue() const;

  virtual bool eventKeyDown(const AGEvent *m);

  AGSignal sigSelect;
  AGSignal sigDoubleClick;

  virtual void draw(AGPainter &p);
  virtual bool eventMouseClick(const AGEvent *m);

  void clearList();

 private:

  void arrange();

  int mY;
  std::vector<AGListBoxItem> mItems;
  int mSelected;
  int mHeight;
  int mItemHeight;
  std::vector<AGEdit*> mEdits;
  AGBackground mBackground,mHilight;
};


AGListBox &toAGListBox(AGWidget &w);

#endif
