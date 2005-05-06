/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_geometry.cpp
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

#include "ant_geometry.h"
#include "store.h"

///////////////////////////////////////////////////////////////////////////////
// Pos3D
///////////////////////////////////////////////////////////////////////////////

Pos3D::Pos3D():x(0),y(0),z(0)
{
}

Pos3D::Pos3D(float px,float py,float pz):
  x(px),y(py),z(pz)
{}

bool Pos3D::operator<(const Pos3D &p) const
{
  return x<p.x || (x==p.x && (y<p.y || (y==p.y && z<p.z)));
}

Pos3D Pos3D::operator+(const Pos3D &p) const
{
  return Pos3D(x+p.x,y+p.y,z+p.z);
}
Pos3D Pos3D::operator-(const Pos3D &p) const
{
  return Pos3D(x-p.x,y-p.y,z-p.z);
}

Pos3D Pos3D::operator*(float p) const
{
  return Pos3D(x*p,y*p,z*p);
}

void Pos3D::saveXML(xmlpp::Node &pNode) const
{
  pNode.set("x",toString(x));
  pNode.set("y",toString(y));
  pNode.set("z",toString(z));
}
void Pos3D::loadXML(const xmlpp::Node &pNode)
{
  x=toFloat(pNode.get("x"));
  y=toFloat(pNode.get("y"));
  z=toFloat(pNode.get("z"));
}
/*
see above
bool Pos3D::operator<(const Pos3D &p) const
{
  return x<p.x || (x==p.x && (y<p.y || (y==p.y && z<p.y)));
}*/


///////////////////////////////////////////////////////////////////////////////
// Pos2D
///////////////////////////////////////////////////////////////////////////////


  Pos2D::Pos2D(const Pos2D&p):x(p.x),y(p.y)
{
}

Pos2D::Pos2D():x(0),y(0)
{
}
  
Pos2D::Pos2D(float px,float py):
  x(px),y(py)
{
  //  cdebug(px<<"//"<<py);
}
std::string Pos2D::toString() const
{
  return ::toString((int)x)+";"+::toString((int)y);
}
std::string Pos2D::completeString() const
{
  return ::toString(x)+";"+::toString(y);
}


bool Pos2D::operator<(const Pos2D &p) const
{
  return x<p.x || (x==p.x && y<p.y);
}

Pos2D Pos2D::normalized() const
{
  //  CTRACE;
  float n=1.0/sqrt(x*x+y*y);
  //  cdebug(n);
  Pos2D pn(x*n,y*n);
  //  cdebug(pn);
  return pn;
}


float Pos2D::norm2() const
{
  return float(x*x+y*y);
}

float Pos2D::norm() const
{
  return sqrt(float(x*x+y*y));
}

Pos2D Pos2D::operator+(const Pos2D &p) const
{
  return Pos2D(x+p.x,y+p.y);
}
Pos2D Pos2D::operator-(const Pos2D &p) const
{
  return Pos2D(x-p.x,y-p.y);
}

Pos2D Pos2D::operator*(float f) const
{
  return Pos2D(x*f,y*f);
}
Pos2D Pos2D::operator/(float f) const
{
  return Pos2D(x/f,y/f);
}
