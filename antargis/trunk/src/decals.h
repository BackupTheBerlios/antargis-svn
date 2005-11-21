#ifndef __DECALS_H__
#define __DECALS_H__

#include "map.h"

struct Color4f
{
  float c[4];

  Color4f(float r,float g,float b,float a=1.0f)
  {
    c[0]=r;
    c[1]=g;
    c[2]=b;
    c[3]=a;
  }
  Color4f()
  {
    c[0]=c[1]=c[2]=c[3]=1;
  }
#ifndef SWIG
  (operator float*)()
  {
    return c;
  }
#endif
};
struct Decal
{
  AGVector4 vs[4];
  Color4f c;
  AGVector4 n;
  Decal(AGVector4 v,AGVector4 d,Color4f pc):
    c(pc)
  {
    float m=0.05;
    float h=0.08;
    vs[0]=v+d*m+AGVector4(0,0,0);
    vs[1]=v+d*m+AGVector4(0,0,h);
    vs[2]=v-d*m+AGVector4(0,0,h);
    vs[3]=v-d*m+AGVector4(0,0,0);


    n=((vs[1]-vs[0])%(vs[2]-vs[0]));
    n.normalize3();
    n=AGVector4(0,0,1)*0.25+n*0.75;
  }
  void draw()
  {
    glColor4fv(c);
    glNormal3fv(n);
    glTexCoord2f(0,0);
    glVertex3fv(vs[0]);

    glColor4fv(c);
    glNormal3fv(n);
    glTexCoord2f(0,1);
    glVertex3fv(vs[1]);

    glColor4fv(c);
    glNormal3fv(n);
    glTexCoord2f(1,1);
    glVertex3fv(vs[2]);

    glColor4fv(c);
    glNormal3fv(n);
    glTexCoord2f(1,0);
    glVertex3fv(vs[3]);
    
  }
};

class Decals:public SceneNode
{
 public:
  Decals(HeightMap *map);

 private:
  HeightMap *map;
  std::vector<Decal> mDecals;
};



#endif
