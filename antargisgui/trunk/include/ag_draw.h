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
