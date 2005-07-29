/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_triangle.h
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

#ifndef AG_TRIANGLE_H
#define AG_TRIANGLE_H

#include <SDL.h>
#include <string>
#include <iostream>
#include <list>
#include <vector>

struct AGAngle
{
  float angle;
  
  explicit AGAngle(float a);
};

// 2-dim homogenous vector
class AGVector
{
 public:
  float v[3];

  AGVector(float pX,float pY,float pZ=0.0f);
  AGVector(const AGAngle &a);
  AGVector(const AGVector &a);
  AGVector();
  virtual ~AGVector();

  void setX(float pX);
  void setY(float pY);
  void setZ(float pZ);

  
#ifdef SWIG
  %rename(x) getX() const;
  %rename(y) getY() const;
  %rename(z) getZ() const;
#endif
  float getX() const;
  float getY() const;
  float getZ() const;

  AGAngle getAngle() const;

  AGVector operator-(const AGVector &p) const;
  AGVector operator+(const AGVector &p) const;
  AGVector &operator+=(const AGVector &p);
  AGVector &operator-=(const AGVector &p);

  float operator*(const AGVector &p) const;
  AGVector &operator*=(float f);
  AGVector &operator/=(float f);
  AGVector operator*(float f) const;
  AGVector operator/(float f) const;

  bool operator==(const AGVector &a) const;
  bool operator!=(const AGVector &a) const;

  float length() const;
  float length2() const;

  AGVector normalized() const;
  void normalize();

  AGVector normal() const;

  float operator[](int index) const;

  bool nonZero() const;

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;

#ifndef SWIG
  (operator float*)();
#endif
};

class AGPointF:public AGVector
{
 public:
  AGPointF();
  AGPointF(float pX,float pY,float pZ=1.0f);
  AGPointF(const AGVector &p);

  AGPointF&operator=(const AGVector &v);
};

class AGMatrix
{
  float a[3][3];
 public:
  AGMatrix();
  AGMatrix(const AGAngle &a);
  AGMatrix(const AGVector &a);
  void set(size_t x,size_t y,float f);
  float get(size_t x,size_t y) const;

  AGMatrix operator*(const AGMatrix &m) const;
  AGMatrix &operator*=(const AGMatrix &m);

  AGVector operator*(const AGVector &v) const;

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;
};

class AGLine
{
  AGVector v0,v1;
 public:
  AGLine();
  AGLine(const AGVector &pv0,const AGVector &pv1);
  
  AGVector getV0() const;
  AGVector getV1() const;

  bool has(const AGVector &v) const;

  bool collide(const AGLine &l) const;

  AGVector normal() const;
  AGVector direction() const;

  float distance(const AGVector &v) const;

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;
};

class AGCollisionData
{
};

class AGTriangle
{
  AGPointF p[3];
 public:
  AGTriangle(const AGVector &v0,const AGVector &v1,const AGVector &v2);

  AGPointF operator[](int index) const;

  AGPointF get(int index) const;

  bool collide(const AGTriangle &t) const;

  // FIXME: sweep-based test
  //  AGCollisionData collide(const AGTriangle &t,const AGVector &v0,const AGVector &v1) const;

  std::vector<AGVector> getNormals() const;

  void apply(const AGMatrix &m);
  AGTriangle applied(const AGMatrix &m) const;

  // estimate touching point (or middle of touching area)
  AGPointF touchPoint(const AGTriangle &t) const;
  AGVector touchVector(const AGTriangle &t) const;

  bool contains(const AGPointF &p) const;

  AGLine nearestLine(const AGVector &v) const;

  std::vector<AGLine> getLines() const;
#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;
};

class AGRectF
{
  AGVector v0,v1;
 public:
  AGRectF(const AGVector &pv0,const AGVector &pv1);
  AGRectF(float x,float y,float w,float h);

  bool collide(const AGRectF &r) const;

  AGRectF operator+(const AGVector &v) const;

  void setX(float p);
  void setY(float p);

  float x() const;
  float y() const;
  float w() const;
  float h() const;
};

#endif
