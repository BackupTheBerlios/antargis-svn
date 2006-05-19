/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_fontengine.h
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

#ifndef AG_FONTENGINE_H
#define AG_FONTENGINE_H

#include <string>

#include "ag_surface.h"
#include "ag_font.h"
#include "ag_geometry.h"

class AGFontEngine
{
 public:
  AGFontEngine();
  ~AGFontEngine();


  //  static bool renderText (AGSurface &pSurface, const AGRect2 &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn);
  //  static bool renderTextOld (AGScreen *pScreen, const AGRect2 &pClipRect, int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn);

  static AGTexture *renderText(int BaseLineX, int BaseLineY, const std::string &pText, const AGFont &ParamIn);

  static int getWidth(const AGFont &pFont,const std::string &pText);
  static int getHeight(const AGFont &pFont,const std::string &pText);
};

#endif

