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
#include <map>
#include <list>
#include <ruby.h>

#define MAXF 100000.0f

bool collide1d(float a1,float a2,float b1,float b2, bool normal=true)
{
  float amin=std::min(a1,a2);
  float amax=std::max(a1,a2);
  float bmin=std::min(b1,b2);
  float bmax=std::max(b1,b2);

  if(normal && (amax==bmin || bmax==amin))
    return false;

  return (amin>=bmin && amin<=bmax) || (amax>=bmin && amax<=bmax) || (bmin>=amin && bmin<=amax) || (bmax>=amin && bmax<=amin);
}

struct AGSweepResult
{
  enum CollisionState { COL_AT, COL_ALWAYS, COL_NEVER };
  CollisionState state;
  float t0;
  float t1;

  AGSweepResult()
  {
    state=COL_ALWAYS;
  }

  void combine(const AGSweepResult &r)
  {
    if(state==COL_AT)
      {
	if(r.state==COL_NEVER)
	  state=COL_NEVER;
	else if(r.state!=COL_ALWAYS) // ALWAYS has no effect
	  {
	    t0=std::max(t0,r.t0);
	    t1=std::min(t1,r.t1);
	  }
      }
    else if(state==COL_ALWAYS)
      {
	if(r.state==COL_NEVER)
	  state=COL_NEVER;
	else if(r.state!=COL_ALWAYS) // ALWAYS has no effect
	  {
	    state=COL_AT;
	    t0=r.t0;
	    t1=r.t1;
	  }
	
      }
    // COL_NEVER cannot be changed
  }

  bool collision(float delta) const
  {
    return state==COL_ALWAYS || (state==COL_AT && t1>0 && t0<delta);
  }

  float begin(float delta) const
  {
    if(state==COL_ALWAYS || state==COL_NEVER)
      return MAXF;

    if(t0>=0.0f && t0<delta)
      return t0;
    else
      return MAXF;
  }

  bool always() const
  {
    return state == COL_ALWAYS;
  }
};

