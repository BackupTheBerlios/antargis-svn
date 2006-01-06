/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_menu.h
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

#ifndef AG_MENU_H
#define AG_MENU_H

#include "ag_vtiler.h"
#include "ag_surface.h"

class AGMenuEvent:public AGEvent
{
 public:
  AGMenuEvent(AGListener *pListener,const std::string &pName);

  std::string getName() const;
 private:
  std::string mName;
};

class AGMenu:public AGVTiler
{
 public:
  AGMenu(AGWidget *pParent,AGPoint pWishPos,const std::string &pName);

  void show(AGPoint pWishPos);

  void addItem(const std::string &pString);
  AGMenu &addMenu(const std::string &pString);

  void draw(const AGRect &r);

  void eventItemSelected(const std::string &pString);
  void eventItemClicked(const std::string &pString);

  AGSignal sigSelected;

 private:
  AGTexture mBg;
  std::string mName;
  AGPoint mWishPos;
};

#endif
