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

/* There are following classes:

   We do NOT use templates here, because of SWIG-problems.

   - AGVector2 - 2d vector
   - AGVector3 - is homogenous 2d and inhom 3d
   - AGPoint3  - is 2d (with z=1) or 3d
   - AGVector4 - hom. 3d
   - AGMatrix3 - 3x3 Matrix
   - AGMatrix4 - 4x4 Matrix

   - AGLine2     - 2d Line (eats AGVector3)
   - AGTriangle2 - 2d triangle (eats AGVector3)

   ATTENTION: the classes may NOT be virtual, as they're possibly used in an array, where then the size is NOT sizeof(v[x]) !!!
*/


class AGRect2;
class AGBox3;
class AGRect3;
class Node;

struct AGAngle
{
  float angle;
  
  explicit AGAngle(float a);
};

class AGVector2
{
 public:
  float v[2];

  AGVector2(float pX,float pY);
  AGVector2(const AGAngle &a);
  AGVector2(const AGVector2 &a);
  AGVector2();

  void setX(float pX);
  void setY(float pY);

  
#ifdef SWIG
  %rename(x) getX() const;
  %rename(y) getY() const;
#endif
  float getX() const;
  float getY() const;

  AGAngle getAngle() const;

  AGVector2 operator-(const AGVector2 &p) const;
  AGVector2 operator+(const AGVector2 &p) const;
  AGVector2 &operator+=(const AGVector2 &p);
  AGVector2 &operator-=(const AGVector2 &p);

  float operator*(const AGVector2 &p) const;
  AGVector2 &operator*=(float f);
  AGVector2 &operator/=(float f);
  AGVector2 operator*(float f) const;
  AGVector2 operator/(float f) const;

  bool operator==(const AGVector2 &a) const;
  bool operator!=(const AGVector2 &a) const;

  float length() const;
  float length2() const;

  AGVector2 normalized() const;
  void normalize();

  AGVector2 normal() const;

  float operator[](int index) const;

  bool nonZero() const;

  void saveXML(Node &node) const;
  void loadXML(const Node &node);

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;

#ifndef SWIG
  (operator float*)();
#endif
};

// 2-dim homogenous vector
class AGVector3
{
 public:
  float v[3];

  AGVector3(const AGVector2 &p,float h=1.0f);
  AGVector3(float pX,float pY,float pZ=0.0f);
  AGVector3(const AGAngle &a);
  AGVector3(const AGVector3 &a);
  AGVector3();

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

  AGVector3 operator-() const;

  AGVector3 operator-(const AGVector3 &p) const;
  AGVector3 operator+(const AGVector3 &p) const;
#ifdef SWIG
%rename(add) operator+=(const AGVector3 &p);
%rename(sub) operator-=(const AGVector3 &p);
#endif
  AGVector3 &operator+=(const AGVector3 &p);
  AGVector3 &operator-=(const AGVector3 &p);

  float operator*(const AGVector3 &p) const;
  AGVector3 &operator*=(float f);
  AGVector3 &operator/=(float f);
  AGVector3 operator*(float f) const;
  AGVector3 operator/(float f) const;
  AGVector3 operator%(const AGVector3 &a) const;

  bool operator==(const AGVector3 &a) const;
  bool operator!=(const AGVector3 &a) const;

  float length() const;
  float length2() const;

  AGVector3 normalized() const;
  void normalize();

  AGVector3 normal() const;

  float operator[](int index) const;
  float &operator[](int index);

  bool nonZero() const;

  void saveXML(Node &node) const;
  void loadXML(const Node &node);

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;

#ifndef SWIG
  (operator float*)();
#endif
};

class AGPoint3:public AGVector3
{
 public:
  AGPoint3();
  AGPoint3(float pX,float pY,float pZ=1.0f);
  AGPoint3(const AGVector3 &p);

  AGPoint3&operator=(const AGVector3 &v);
};


class AGMatrix3
{
  float a[3][3];
 public:
  struct Row
  {
    AGMatrix3 *matrix;
    int y;

    float &operator[](size_t x)
    {
      return matrix->get(x,y);
    }
    float operator[](size_t x) const
    {
      return matrix->get(x,y);
    }
  };

  AGMatrix3();
  AGMatrix3(const AGAngle &a);
  AGMatrix3(const AGVector3 &a);
  void set(size_t x,size_t y,float f);
  float get(size_t x,size_t y) const;
  float &get(size_t x,size_t y);

  AGMatrix3 operator*(const AGMatrix3 &m) const;
  AGMatrix3 &operator*=(const AGMatrix3 &m);