AGSweepResult collide1d(float pa1,float pa2,float pb1,float pb2,float v)
{
  float a1=std::min(pa1,pa2);
  float a2=std::max(pa1,pa2);
  float b1=std::min(pb1,pb2);
  float b2=std::max(pb1,pb2);
  AGSweepResult r;

  if(v>0)
    {
      r.t0=(b1-a2)/v;
      r.t1=(b2-a1)/v;
      r.state = AGSweepResult::COL_AT;

      assert(r.t0 <= r.t1);
    }
  else if(v<0)
    {
      r.t0=(b2-a1)/v;
      r.t1=(b1-a2)/v;
      r.state = AGSweepResult::COL_AT;

      assert(r.t0 <= r.t1);
    }
  else
    {
      if (a2 < b1 || a1 > b2)
        r.state = AGSweepResult::COL_NEVER;
      else
        r.state = AGSweepResult::COL_ALWAYS;
    }
  return r;
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

AGVector::~AGVector()
{
}


float getArcInternal(float x,float y)
{
  if(y==0.0)
    {
      if(x<0.0)
	return -M_PI/2.0;
      else
	return M_PI/2.0;
    }
  else if(y<0.0)
    {
      float a=M_PI+atan(x/y);
      if(a>M_PI)
	a-=M_PI*2.0;
      return a;
    }
  else
    return atan(x/y);
}


AGAngle AGVector::getAngle() const
{
  return AGAngle(getArcInternal(v[0],v[1]));
}


bool AGVector::operator==(const AGVector &a) const
{
  return v[0]==a.v[0] && v[1]==a.v[1] && v[2]==a.v[2];
}
bool AGVector::operator!=(const AGVector &a) const
{
  return !operator==(a);
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
AGPointF::AGPointF(const AGVector &a)
{
  assert(a.v[2]);
  v[0]=a.v[0]/a.v[2];
  v[1]=a.v[1]/a.v[2];
  v[2]=1.0f;
}


AGPointF& AGPointF::operator=(const AGVector &a)
{
  if(a.v[2]==0.0f)
    {
      cdebug("ERROR in conversion :"<<a[0]<<","<<a[1]<<","<<a[2]);
      rb_raise(rb_str_new2("Vec2Point error"),"");
      v[0]=a.v[0];
      v[1]=a.v[1];
      v[2]=a.v[2];
    }
  else
    {
      v[0]=a.v[0]/a.v[2];
      v[1]=a.v[1]/a.v[2];
      v[2]=1.0f;
    }
  return *this;
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

/* FIXME: this will be some sweep-base collision detection,
   HOWEVER there's the angular-velocity still missing :-(

AGCollisionData AGTriangle::collide(const AGTriangle &t,const AGVector &v0,const AGVector &v1) const
{
  AGCollisionData data;

  AGVector v=v1-v0;

  float t0,t1;

  std::vector<AGVector> v=getNormals();
  append(v,t.getNormals());
  std::vector<AGVector>::iterator i=v.begin();
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
      
      float a1=
    }
}
*/

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

AGPointF AGTriangle::get(int index) const
{
  return p[index];
}


AGPointF AGTriangle::operator[](int index) const
{
  return p[index];
}

bool AGTriangle::collide(const AGTriangle &t) const
{
  std::vector<AGVector> v=getNormals();
  append(v,t.getNormals());
  std::vector<AGVector>::iterator i=v.begin();

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

std::vector<AGVector> AGTriangle::getNormals() const
{
  std::vector<AGVector> l;
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

bool AGTriangle::contains(const AGPointF &pp) const
{
  std::vector<AGVector> l=getNormals(); // BEWARE: dont' change the order in getNormals!!!

  if(AGsign((pp-p[0])*l[0])==AGsign((p[2]-p[0])*l[0]))
    if(AGsign((pp-p[1])*l[1])==AGsign((p[0]-p[1])*l[1]))
      if(AGsign((pp-p[2])*l[2])==AGsign((p[1]-p[2])*l[2]))
	return true;
  return false;
}


AGPointF AGTriangle::touchPoint(const AGTriangle &t) const
{
  // assume that one point of a triangle is contained in the other and return this point
  size_t i;
  for(i=0;i<3;i++)
    {
      if(contains(t[i]))
	return t[i];
      else if(t.contains((*this)[i]))
	return (*this)[i];
    }
  return AGPointF(0,0,0);
}

AGVector AGTriangle::touchVector(const AGTriangle &t) const
{
  //#ifdef OLD_TOUCH_APPROXIMATION
  // try to get line, which has two intersections with other triangle's lines

  std::vector<AGLine> l0=getLines();
  std::vector<AGLine> l1=t.getLines();

  std::vector<AGLine>::iterator i,j;

  std::map<AGLine*,int> sum;

  for(i=l0.begin();i!=l0.end();i++)
    for(j=l1.begin();j!=l1.end();j++)
      {
	if(i->collide(*j))
	  {
	    sum[&(*i)]++;
	    sum[&(*j)]++;
	  }
      }
  std::map<AGLine*,int>::iterator k=sum.begin();
  for(;k!=sum.end();k++)
    {
      if(k->second==2)
	return k->first->normal();
    }

  // FIXME: not found - find other approximation

  //#else
  {
  size_t i;
  for(i=0;i<3;i++)
    {
      if(contains(t[i]))
	return nearestLine(t[i]).normal();
      else if(t.contains((*this)[i]))
	return t.nearestLine((*this)[i]).normal();
    }
  return AGVector();
  }
  //#endif
}

AGLine AGTriangle::nearestLine(const AGVector &v) const
{
  std::vector<AGLine> l=getLines();
  std::vector<AGLine>::iterator i;
  std::map<float,AGLine> dist;
  for(i=l.begin();i!=l.end();i++)
    {
      dist[i->distance(v)]=*i;
      //      cdebug(i->distance(v)<<i->toString());
    }
  return dist.begin()->second;
}


/////////////////////////////////////////////////////////////////////////////
// AGRectF
/////////////////////////////////////////////////////////////////////////////


AGRectF::AGRectF(const AGVector &pv0,const AGVector &pv1):
  v0(pv0),v1(pv1)
{
}

AGRectF::AGRectF(float x,float y,float w,float h):
  v0(x,y,1),v1(x+w,y+h,1)
{
}

void AGRectF::setX(float p)
{
  float mw=w();
  v0.setX(p);
  v1.setX(p+mw);
}
void AGRectF::setY(float p)
{
  float mh=h();
  v0.setY(p);
  v1.setY(p+mh);
}


bool AGRectF::collide(const AGRectF &r) const
{
  return collide1d(v0.getX(),v1.getX(),r.v0.getX(),r.v1.getX()) && collide1d(v0.getY(),v1.getY(),r.v0.getY(),r.v1.getY());
}

AGRectF AGRectF::operator+(const AGVector &v) const
{
  return AGRectF(v0+v,v1+v);
}

float AGRectF::x() const
{
  return v0.getX();
}
float AGRectF::y() const
{
  return v0.getY();
}
float AGRectF::w() const
{
  return v1.getX()-v0.getX();
}
float AGRectF::h() const
{
  return v1.getY()-v0.getY();
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

bool AGLine::has(const AGVector &v) const
{
  return v0==v || v1==v;
}

bool AGLine::collide(const AGLine &l) const
{
  AGVector d0,d1;
  AGVector n0,n1;

  d0=direction();
  d1=l.direction();
  n0=normal();
  n1=l.normal();
  
  bool t0=collide1d(v0*d0,v1*d0,l.v0*d0,l.v1*d0,false);
  bool t1=collide1d(v0*d1,v1*d1,l.v0*d1,l.v1*d1,false);

  bool t2=collide1d(v0*n0,v1*n0,l.v0*n0,l.v1*n0,false);
  bool t3=collide1d(v0*n1,v1*n1,l.v0*n1,l.v1*n1,false);

  return t0 && t1 && t2 && t3;
}

AGVector AGLine::normal() const
{
  return (v1-v0).normalized().normal();
}

AGVector AGLine::direction() const
{
  return v1-v0;
}



std::string AGLine::toString() const
{
  std::ostringstream os;
  os<<"("<<v0.toString()<<"-"<<v1.toString()<<")";
  return os.str();
}

float AGLine::distance(const AGVector &v) const
{
  return fabs(normal()*(v-v0));
}
