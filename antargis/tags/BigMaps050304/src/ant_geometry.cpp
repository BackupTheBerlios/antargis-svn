#include "ant_geometry.h"
#include "store.h"

///////////////////////////////////////////////////////////////////////////////
// Pos3D
///////////////////////////////////////////////////////////////////////////////

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



///////////////////////////////////////////////////////////////////////////////
// Pos2D
///////////////////////////////////////////////////////////////////////////////
  
Pos2D::Pos2D(float px,float py):
  x(px),y(py)
{}

bool Pos2D::operator<(const Pos2D &p) const
{
  return x<p.x || (x==p.x && y<p.y);
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
