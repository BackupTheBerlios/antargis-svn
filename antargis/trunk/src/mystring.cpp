/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * mystring.cpp
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

#include "mystring.h"

#include <string>
#include <deque>

std::string replace(const std::string &s,const std::string &a,const std::string &b)
{
  std::string str=s;
  size_t i=str.find(a);
  while(i!=str.npos)
    {
      str=str.substr(0,i)+b+str.substr(i+a.length(),str.length()-i-a.length());
      i=str.find(a,i+b.length());
    }
  return str;
}
