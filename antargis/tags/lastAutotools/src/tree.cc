#include "tree.h"

#include <math.h>

#include "ag_texturecache.h"

GLTree::GLTree(AGVector4 p,float h)
{
  position=p;
  height=h;
  wu=0.01;
  wl=0.2;
  
  trunkTex=getTextureCache()->get("data/textures/models/fir_trunk.png");
  firTex=getTextureCache()->get("data/textures/models/fir1.png");
  init();
}
GLTree::~GLTree()
{
}

void GLTree::draw()
{
  mydraw(true);
}

void GLTree::drawDepth()
{
  mydraw(false);
}
void GLTree::drawShadow()
{
  mydraw(false);
}

void GLTree::mydraw(bool texture)
{
  glPushMatrix();
  
  glTranslatef(position[0],position[1],position[2]);
  
  if(texture)
    {
      glColor4f(.5f,0.4f,0.2f,1.0f);
      
      glBindTexture(GL_TEXTURE_2D,trunkTex.getTextureID());
      mTrunkVA.setColors(true);
      mFirVA.setColors(true);
    }
  else
    {
      mTrunkVA.setColors(false);
      mFirVA.setColors(false);
    }

  mTrunkVA.draw();
  
  if(texture)
    {
      glColor4f(0.2f,.5f,0.0f,1.0f);
      // needles and so
      glBindTexture(GL_TEXTURE_2D,firTex.getTextureID());
    }

  mFirVA.draw();
  
  glPopMatrix();
}

void GLTree::init()
{
  float a;
  float step=M_PI/2.0; // MUST BE A DEVIDER OF 2*pi

  AGVector4 white(1,1,1,1);
  size_t triIndex=0;
  for(a=0;a<2*M_PI;a+=step)
    {
      float x,y;
      float xu,yu,xl,yl;
      x=sin(a);
      y=cos(a);
      
      xu=x*wu;
      yu=y*wu;
      xl=x*wl;
      yl=y*wl;
      
      AGVector4 v1(xl,yl,0.0);
      AGVector4 v2(xu,yu,height);
      
      x=sin(a+step);
      y=cos(a+step);
      
      xu=x*wu;
      yu=y*wu;
      xl=x*wl;
      yl=y*wl;
      
      AGVector4 v3(xl,yl,0.0);
      AGVector4 v4(xu,yu,height);

      AGVector3 n1(v1[0],v1[1],v1[2]);
      AGVector3 n2(v2[0],v2[1],v2[2]-height);
      AGVector3 n3(v3[0],v3[1],v3[2]);
      AGVector3 n4(v4[0],v4[1],v4[2]-height);
      n1.normalize();
      n2.normalize();
      n3.normalize();
      n4.normalize();
      mTrunkVA.addVertex(v1,white,n1,AGVector2(0,0));
      mTrunkVA.addVertex(v2,white,n2,AGVector2(0,1));
      mTrunkVA.addVertex(v3,white,n3,AGVector2(1,0));
      mTrunkVA.addVertex(v4,white,n4,AGVector2(1,1));
      mTrunkVA.addTriangle(triIndex,triIndex+1,triIndex+2);
      mTrunkVA.addTriangle(triIndex+1,triIndex+2,triIndex+3);
      triIndex+=4;
      /*
	drawVertex(v1,v1,0,0);
      drawVertex(v2,v2-AGVector4(0.0f,0.0f,height),0,1);
      drawVertex(v3,v3,1,0);
      drawVertex(v2,v2-AGVector4(0.0f,0.0f,height),0,1);
      drawVertex(v3,v3,1,0);
      drawVertex(v4,v4-AGVector4(0.0f,0.0f,height),1,1);*/
    }

  triIndex=0;
  float y,ys=0.05;
  AGMatrix4 rot,current;
  rot=AGMatrix4(44.0f*M_PI/180.0,AGVector3(0,0,1));


  for(y=height*0.4;y<height;y+=ys)
    {
      // FIXME: add rotation
      current*=rot;
      
      // 2. must go down
      AGVector4 v1(0.0,2.4*(1.0-y/height),y-0.5); // outer
      AGVector4 v2(-0.9*(1.0-y/height),0.0,y);
      AGVector4 v3(0.9*(1.0-y/height),0.0,y);
      
      v1=current*v1;
      v2=current*v2;
      v3=current*v3;
          
      AGVector4 n1=v2-v1;
      AGVector4 n2=v3-v1;
      AGVector4 ntmp=n2%n1;
      AGVector3 n(ntmp[0],ntmp[1],ntmp[2]);
      n.normalize();
      n*=-1;

      mFirVA.addVertex(v1,white,n,AGVector2(0.5,1));
      mFirVA.addVertex(v2,white,n,AGVector2(0,1));
      mFirVA.addVertex(v3,white,n,AGVector2(1,0));
      mFirVA.addTriangle(triIndex,triIndex+1,triIndex+2);
      triIndex+=3;
      //      break;
      /*
      glRotatef(44.0f,0.0f,0.0f,1.0f);
      glBegin(GL_TRIANGLES);
      //	  glColor4f(0.1f,.3f,0.0f,1.0f);
      drawVertex(v1,n,0.5,1);
      //	  glColor4f(0.1f,.3f,0.0f,0.7f);
      drawVertex(v2,n,0,0);
      //	  glColor4f(0.1f,.3f,0.0f,0.7f);
      drawVertex(v3,n,1,0);
      glEnd();
      */
    }
  
  //  glPopMatrix();
  //  glPopMatrix();
}

void GLTree::drawVertex(const AGVector4 &v,AGVector4 n,float t1,float t2)
{
  n.normalize3();
  glNormal3fv(n);
  glTexCoord2f(t1,t2);
  glVertex3fv(v);
}

