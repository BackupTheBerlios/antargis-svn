/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_color.h
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

#ifndef AG_COLOR_H
#define AG_COLOR_H

#include <SDL.h>
#include <string>
#include <iostream>

class AGSurface;

class AGColor:public SDL_Color
{
 public:
  //#ifdef SWIG
  AGColor(int pr,int pg,int pb,int pa=255);//Uint8 pr,Uint8 pg,Uint8 pb,Uint8 pa);
  /*#else
  //  AGColor(Uint8 pr=0,Uint8 pg=0,Uint8 pb=0,Uint8 pa=255);
  #endif*/
  
  AGColor(const AGColor &c);
  AGColor(Uint32 c,const AGSurface &pSurface);
  AGColor(const std::string &s);

  AGColor();

  AGColor &operator=(const AGColor&c);

  Uint32 mapRGB(SDL_PixelFormat *f) const;

  std::string toString() const;

  AGColor grey() const;

  Uint8 brightness() const;

  void light(Uint8 l);

  //  SDL_Color sdlColor() const;

  //  Uint8 r,g,b;
  Uint8 a;
};

bool operator<(const AGColor &c1,const AGColor &c2);

std::ostream &operator<<(std::ostream &o,const AGColor &c);

#endif
