/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_vtiler.h
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

#ifndef AG_VTILER_H
#define AG_VTILER_H

#include "ag_widget.h"

class AGVTiler:public AGWidget
{
 public:
  AGVTiler(AGWidget *pParent,const AGRect&pRect,bool pAdaptMyHeight=true);


  virtual void addChild(AGWidget *pWidget);

  virtual void rePosition();
  //void adaptWidth();
  void adaptHeightFromChildren();
  void adaptWidthFromChildren();
 protected:
  bool mAdaptMyHeight;
};

class AGHTiler:public AGVTiler
{
 public:
  AGHTiler(AGWidget *pParent,const AGRect&pRect,bool pAdaptMyHeight=true);
  virtual void rePosition();
  void adaptHeightFromChildren();
  void adaptWidthFromChildren();
  void adaptWidthFromParent();
};

#endif
