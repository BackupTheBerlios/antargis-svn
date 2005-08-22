/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * store.h
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

#ifndef STORE_H
#define STORE_H

#include "mpng.h"
#include "ag_fs.h"

struct Filename
{
  std::string name;
  Filename(std::string filename):name(filename)
  {
  }
};

template<class T,class F>
  T convert(const F &f);

template<class T,class F>
  T convert(const F *f);

template<class T>
std::string convert(const T&t)
{
  return t.toString();
}

template<>
inline std::string convert(const SDL_Surface *ps)
{
  return toPNG(ps);
}

template<>
inline SDL_Surface *convert(const std::string &s)
{
  return fromPNG(s);
}

template<class R>
R convert(Filename f)
{
  return convert<R>(loadFile(f.name));
}

inline bool toBool(const std::string &s)
{
  return(s=="true");
}
inline int toInt(const std::string &s)
{
  return atoi(s.c_str());
}

inline float toFloat(const std::string &s)
{
  return atof(s.c_str());
}

/*template<class T>
std::string toString(const T&t)
{
  return convert<std::string>(t);
}
*/
inline float antRound(float a)
{
  return (int)a;
}


#endif
