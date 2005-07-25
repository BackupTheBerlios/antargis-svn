/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_triangle.cc
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

#include "ag_triangle.h"
#include "ag_tools.h"
#include "ag_debug.h"
#include <assert.h>
#include <cmath>
#include <list>

bool collide1d(float a1,float a2,float b1,float b2)
{
  float amin=std::min(a1,a2);
  float amax=std::max(a1,a2);
  float bmin=std::min(b1,b2);
  float bmax=std::max(b1,b2);

  if(amax==bmin || bmax==amin)
    return false;

  return (a1>=b1 && a1<=b2) || (a2>=b1 && a2<=b2) || (b1>=a1 && b1<=a2) || (b2>=a1 && b2<=a2);
}

/////////////////////////////////////////////////////////////////////////////
// AGAngle
/////////////////////////////////////////////////////////////////////////////

AGAngle::AGAngle(float p):angle(p)
{
}

/////////////////////////////////////////////////////////////////////////////
// AGVector
/////////////////////////////////////////////////////////////////////////////

AGVector::AGVector(float pX,float pY,float pZ)
{
  v[0]=pX;
  v[1]=pY;
  v[2]=pZ;
}
AGVector::AGVector(const AGVector &a)
{
  v[0]=a.v[0];
  v[1]=a.v[1];
  v[2]=a.v[2];
}
AGVector::AGVector(const AGAngle &a)
{
  v[0]=sin(a.angle);
  v[1]=-cos(a.angle);
  v[2]=0.0f;
}

AGVector::AGVector()
{
  v[0]=v[1]=0;
  v[2]=0.0f;
}


std::string AGVector::toString() const
{
  std::ostringstream os;
  os<<"("<<v[0]<<","<<v[1]<<","<<v[2]<<")";
  return os.str();
}

float AGVector::length2() const
{
  return v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
}


bool AGVector::nonZero() const
{
  return length2()!=0.0f;
}

AGVector AGVector::normal() const
{
  return AGVector(-v[1],v[0],v[2]);
}

void AGVector::setX(float pX)
{
  v[0]=pX;
}
void AGVector::setY(float pY)
{
  v[1]=pY;
}
void AGVector::setZ(float pZ)
{
  v[2]=pZ;
}

float AGVector::getX() const
{
  return v[0];
}
float AGVector::getY() const
{
  return v[1];
}

float AGVector::getZ() const
{
  return v[2];
}

(AGVector::operator float*)()
{
  return v;
}

AGVector AGVector::operator-(const AGVector &p) const
{
  return AGVector(v[0]-p.v[0],v[1]-p.v[1],v[2]-p.v[2]);
}
AGVector AGVector::operator+(const AGVector &p) const
{
  return AGVector(v[0]+p.v[0],v[1]+p.v[1],v[2]+p.v[2]);
}
AGVector &AGVector::operator+=(const AGVector &p)
{
  v[0]+=p.v[0];
  v[1]+=p.v[1];
  v[2]+=p.v[2];
  return *this;
}
AGVector &AGVector::operator-=(const AGVector &p)
{
  v[0]+=p.v[0];
  v[1]+=p.v[1];
  v[2]+=p.v[2];
  return *this;
}

float AGVector::operator*(const AGVector &p) const
{
  return v[0]*p.v[0]+v[1]*p.v[1]+v[2]*p.v[2];
}
AGVector &AGVector::operator*=(float f)
{
  v[0]*=f;
  v[1]*=f;
  v[2]*=f;
  return *this;
}
AGVector &AGVector::operator/=(float f)
{
  v[0]/=f;
  v[1]/=f;
  v[2]/=f;
  return *this;
}
AGVector AGVector::operator*(float f) const
{
  return AGVector(v[0]*f,v[1]*f,v[2]*f);
}
AGVector AGVector::operator/(float f) const
{
  f=1.0/f;
  return AGVector(v[0]*f,v[1]*f,v[2]*f);
}

