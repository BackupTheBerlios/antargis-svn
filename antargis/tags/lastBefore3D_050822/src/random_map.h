/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * random_map.h
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

//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include<vector>
#include<algorithm>

class RandomMap
{
  std::vector<int> v;
  int mMax;
  int mVar;
  int mx,my;
  public:
  RandomMap();

  RandomMap(int pMax,int pVar=1);
  
  int get(int p) const;
  int get(int x,int y) const;
   
};
