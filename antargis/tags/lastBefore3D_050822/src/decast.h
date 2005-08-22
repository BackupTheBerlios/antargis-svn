/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * decast.h
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

#ifndef DECAST_H
#define DECAST_H

#include <vector>
#include <sstream>
#include <map>


#include "ag_debug.h"
#include "ant_geometry.h"
#include "store.h"
#include "md5.h"
#include "random_map.h"


#define HIGH_VOXEL_DETAIL
#ifdef HIGH_VOXEL_DETAIL
#define MAP_SPLINE_ORDER 3
#endif


#define P(x,y) std::make_pair(x,y)

class Bitmap:public std::vector<float>
{
 public:
  int w,h;
 public:
  Bitmap();
  Bitmap(int W,int H);
  Bitmap(const Bitmap &b);
  
  float &operator[](std::pair<int,int> p);
  
  std::string toString() const;
  
  const float &operator[](std::pair<int,int> p) const;
};

/*
  0   1   2   3   4   5
  |---|---|---|---|---|
  ........|--x|
  ....|------x|
  ........|--x----|
  |----------x|
  ....|------x----|
  ........|--x--------|
*/

/** (a0,a1,a2,a3) are at (-1,0,1,2) and x is in [0,1) */
float spline(float x,float a0,float a1,float a2,float a3);

float spline_derive(float x,float a0,float a1,float a2,float a3);

float spline(float x,float a0,float a1,float a2);


float bezier(float x,float a0,float a1,float a2,float a3);

template<class T>
T bezier(float x,const T&a0,const T& a1,const T& a2);
/*{
  T c0=a0*(1.0f-x)+a1*x;
  T c1=a1*(1.0f-x)+a2*x;

  return c0*(1.0f-x)+c1*x;
  }*/

float bezier(float x,float a0,float a1);

float bezier_derive(float x,float a0,float a1,float a2);

float bezier_derive(float x,float a0,float a1);

float catmull_rom(float t,float a0,float a1,float a2,float a3);


// order for cubic is 4
// order for quadratic is 3

// Cubic Uniform Spline map
class SplineMap
{
  Bitmap values;
  int w,h;
  RandomMap rMap;
  bool realRandom;
 public:
  SplineMap();
  SplineMap(int W,int H,int tile,int pmin,int pmax,bool rrand=false);
  SplineMap(const Bitmap&b);
  
  int width() const;
  int height() const;
  
  std::string toString() const;
  
  float getPoint(int x,int y) const;
  
  void setPoint(int x,int y,const float&t);
  void edit(int x,int y,int v);
  
  std::string nice() const;
  
  SplineMap getPatch(float px,float py,int pw,int ph) const;
  
  Pos3D getNormal(float x,float y) const;
  float dy(float x,float y) const;
  float dx(float x,float y) const;

  float get(float x,float y) const;
  
};


#endif
