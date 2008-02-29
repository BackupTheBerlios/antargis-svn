/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_screenwidget.cc
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

#include "ag_screen.h"
#include "ag_screenwidget.h"
#include "ag_surface.h"
#include "ag_theme.h"
#include "ag_debug.h"


AGScreenWidget::AGScreenWidget():
  AGWidget(0,getScreen().getRect())
  {
    //  mTexture=AGTexture(getTheme()->getSurface("screen.background.image"));
  }
AGScreenWidget::~AGScreenWidget()
  {
  }

void AGScreenWidget::draw(AGPainter &p)
  {
    //  p.clip(AGRect2(10,10,50,50));
    //p.tile(mTexture);
    p.fillRect(p.getRect().origin(),AGColor(0,0,0));
  }