  AGVector3 operator*(const AGVector3 &v) const;

  AGMatrix3 operator-() const;

  AGMatrix3 inverted() const;
  AGMatrix3 transposed() const;

  Row operator[](size_t y);
  const Row operator[](size_t y) const;

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;
};

class AGLine2
{
  AGVector3 v0,v1;
 public:
  AGLine2();
  AGLine2(const AGVector3 &pv0,const AGVector3 &pv1);
  
  AGVector3 getV0() const;
  AGVector3 getV1() const;

  bool has(const AGVector3 &v) const;

  bool collide(const AGLine2 &l) const;
  AGVector3 collisionPoint(const AGLine2 &l) const;
  AGVector3 collisionPointNI(const AGLine2 &l) const; // no inclusion test
  bool includes(const AGVector3 &v) const;

  AGRect2 getBBox() const;

  AGVector3 normal() const;
  AGVector3 direction() const;

  float distance(const AGVector3 &v) const;

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;
};

class AGLine3
{
  AGVector3 v0,v1;
 public:
  AGLine3();
  AGLine3(const AGVector3 &pv0,const AGVector3 &pv1);
  
  AGVector3 getV0() const;
  AGVector3 getV1() const;

  bool has(const AGVector3 &v) const;
  /*
  bool collide(const AGLine3 &l) const;
  AGVector3 collisionPoint(const AGLine3 &l) const;
  AGVector3 collisionPointNI(const AGLine3 &l) const; // no inclusion test
  bool includes(const AGVector3 &v) const;
  */
  //  AGBox3 getBBox() const;

  //  AGVector3 normal() const;
  AGVector3 direction() const;

  float distance(const AGVector3 &p) const;

  //  float distance(const AGVector3 &v) const;

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;
};

class AGCollisionData
{
};

class AGRect2;

class AGTriangle2
{
  AGPoint3 p[3];
 public:
  AGTriangle2(const AGVector3 &v0,const AGVector3 &v1,const AGVector3 &v2);

  AGPoint3 operator[](int index) const;

  AGPoint3 get(int index) const;

  bool collide(const AGTriangle2 &t) const;

  // FIXME: sweep-based test
  //  AGCollisionData collide(const AGTriangle &t,const AGVector &v0,const AGVector &v1) const;

  std::vector<AGVector3> getNormals() const;

  void apply(const AGMatrix3 &m);
  AGTriangle2 applied(const AGMatrix3 &m) const;

  // estimate touching point (or middle of touching area)
  AGPoint3 touchPoint(const AGTriangle2 &t) const;
  AGVector3 touchVector(const AGTriangle2 &t) const;

  std::vector<AGVector3> collisionPoints(const AGLine2 &l) const;

  bool contains(const AGPoint3 &p) const;

  AGRect2 getBBox() const;

  AGLine2 nearestLine(const AGVector3 &v) const;

  std::vector<AGLine2> getLines() const;
#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;
};

class AGCircle2
{
  AGVector3 p;
  float radius;
 public:
  AGCircle2(const AGVector3 &v,float rad);

  bool inCircle(const AGTriangle2 &t) const;
  bool outCircle(const AGTriangle2 &t) const;
  bool inCircle(const AGVector3 &v) const;
  bool outCircle(const AGVector3 &v) const;

  AGVector3 getPos() const;
  float getRadius() const;

};

class AGVector4;
class AGTriangle3
{
  AGVector3 p[3];
 public:
  AGTriangle3(const AGVector3 &v0,const AGVector3 &v1,const AGVector3 &v2);
  
  // (x,y,z,0) for no collision 
  // (x,y,z,1) for collision in point (x,y,z)
  AGVector4 collide(const AGLine3 &pLine) const;

  std::string toString() const;

  AGVector3 operator[](int index) const;
};

class AGRect2
{
  AGVector3 v0,v1;
 public:
  AGRect2(const AGVector3 &pv0,const AGVector3 &pv1);
  AGRect2(const AGVector2 &pv0,const AGVector2 &pv1);
  AGRect2(float x,float y,float w,float h);

  bool collide(const AGRect2 &r) const;

  void include(const AGVector2 &v);

  AGRect2 operator+(const AGVector3 &v) const;

  bool contains(const AGVector3 &v) const;
  bool contains(const AGRect2 &v) const;

  std::list<AGRect2> split() const;

  AGVector3 getV0() const;
  AGVector3 getV1() const;
  AGVector3 getV01() const;
  AGVector3 getV10() const;


  void setX(float p);
  void setY(float p);

  float x() const;
  float y() const;
  float w() const;
  float h() const;

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;
};


