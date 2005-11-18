/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_main.h
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

#ifndef __AG_MAIN
#define __AG_MAIN

#include <string>

class AGScreen;
struct SDL_VideoInfo;
class AGMain
{
 public:
  AGMain(int w,int h,int d,bool fs,bool gl);
  ~AGMain();

  void flip();
  void changeRes(int w,int h,int d,bool fs,bool gl);
  void toggleFull();

  int width() const;
  int height() const;

  void setIcon(const std::string &pFile);
  void setCaption(const std::string &pCaption);
 private:
  AGScreen *mScreen;

  const SDL_VideoInfo *videoInfo;
};

AGMain *getMain();

#endif
