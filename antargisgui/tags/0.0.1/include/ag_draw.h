/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_draw.h
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

#ifndef AG_DRAW_H
#define AG_DRAW_H

#include "ag_geometry.h"
#include "ag_color.h"
#include "ag_surface.h"

class AGDraw
{
public:
  static void AGDraw::drawGradient(AGSurface &surface, const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  static void AGDraw::drawGradientAlpha(AGSurface &surface, const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  static void AGDraw::drawGradientAlpha2(AGSurface &surface, const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  static void AGDraw::drawBorder(AGSurface &surface, const AGRect& rect,int width, const AGColor& c1, const AGColor& c2);

  static void AGDraw::drawGradient(AGGScreen *surface, const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  static void AGDraw::drawGradientAlpha(AGGScreen *surface, const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  static void AGDraw::drawGradientAlpha2(AGGScreen *surface, const AGRect& rect, const AGColor& ul, const AGColor& ur, const AGColor& dl, const AGColor& dr);
  static void AGDraw::drawBorder(AGGScreen *surface, const AGRect& rect,int width, const AGColor& c1, const AGColor& c2);
};


#endif
