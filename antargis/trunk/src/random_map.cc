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

#include "random_map.h"

RandomMap mRandomMapX(99,99);
RandomMap mRandomMapY(99,99);

RandomMap::RandomMap()
    {
      mMax=mVar=1;
      mx=47;
      my=13;
    }
RandomMap::RandomMap(int pMax,int pVar):mMax(pMax),mVar(pVar)
  {
    mx=47;
    my=13;
    for(int i=0;i<mMax*mVar;i++)
      v.push_back(i);
    std::random_shuffle(v.begin(),v.end());
  }
  
int RandomMap::get(int p) const
  {
    p%=mMax*mVar;
    if(p>=0 && p<mMax*mVar)
      return v[p]%mMax;
    return -1;
  }

int RandomMap::get(int x,int y) const
{
  //  int p=x+y;//mRandomMapX.get(x)+mRandomMapY.get(y);
  return get(get(x)+get(y));
}