class AGVector4
{
 public:
  float v[4];

  AGVector4(float pX,float pY,float pZ,float pW=1.0f);
  AGVector4(const AGVector4 &a);
  AGVector4(const AGVector3 &a,float h);
  AGVector4();

  void setX(float pX);
  void setY(float pY);
  void setZ(float pZ);
  void setW(float pW);

  
#ifdef SWIG
  %rename(x) getX() const;
  %rename(y) getY() const;
  %rename(z) getZ() const;
  %rename(w) getW() const;
#endif
  float getX() const;
  float getY() const;
  float getZ() const;
  float getW() const;

  AGVector4 operator-(const AGVector4 &p) const;
  AGVector4 operator+(const AGVector4 &p) const;
#ifdef SWIG
%rename(add) operator+=(const AGVector4 &p);
%rename(sub) operator-=(const AGVector4 &p);
#endif
  AGVector4 &operator+=(const AGVector4 &p);
  AGVector4 &operator-=(const AGVector4 &p);

  float operator*(const AGVector4 &p) const;
  AGVector4 &operator*=(float f);
  AGVector4 &operator/=(float f);
  AGVector4 operator*(float f) const;
  AGVector4 operator/(float f) const;
  AGVector4 operator-() const;

  bool operator==(const AGVector4 &a) const;
  bool operator!=(const AGVector4 &a) const;

  float length() const;
  // length^2
  float length2() const;

  // length 3-dim vector
  float length3() const;


  AGVector4 normalized() const;
  void normalize();

  // 3-dimensional
  AGVector4 normalized3() const;
  void normalize3();


  float operator[](int index) const;
  float &operator[](int index);

  bool nonZero() const;

  // vector-cross-product
  // ignores 4th coordinate
  AGVector4 operator%(const AGVector4 &a) const;
  AGVector4 cross(const AGVector4 &a) const
  {
    return operator%(a);
  }


  AGVector3 dim3() const;
  AGVector2 dim2() const;

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;

#ifndef SWIG
  (operator float*)();
  (operator const float*)() const;
#endif
};

class AGMatrix4
{
  float a[16];
 public:
  struct MRow
  {
    AGMatrix4 *matrix;
    int y;

    float &operator[](size_t x)
    {
      return matrix->get(x,y);
    }
    float operator[](size_t x) const
    {
      return matrix->get(x,y);
    }
  };

  AGMatrix4();
  AGMatrix4(float v[16]);
  AGMatrix4(float angle,const AGVector3 &d);
  // FIXME: add quaternion
  //  AGMatrix4(const AGAngle &a);
  AGMatrix4(const AGVector4 &a);
  void set(size_t x,size_t y,float f);
  float get(size_t x,size_t y) const;
  float &get(size_t x,size_t y);

  AGMatrix4 operator*(const AGMatrix4 &m) const;
  AGMatrix4 &operator*=(const AGMatrix4 &m);
  AGMatrix4 &operator+=(const AGMatrix4 &m);

  AGMatrix3 get3x3(size_t x,size_t y) const;

  AGVector4 operator*(const AGVector4 &v) const;

  float operator()(size_t x,size_t y) const;
  float &operator()(size_t x,size_t y);

  AGMatrix4 inverted() const;
  AGMatrix4 transposed() const;

  MRow operator[](size_t y);
  //  const Row operator[](size_t y) const;

#ifdef SWIG
  %rename(to_s) toString() const;
#endif
  std::string toString() const;

#ifndef SWIG
  (operator float*)();
  (operator const float*)() const;
#endif

  AGVector4 getRow(size_t i) const;
};

// axis-aligned
class AGRect3
{
  AGVector3 base,dir;
 public:
  AGRect3(const AGVector3 &pBase,const AGVector3 &pDir);

  bool collides(const AGLine3&pLine) const;

  std::string toString() const;
};

// axis-aligned
class AGBox3
{
 public:
  AGVector3 base,dir;

  AGBox3();
  AGBox3(const AGVector3 &pBase,const AGVector3 &pDir);

  void include(const AGVector3&p);
  bool includes(const AGBox3 &b);
  bool collides(const AGVector3&p) const;

  bool collides(const AGLine3&p) const;
  bool collides(const AGBox3 &b) const;

  std::vector<AGRect3> getSides() const;

  std::string toString() const;

  std::vector<AGBox3> split() const;

  bool collides(const AGMatrix4 &frustum) const;

  std::vector<AGVector4> getVertices() const;

  AGBox3 operator+(const AGVector3 &v) const;
};


#endif
