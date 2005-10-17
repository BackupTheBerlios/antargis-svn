/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_radio.h
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

#ifndef AG_RADIO_H
#define AG_RADIO_H

#include "ag_checkbox.h"

class AGRadio;

// derive from AGWidget, so that it can be put into the hierarchy
class AGRadioGroup:public AGWidget
{
 public:
  AGRadioGroup(AGWidget *pParent,const AGRect &pr);
  virtual ~AGRadioGroup();

  virtual void eventChange(const std::string &p);

  void add(AGRadio *r);
  void erase(AGRadio *r);
  
  AGSignal sigChanged;
 private:
  std::set<AGRadio*> mChildren;
};

class AGRadio:public AGCheckBox
{
 public:
  AGRadio(AGWidget *pParent,AGRect pRect);//,AGRadioGroup *pGroup,std::string pName);
  virtual ~AGRadio();

  bool eventMouseClick(const AGEvent *m);
  void setGroup(AGRadioGroup *pGroup);
  //  bool eventMouseButtonUp(const AGEvent *m);
  //  bool eventMouseLeave();
  //  bool eventMouseEnter();
  
  void deselect();

 private:
  AGRadioGroup *mGroup;
};

#endif
