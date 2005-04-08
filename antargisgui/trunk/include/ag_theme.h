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

  AGFont getFont(const std::string &pName);
  AGColor getColor(const std::string &pName);

  void setFont(const std::string &pName,AGFont pFont);
  void setColor(const std::string &pName,AGColor pColor);

  int getInt(const std::string &pName);
  void setInt(const std::string &pName,int i);

  AGSurface getSurface(const std::string &pName);
  void setSurface(const std::string &pName,const AGSurface &pSurface);

  bool hasSurface(const std::string &pName) const;

 private:

  std::string trunk(std::string s);

  std::map<std::string,AGFont> mFonts;
  std::map<std::string,AGColor> mColors;
  std::map<std::string,AGSurface> mSurfaces;
  std::map<std::string,int> mInts;
};

AGTheme *getTheme();
void setTheme(const AGTheme &aTheme);
void loadTheme(const std::string &pXML);

#endif
