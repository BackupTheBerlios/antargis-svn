#ifndef ANT_GEOMETRY_H
#define ANT_GEOMETRY_H

#include <iostream>
#include "serialize.h"
#include "xml.h"

struct Pos3D
{
  float x,y,z;

  Pos3D(float px,float py,float pz);

  bool operator<(const Pos3D &p) const;
  Pos3D operator+(const Pos3D &p) const;
  Pos3D operator-(const Pos3D &p) const;

  Pos3D operator*(float p) const;

  float operator*(const Pos3D &p) const
  {
    return x*p.x + y*p.y + z*p.z;
  }

  Pos3D normalized() const
  {
    float n=1.0/sqrt(x*x+y*y+z*z);
    return Pos3D(x*n,y*n,z*n);
  }

  Pos3D operator%(const Pos3D &p) const
  {
    return Pos3D(y*p.z - z*p.y,
		 z*p.x - x*p.z,
		 x*p.y - y*p.x);
  }

  bool operator==(const Pos3D &p) const
  {
    return x==p.x && y==p.y && z==p.z;
  }
  void saveXML(xmlpp::Node &pNode) const;
  void loadXML(const xmlpp::Node &pNode);
};


struct Matrix3
{
  float a[9];

  Pos3D operator*(const Pos3D&p)
  {
    return Pos3D(a[0]*p.x+a[1]*p.y+a[2]*p.z,
		 a[3]*p.x+a[4]*p.y+a[5]*p.z,
		 a[6]*p.x+a[7]*p.y+a[8]*p.z);
  }

  Matrix3 operator*(float f) const
  {
    Matrix3 m=*this;
    for(int i=0;i<9;i++)
      m.a[i]=a[i]*f;
    return m;
  } 

  Matrix3 operator+(const Matrix3&p) const
  {
    Matrix3 m=*this;
    for(int i=0;i<9;i++)
      m.a[i]=a[i]+p.a[i];
    return m;

  }
};

inline Matrix3 getIdentity()
{
  Matrix3 m;
  m.a[0]=1;
  m.a[1]=0;
  m.a[2]=0;
  m.a[3]=0;
  m.a[4]=1;
  m.a[5]=0;
  m.a[6]=0;
  m.a[7]=0;
  m.a[8]=1;
  return m;
}

inline Matrix3 getRotMatrix(Pos3D p)
{
  Matrix3 m;
  m.a[0]=0;
  m.a[1]=p.z;
  m.a[2]=-p.y;
  m.a[3]=-p.z;
  m.a[4]=0;
  m.a[5]=p.x;
  m.a[6]=p.y;
  m.a[7]=-p.x;
  m.a[8]=0;
  return m;
}

// p*pT
inline Matrix3 toMatrix(Pos3D p)
{
  Matrix3 m;

  m.a[0]=p.x*p.x;
  m.a[1]=p.x*p.y;
  m.a[2]=p.x*p.z;
  m.a[3]=p.y*p.x;
  m.a[4]=p.y*p.y;
  m.a[5]=p.y*p.z;
  m.a[6]=p.z*p.x;
  m.a[7]=p.z*p.y;
  m.a[8]=p.z*p.z;

  return m;
}

inline Pos3D rotVector(Pos3D p,Pos3D n,float a)
{
  Matrix3 hrot=getRotMatrix(n);

  Matrix3 rot=getIdentity()*cos(a)+toMatrix(n)*(1-cos(a))+getRotMatrix(n)*sin(a);

  return rot*p;
}

struct Pos2D
{
  float x,y;
  
  Pos2D(float px,float py);

  float norm2() const;
  float norm() const;
  
  Pos2D normal() const
  {
    return Pos2D(y,-x);
  }

  Pos2D operator+(const Pos2D &p) const;
  Pos2D operator-(const Pos2D &p) const;

  bool operator<(const Pos2D &p) const;
  
  Pos2D operator*(float f) const;
  Pos2D operator/(float f) const;

  Pos2D normalized() const
  {
    float n=1.0/sqrt(x*x+y*y);
    return Pos2D(x*n,y*n);
  }

};

inline std::ostream &operator<<(std::ostream &o,const Pos3D &p)
{
  o<<"("<<p.x<<","<<p.y<<","<<p.z<<")";
  return o;
}

inline std::ostream &operator<<(std::ostream &o,const Pos2D &p)
{
  o<<"("<<p.x<<","<<p.y<<")";
  return o;
}

inline Serial&operator<<(Serial &s,const Pos2D &p)
{
  s<<p.x<<p.y;
  return s;
}

inline Serial&operator>>(Serial &s,Pos2D &p)
{
  s>>p.x>>p.y;
  return s;
}

struct AntRect
{
  int x,y,w,h;

  AntRect(int X,int Y,int W,int H):x(X),y(Y),w(W),h(H)
  {
  }
};

#endif