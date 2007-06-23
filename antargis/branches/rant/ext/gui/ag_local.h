/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_local.h
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

#ifndef AG_LOCAL_H
#define AG_LOCAL_H

#include <ag_string.h>
#include <ag_string_utf8.h>

class AGLocalizer
{
 public:
  virtual ~AGLocalizer();
  
  virtual AGStringUtf8 find(const AGString &p);
  
};

void setLocalizer(AGLocalizer *p);
AGLocalizer *getLocalizer();

AGStringUtf8 translate(const AGString &s);

#ifndef SWIG
#ifdef _
#undef _
#endif
#ifdef __
#undef __
#endif
#define _(x) translate(x)
#define __(x) translate(x)
#endif

#endif
