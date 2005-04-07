/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_menuitem.h
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

#ifndef AG_MENUITEM_H
#define AG_MENUITEM_H

#include "ag_text.h"
#include "ag_vtiler.h"

class AGMenu;

class AGMenuItem:public AGHTiler
{
 public:
  AGMenuItem(AGWidget *pParent,const std::string &pText);
  virtual ~AGMenuItem();

  void draw(const AGRect &r);

  virtual bool eventMouseEnter();
  virtual bool eventMouseLeave();
  
  bool getSelected() const;
  void unSelect();

  std::string getName() const;

  virtual bool eventMouseClick(const AGEvent *m);

  virtual void eventSelect();
  virtual void eventUnselect();

 private:
  std::string mText;
  bool mMouseOver;
  bool mSelected;
};

class AGSubMenu:public AGMenuItem
{
 public:
  AGSubMenu(AGWidget *pParent,const std::string &ptext);
  virtual ~AGSubMenu();

  AGMenu *getMenu();

  virtual void eventSelect();
  virtual void eventUnselect();

  virtual bool eventMouseClick(const AGEvent *m);

 private:
  void addChild(AGWidget*pWidget);

  AGMenu *mSubMenu;
};

#endif
