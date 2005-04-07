/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_color.cc
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

#include "ag_color.h"
#include "ag_gsurface.h"

#include <sstream>


AGColor::AGColor()//:  mr(0),mg(0),mb(0),ma(0)
{
  r=g=b=a;
}

AGColor::AGColor(int pr,int pg,int pb,int pa)/*:
AGColor::AGColor(Uint8 pr,Uint8 pg,Uint8 pb,Uint8 pa):
  mr(r),
  mg(g),
  mb(b),
  ma(a)*/
{
  r=pr;
  g=pg;
  b=pb;
  a=pa;
}
AGColor::AGColor(const AGColor &c):
  /*  r(c.r),
  g(c.g),
  b(c.b),*/
  a(c.a)
{
  r=c.r;
  g=c.g;
  b=c.b;
}

/*
SDL_Color AGColor::sdlColor() const
{
  SDL_Color c;
  c.r=mr;
  c.g=mg;
  c.b=mb;
}
*/

Uint32 AGColor::mapRGB(SDL_PixelFormat *f) const
{
  return SDL_MapRGBA(f,r,g,b,a);
}

std::string AGColor::toString() const
{
  std::ostringstream os;
  os<<"("<<int(r)<<","<<int(g)<<","<<int(b)<<")";
  return os.str();
}

AGColor &AGColor::operator=(const AGColor&c)
{
  r=c.r;
  g=c.g;
  b=c.b;
  a=c.a;
  return *this;
}

bool operator<(const AGColor &c1,const AGColor &c2)
{
  return c1.r<c2.r || (c1.r==c2.r && (c1.g<c2.g || (c1.g==c2.g && (c1.b<c2.b || (c1.b==c2.b && c1.a<c2.a)))));
}

std::ostream &operator<<(std::ostream &o,const AGColor &c)
{
  o<<"("<<(int)c.r<<","<<(int)c.g<<","<<(int)c.b<<","<<(int)c.a<<")";
  return o;
}

AGColor AGColor::grey() const
{
  Uint16 m=brightness();
  return AGColor(m,m,m,a);
}

Uint8 AGColor::brightness() const
{
  Uint16 m=r;
  m+=g;
  m+=b;
  m/=3;

  return m;
}

void AGColor::light(Uint8 l)
{
  r=(((Uint16)r)*l)>>8;
  g=(((Uint16)g)*l)>>8;
  b=(((Uint16)b)*l)>>8;
}

AGColor::AGColor(Uint32 c,const AGSurface &pSurface)
{
  SDL_GetRGBA(c,pSurface.s->format,&r,&g,&b,&a);
}
