/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_debug.cc
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

#include "ag_debug.h"

#include <fstream>
#include <iostream>

#ifndef NDEBUG
int D::d=0;

std::ofstream debugOFS("debug.txt");

std::ostream &getDebug()
{
  return std::cout;
}

#endif

std::vector<std::string> split(const std::string &needle,const std::string &haystack)
{
  std::vector<std::string> d;
  size_t i,last=0;
  for(i=haystack.find(needle);i!=std::string::npos;i=haystack.find(needle,i+1))
    {
      d.push_back(haystack.substr(last,i-last));
      last=i+needle.length();
    }
  i=haystack.length();
  d.push_back(haystack.substr(last,i-last));
  return d;
}



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
