/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_local.cc
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

#include "ag_local.h"


AGLocalizer *gLocalizer=0;

AGLocalizer::~AGLocalizer()
{
  if(gLocalizer==this)
    gLocalizer=0;
}
  
std::string AGLocalizer::find(const std::string &p)
{
  return p;
}

void setLocalizer(AGLocalizer *p)
{
  gLocalizer=p;
}

AGLocalizer *getLocalizer()
{
  return gLocalizer;
}


std::string translate(const std::string &s)
{
  AGLocalizer *l=getLocalizer();
  if(l)
    return l->find(s);
  return s;
}