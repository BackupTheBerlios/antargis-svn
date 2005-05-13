/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * mystring.h
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

#ifndef __mystring_h
#define __mystring_h

#include <string>
#include <deque>

#include "ag_debug.h"

//std::deque<std::string> split(const std::string &needle,const std::string &haystack);
std::string replace(const std::string &s,const std::string &a,const std::string &b);

#endif
