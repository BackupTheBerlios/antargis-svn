/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_combo.h
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

#ifndef AG_COMBO_H
#define AG_COMBO_H

#include <ag_widget.h>

class AGEdit;
class AGButton;
class AGListBox;

class AGComboBox:public AGWidget
{
 public:
  AGComboBox(AGWidget *pParent,const AGRect2 &pRect);

  void insertItem(const std::string &pID,const std::string &pContent);

  bool eventButtonClicked(AGEvent *pEvent);
  bool eventSelected(AGEvent *pEvent);

  AGSignal sigSelect;
  
  std::string getSelected() const;
  void setSelected(const std::string &pID);

  void clear();

 private:
  void update();

  AGEdit *mEdit;
  AGButton *mButton;
  AGListBox *mListBox;

  std::string mID;

  std::list<std::pair<std::string,std::string> > ops;
};

#endif