float AGVector::length() const
{
  return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

AGVector AGVector::normalized() const
{
  if(length2()!=0.0f)
    return *this/length();
  else
    return *this;
}
void AGVector::normalize()
{
  if(length2()!=0.0f)
    operator/=(length());
}

float AGVector::operator[](int index) const
{
  assert(index>=0 && index<3);
  return v[index];
}


/////////////////////////////////////////////////////////////////////////////
// AGPoint
/////////////////////////////////////////////////////////////////////////////
AGPointF::AGPointF():AGVector()
{
}
AGPointF::AGPointF(float pX,float pY,float pZ):AGVector(pX,pY,pZ)
{
}

////////////////////////////////////////////////////////////////////////////
// AGMatrix
/////////////////////////////////////////////////////////////////////////////

AGMatrix::AGMatrix()
{
  a[0][1]=a[0][2]=
    a[1][0]=a[1][2]=
    a[2][0]=a[2][1]=0.0f;

  a[0][0]=a[1][1]=a[2][2]=1.0f;
}

AGMatrix::AGMatrix(const AGAngle &n)
{
  a[0][0]=cos(n.angle);
  a[1][0]=sin(n.angle);
  a[0][1]=-sin(n.angle);
  a[1][1]=cos(n.angle);
  a[2][0]=a[2][1]=a[0][2]=a[1][2]=0.0f;
  a[2][2]=1.0f;
}

AGMatrix::AGMatrix(const AGVector &n)
{
  a[0][0]=a[1][1]=a[2][2]=1.0f;
  a[0][1]=a[0][2]=
    a[1][0]=a[1][2]=
    a[2][0]=a[2][1]=0.0f;
  a[2][0]=n[0];
  a[2][1]=n[1];
}



void AGMatrix::set(size_t x,size_t y,float f)
{
  assert(x>=0 && x<3);
  assert(y>=0 && y<3);
  a[x][y]=f;
}
float AGMatrix::get(size_t x,size_t y) const
{
  assert(x>=0 && x<3);
  assert(y>=0 && y<3);
  return a[x][y];
}

AGMatrix AGMatrix::operator*(const AGMatrix &m) const
{
  AGMatrix n;
  size_t x,y;
  for(x=0;x<3;x++)
    for(y=0;y<3;y++)
      n.a[x][y]=a[0][y]*m.a[x][0]+
	a[1][y]*m.a[x][1]+
	a[2][y]*m.a[x][2];
  return n;
}
AGMatrix &AGMatrix::operator*=(const AGMatrix &m)
{
  *this=(*this)*m;
  return *this;
}

AGVector AGMatrix::operator*(const AGVector &v) const
{
  return AGVector(a[0][0]*v[0]+a[1][0]*v[1]+a[2][0]*v[2],
		  a[0][1]*v[0]+a[1][1]*v[1]+a[2][1]*v[2],
		  a[0][2]*v[0]+a[1][2]*v[1]+a[2][2]*v[2]);
}

std::string AGMatrix::toString() const
{
  std::ostringstream os;
  os<<"("<<a[0][0]<<","<<a[1][0]<<","<<a[2][0]<<")"<<endl;
  os<<"("<<a[0][1]<<","<<a[1][1]<<","<<a[2][1]<<")"<<endl;
  os<<"("<<a[0][2]<<","<<a[1][2]<<","<<a[2][2]<<")"<<endl;
  return os.str();
}

/////////////////////////////////////////////////////////////////////////////
// AGTriangle
/////////////////////////////////////////////////////////////////////////////

AGTriangle::AGTriangle(const AGVector &v0,const AGVector &v1,const AGVector &v2)
{
  p[0]=v0;
  p[1]=v1;
  p[2]=v2;
}

void AGTriangle::apply(const AGMatrix &m)
{
  p[0]=m*p[0];
  p[1]=m*p[1];
  p[2]=m*p[2];
}

AGTriangle AGTriangle::applied(const AGMatrix &m) const
{
  return AGTriangle(m*p[0],m*p[1],m*p[2]);
}

std::string AGTriangle::toString() const
{
  std::ostringstream os;
  os<<"["<<p[0].toString()<<" "<<p[1].toString()<<" "<<p[2].toString()<<"]";
  return os.str();
}

AGVector AGTriangle::get(int index) const
{
  return p[index];
}


AGVector AGTriangle::operator[](int index) const
{
  return p[index];
}

bool AGTriangle::collide(const AGTriangle &t) const
{
  std::list<AGVector> v=getNormals();
  append(v,t.getNormals());
  std::list<AGVector>::iterator i=v.begin();

  for(;i!=v.end();i++)
    {
      float min1,min2,max1,max2;
      size_t j;
      for(j=0;j<3;j++)
	{
	  if(j==0)
	    {
	      min1=max1=(*i)*p[j];
	      min2=max2=(*i)*t.p[j];
	    }
	  else
	    {
	      float v0=(*i)*p[j];
	      float v1=(*i)*t.p[j];
	      min1=std::min(min1,v0);
	      max1=std::max(max1,v0);
	      min2=std::min(min2,v1);
	      max2=std::max(max2,v1);
	    }
	}
      if(!collide1d(min1,max1,min2,max2))
	return false;
    }
  return true;
}

std::list<AGVector> AGTriangle::getNormals() const
{
  std::list<AGVector> l;
  l.push_back((p[1]-p[0]).normalized().normal());
  l.push_back((p[2]-p[1]).normalized().normal());
  l.push_back((p[0]-p[2]).normalized().normal());
  return l;
}

std::vector<AGLine> AGTriangle::getLines() const
{
  std::vector<AGLine> v;
  v.push_back(AGLine(p[0],p[1]));
  v.push_back(AGLine(p[1],p[2]));
  v.push_back(AGLine(p[2],p[0]));
  return v;
}

/////////////////////////////////////////////////////////////////////////////
// AGRectF
/////////////////////////////////////////////////////////////////////////////


AGRectF::AGRectF(const AGVector &pv0,const AGVector &pv1):
  v0(pv0),v1(pv1)
{
}

bool AGRectF::collide(const AGRectF &r) const
{
  return collide1d(v0.getX(),v1.getX(),r.v0.getX(),r.v1.getX()) && collide1d(v0.getY(),v1.getY(),r.v0.getY(),r.v1.getY());
}

AGRectF AGRectF::operator+(const AGVector &v) const
{
  return AGRectF(v0+v,v1+v);
}

/////////////////////////////////////////////////////////////////////////////
// AGLine
/////////////////////////////////////////////////////////////////////////////

AGLine::AGLine()
{
}

AGLine::AGLine(const AGVector &pv0,const AGVector &pv1):
  v0(pv0),v1(pv1)
{
}
  
AGVector AGLine::getV0() const
{
  return v0;
}
AGVector AGLine::getV1() const
{
  return v1;
}

std::string AGLine::toString() const
{
  std::ostringstream os;
  os<<"("<<v0.toString()<<"-"<<v1.toString()<<")";
  return os.str();
}
