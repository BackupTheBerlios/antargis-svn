/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_theme.h
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

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#ifndef AG_THEME_H
#define AG_THEME_H

#include "ag_font.h"
#include "ag_surface.h"

#include <map>

class AGTheme
{
 public:
  AGTheme();
  virtual ~AGTheme();

  AGFont getFont(const AGString &pName);
  AGColor getColor(const AGString &pName);

  void setFont(const AGString &pName,AGFont pFont);
  void setColor(const AGString &pName,AGColor pColor);

  int getInt(const AGString &pName);
  void setInt(const AGString &pName,int i);

  AGSurface getSurface(const AGString &pName);
  void setSurface(const AGString &pName,const AGSurface &pSurface);

  std::string getSurfaceName(const AGString &pName);
  void setSurfaceName(const AGString &pName,const std::string &pSurface);


  bool hasSurface(const AGString &pName) const;
  bool hasColor(const AGString &pName) const;

 private:

  AGString trunk(AGString s);

  std::map<AGString,AGFont> mFonts;
  std::map<AGString,AGColor> mColors;
  std::map<AGString,AGSurface> mSurfaces;
  std::map<AGString,std::string> mSurfaceNames;
  std::map<AGString,int> mInts;
};

AGTheme *getTheme();
void setTheme(const AGTheme &aTheme);
void loadTheme(const AGData &pXML);
bool loadThemeFile(const AGFilename &pFilename);

AGString addPoint(const AGString &pTheme);

#endif
