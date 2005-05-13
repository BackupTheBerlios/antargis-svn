/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * voxel.cpp
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

#include "voxel.h"
//#include "terrain.h"
#include "SDL.h"
#include <ag_fs.h>
#include <cstdlib>
#include "decast.h"
#include "ag_color.h"
#include "ag_fontengine.h"
#include "texture_cache.h"

#include <map>

#define interval(v,mi,ma) std::min(std::max(mi,v),ma)

//#define VOXELHEIGHT 128
int VOXELHEIGHT;

bool gDrawingTerrain=false;

#define BAD_ALPHA

///////////////////////////////////////////////////////////////////////////////
// Color
///////////////////////////////////////////////////////////////////////////////

Color::Color(float R,float G,float B,float A)
{
  r=R;
  g=G;
  b=B;
  a=A;
  if(r>1 || g>1 || b>1)
    {
      r/=0xFF;
      g/=0xFF;
      b/=0xFF;
      r*=0.8;
      g*=0.8;
      b*=0.8;
      //      cdebug(r<<","<<g<<","<<b<<","<<a);
    }
}
Color::Color()
{
  r=g=b=a=0.0f;
}

Uint32 Color::toColor(SDL_Surface *s) const
  {
    assert(a<=1.0 && a>=0.0);

    /*  return sge_MapAlpha(
          interval(int(r*0xFF),0,0xFF),
          interval(int(g*0xFF),0,0xFF),
          interval(int(b*0xFF),0,0xFF),
          interval(int(a*0xFF),0,0xFF));
    */
    return SDL_MapRGBA(s->format,
                       interval(int(r*0xFF),0,0xFF),
                       interval(int(g*0xFF),0,0xFF),
                       interval(int(b*0xFF),0,0xFF),
                       interval(int(a*0xFF),0,0xFF));
    //  return SDL_MapRGBA(s->format,r*0xFF,g*0xFF,b*0xFF,0xFF);//a*0xFF);
  }

Color Color::operator%=(const Color &c)
{
  r=r*(a)+(1-a)*c.r;
  g=g*(a)+(1-a)*c.g;
  b=b*(a)+(1-a)*c.b;

  a+=c.a;
  if(a>1.0)
    a=1.0;
  return *this;
}

Color Color::light(float d)
{
  r+=d;
  g+=d;
  b+=d;
  r=interval(r,0.0f,1.0f);
  g=interval(g,0.0f,1.0f);
  b=interval(b,0.0f,1.0f);
  return *this;
}

std::ostream&operator<<(std::ostream&o,const Color &c)
{
  o<<"("<<c.r<<","<<c.g<<","<<c.b<<","<<c.a<<")";
  return o;
}


Color Color::operator+=(const Color &c)
{
  r+=c.r;
  g+=c.g;
  b+=c.b;
  return *this;
}

Color Color::operator/=(float f)
{
  r/=f;
  g/=f;
  b/=f;
  return *this;
}

Color Color::operator*=(float f)
{
  r*=f;
  g*=f;
  b*=f;
  return *this;
}


///////////////////////////////////////////////////////////////////////////////
// VoxelSpace
///////////////////////////////////////////////////////////////////////////////

template<class T>
VoxelSpace<T>::VoxelSpace()
{}

template<class T>
inline void VoxelSpace<T>::set
  (Pos3D p,T v)
  {
    mMap[p]=v;
  }

template<class T>
inline T VoxelSpace<T>::get
  (Pos3D p)
  {
    if(mMap.find(p)==mMap.end())
      return Color();
    return mMap[p];
  }




///////////////////////////////////////////////////////////////////////////////
// VoxelView
///////////////////////////////////////////////////////////////////////////////

SDL_Surface *Create32BitSurface(size_t width,size_t height)
{
  /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
     as expected by OpenGL for textures */
  SDL_Surface *surface;
  Uint32 rmask, gmask, bmask, amask;

  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN

  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else

  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
                                 rmask, gmask, bmask, amask);
  if(!surface)
    {
      std::cerr<<"CreateRGBSurface failed: "<<SDL_GetError()<<std::endl;
      //        throw UI::UIException();
    }
  return surface;
}


VoxelView::VoxelView(int pw,int ph,Pos3D pPos,bool pIso): // simple screen coordinates
    AVItem(pPos),iso(pIso),
    w(pw),h(ph)
{
  s=Create32BitSurface(w,h);//sge_CreateAlphaSurface(0,w,h);
  VOXELHEIGHT=w;
}
VoxelView::~VoxelView()
{}

void VoxelView::set
  (Pos3D p,Color v)
  {
    mSpace.set(p,v);
  }

//#define OLD_SHADOW
#ifdef OLD_SHADOW
void VoxelView::calcShadow()
{
  //return;
  int x,y;
  int m;
  Pos3D p(0,0,0);


  float shadow;

  for(x=-h;x<w;x++)
    for(y=0;y<w;y++)
      {
        m=0;
        p=Pos3D(x,h-1,y);
        shadow=0.0f;
        for(;m<h+10;m++)
          {
            Color c=mSpace.get(p);
            if(c.a>0)
              {

                if(shadow>0.01f)
                  {
                    //    cdebug(p.x<<","<<p.y<<","<<p.z<<":"<<int(shadow));
                    mSpace.set(p,c.light(-80.0f/0xFF*shadow));
                  }
                //    cdebug(shadow);
                shadow+=c.a;
                if(shadow>1.0)
                  shadow=1.0;
              }

            p=p+Pos3D(1,-1,0);
            //      mSpace.set(p,Color(1,0,0));
          }

      }
  return;
  // top light

  for(x=0;x<w;x++)
    for(y=-w;y<w;y++)
      {
        m=0;
        p=Pos3D(x,h-1,y);
        shadow=0.0f;
        for(;m<h+10;m++)
          {
            Color c=mSpace.get(p);
            if(c.a>0)
              {

                if(shadow>0.01f)
                  {
                    //    cdebug(p.x<<","<<p.y<<","<<p.z<<":"<<int(shadow));
                    mSpace.set(p,c.light(-80.0f/0xFF*shadow));
                  }
                //    cdebug(shadow);
                shadow+=c.a;
                if(shadow>1.0)
                  shadow=1.0;
              }

            p=p+Pos3D(0,-1,0);
            //      mSpace.set(p,Color(1,0,0));
          }

      }


}
#else
void VoxelView::calcShadow()
{
  return;
  int x,y;
  int m;
  Pos3D p(0,0,0);


  float light;

  for(x=-h;x<w;x++)
    for(y=0;y<w;y++)
      {
        m=0;
        p=Pos3D(x,h-1,y);
        light=1.0f;

        for(;m<h+10;m++)
          {
            Color c=mSpace.get(p);
            if(c.a>0)
              {

                //    if(shadow>0.01f)
                {
                  //    cdebug(p.x<<","<<p.y<<","<<p.z<<":"<<int(shadow));
                  mSpace.set(p,c.light((-80.0f+80.0f*light)/0xFF));
                }
                //    cdebug(shadow);
                light*=(1.0f-c.a);
              }

            p=p+Pos3D(1,-1,0);
            //      mSpace.set(p,Color(1,0,0));
          }

      }

}
#endif

void VoxelView::init()
{
  VOXELHEIGHT=h;
  int x,y;
  Color c;

  calcShadow();

  for(x=0;x<w;x++)
    for(y=0;y<h;y++)
      {
        Pos2D p2(x,y);
        c=trace(p2);
        if(c.a>=0)
          {
            sge_PutPixel(s,x,y,c.toColor(s));

          }
      }
  mSurface=getSurfaceManager()->fromSDL(s);
  mTexture=new AGTexture(getTextureManager()->makeTexture(mSurface));
  return;
}

bool VoxelView::inIso(Pos3D p)
{
  if(!iso)
    return true;

  int x=(int)(p.x-p.z+w/2);
  int y=(int)(p.x+p.z-w/2);
  return x>=0 && y>=0 && x<w && y<w; // for now
}

Pos3D VoxelView::unwind(Pos3D p)
{
  return Pos3D(p.x,int((h-1)-p.y-p.z/2.0),p.z);
}

float VoxelView::blurDiffuseLight(Pos3D p)
{
#ifdef NO_DIFFUSE
  return 0.0;
#endif

  Pos3D x(1,0,0),y(0,1,0),z(0,0,1);
#ifndef BLUR_DIFFUSE

  return diffuseLight(p);
#endif
  //  float v=(diffuseLight(p)+diffuseLight(p+x)+diffuseLight(p+y)+diffuseLight(p+z))*0.25;

  float v=(diffuseLight(p)+
           diffuseLight(p+x)+diffuseLight(p+y)+diffuseLight(p+z)+
           diffuseLight(p-x)+diffuseLight(p-y)+diffuseLight(p-z)
          )/7.0;

  return v;

}

float VoxelView::diffuseLight(Pos3D p)
{
  Pos3D x(1,0,0),y(0,1,0),z(0,0,1);

  float nx=has1(p+x)-has1(p-x);
  float ny=has1(p+y)-has1(p-y);
  //    float nz=has1(p+z)-has1(p-z);

  //    float n=sqrt(nx*nx+ny*ny+nz*nz);
  /*
  nx/=n;
  ny/=n;
  nz/=n;
  */
  float nf=nx-ny;
  /*
  if(n<0.7)
    nf=0.0;

  if(n>2.0)
    nf=0.0;
  */
  /*
  nx+=has1(p+x+x)-has1(p-x-x);
  ny+=has1(p+y+y)-has1(p-y-y);
  nz+=has1(p+z+z)-has1(p-z-z);
  */
  
  
  // f=32, a=32 for trees
  // f=64, a=16 for terrain
  
  float a,f;
  
  if(gDrawingTerrain)
  {
     f=(64/256.0);//32
    a=(16/256.0);//32;
  }
  else
  {
     f=(32/256.0);//32
    a=(32/256.0);//32;
  }

  //    nz=0;

  //    cdebug(nx<<";"<<ny<<";"<<nz);

  //    float v=(0.5*nx-ny)*0.125+0.2;

  float v=nf*f+a;


  /*
  if(hasShadow(p))
    // v-=0.3;
    v-=20;
  */
  //    v/=0xFF;

  //    cdebug(v);

  return v;
}

float VoxelView::has1(Pos3D p)
{
  return mSpace.get(p).a; // return alpha
}






// ALLES QUATSCH - EIGENE FORMELN AUSDENKEN!!!!!!!!!!

#define NEW_TRACE
#ifdef NEW_TRACE
Color VoxelView::trace(Pos2D p)
{
  int z;
  int d=w;
  Color cn;

  float alpha=0;


  //    float amount=0;

  for(z=0;z<d;z++)
    {
      Pos3D op3(p.x,p.y,z);

      if(isIn(op3))
        if(inIso(op3))
          {
            Pos3D p3=unwind(op3);
            Color ac=getColor(p3);
            if(ac.a>0.0001f)
              {
                //      cdebug(cn<<"//"<<alpha);
                //      cdebug("ac:"<<ac);
                Color ac2=ac;
                ac2*=(1-alpha);
                ac2*=ac.a;
                cn+=ac2;
                alpha+=(1-alpha)*ac.a;
                cn.a=std::min(alpha,1.0f);

                //    cdebug("2:"<<cn<<"//"<<alpha);
#ifdef BAD_ALPHA

                if(alpha>0.0f)
                  {
                    cn/=cn.a;
                    cn.a=1.0f;
                    return cn;
                  }
#else
                if(alpha>0.9f)
                  {

                    //          cn/=cn.a;
                    //        cdebug("früh:"<<cn);
                    return cn;
                  }
#endif

              }
          }
    }
  //    if(cn.a<0.9f)
  //  return Color(0,0,0,0);
  //    cdebug(cn);

  //    cn/=cn.a;
  //    cdebug("all:"<<cn);

  return cn;
}
#else

Color VoxelView::trace(Pos2D p)
{
  int z;
  int d=w;
  Color cn;

  //    float alpha=0;

  for(z=0;z<d;z++)
    {
      Pos3D op3(p.x,p.y,z);

      if(isIn(op3))
        if(inIso(op3))
          {
            Pos3D p3=unwind(op3);
            Color ac=getColor(p3);
            if(ac.a>0.0001f)
              {
                cn%=ac;
                //      cdebug("trace-cn:"<<cn);
                if(cn.a>0.9f)
                  {
                    //          cdebug("cn:"<<cn);
                    return cn;
                  }
              }
          }
    }
  //    if(cn.a<0.9f)
  //  return Color(0,0,0,0);
  //    cdebug(cn);
  return cn;
}
#endif
bool VoxelView::isIn(const Pos3D &p3) const
  {
    return p3.x>=0 && p3.y>=0 && p3.z>=0 && p3.x<=w && p3.y<=h && p3.z<=w;
  }


Color VoxelView::getColor(Pos3D p)
{
  Color c=mSpace.get(p);
  //  c.light((rand()%60)/float(0xFF));
  c.light(blurDiffuseLight(p));
  //c.light(blurDiffuseLight(p)+(rand()%20)/float(0xFF));
  return c;
}

bool VoxelView::hasShadow(Pos3D p)
{
  while(isIn(p))
    {
      p=p+Pos3D(-1,1,0);
      if(mSpace.get(p).a>0)
        return true;
    }
  return false;
}



Pos2D projectView(const Pos3D &p)
{
  //  cdebug(VOXELHEIGHT);
  return Pos2D((int)(p.x),(int)(VOXELHEIGHT-p.y-p.z/2-1));
}
Pos2D projectShadow(const Pos3D &p)
{
#ifdef SHADOW_DOWN
  return Pos2D(p.x,p.z);
#else
  //return Pos2D((int)(p*Pos3D(0,0,1)),(int)(p*Pos3D(1,1,0)));
  //  return Pos2D((int)(p.z),(int)(VOXELHEIGHT-p.y-p.x-1));
  return Pos2D((int)(p*Pos3D(1,0,-1)),(int)(p*Pos3D(1,1,1)));
  //  return Pos2D((int)(p.z-p.x),(int)(VOXELHEIGHT-p.y-p.x-1));
#endif
}

int depthView(const Pos3D &p)
{
  return (int)(p.z);
}
int depthShadow(const Pos3D &p)
{
#ifdef SHADOW_DOWN
  return (int)(VOXELHEIGHT-p.y);
#else
  // SHADOW to back right
  return (int)(VOXELHEIGHT+p*Pos3D(1,-1,1));
#endif

  return (int)(p*Pos3D(1,-1,1));
}


////////////////////////////////////////////////////////////////////
// FastVoxelView
////////////////////////////////////////////////////////////////////

FastVoxelView::FastVoxelView(int pw,int ph,Pos3D pPos,bool pIso,int zoom): // simple screen coordinates
    VoxelView(pw,ph,pPos,pIso),
    viewSpace(&projectView,this),
    shadowSpace(&projectShadow,this),
    mZoom(zoom)
{
  SDL_FreeSurface(s);
  s=Create32BitSurface(w*zoom,h*zoom);//sge_CreateAlphaSurface(0,w,h);
  VOXELHEIGHT=ph;
}

void FastVoxelView::calcShadow()
{}


void FastVoxelView::init()
{
  //  VOXELHEIGHT=w;

  std::map<Pos2D,std::pair<int,Voxel*> >::iterator i=viewSpace.mViewSpace.begin();

  Color c;
  Pos2D p(0,0);
  for(;i!=viewSpace.mViewSpace.end();i++)
    {
      p=i->first;
      c=viewSpace.get(p);
      p.x*=mZoom;
      p.y*=mZoom;
      if(mZoom==1)
        sge_PutPixel(s,(int)p.x,(int)p.y,c.toColor(s));
      else
        sge_FilledRect(s,(int)p.x,(int)p.y,(int)(p.x+mZoom),(int)(p.y+mZoom),c.toColor(s));
    }
  //  SDL_SaveBMP(s,"myfile.bmp");

  mSurface=getSurfaceManager()->fromSDL(s);
  return;
}



CVBitmap<ShadowInfo> FastVoxelView::getShadowMap()
{
  CVBitmap<ShadowInfo> smap(w,2*w,w*w);

#ifdef CVOXEL

  std::map<Pos2D,std::pair<int,Voxel*> >::iterator i=shadowSpace.mViewSpace.begin();

  for(;i!=shadowSpace.mViewSpace.end();i++)
    {
      Pos3D p=i->second.second->pos;
      Pos2D viewPos=projectView(p);

      bool visible=false;

      if(viewSpace.mViewSpace[viewPos].second)
        if(viewSpace.mViewSpace[viewPos].second->pos==p)
          visible=true;

      //      if(i->first.y<0)
      //  cdebug(p);
      smap.set(i->first,ShadowInfo(i->second.first,visible,viewPos));

      //      sge_PutPixel(getScreen().surface(),i->first.x+400,i->first.y,color);

    }
#endif
  //    SDL_Flip(getScreen().surface());
  //  SDL_Delay(20000);


  return smap;
}
CVBitmap<DepthInfo> FastVoxelView::getDepthMap()
{
  CVBitmap<DepthInfo> dmap(w,h,0);

  std::map<Pos2D,std::pair<int,Voxel*> >::iterator i=viewSpace.mViewSpace.begin();

  for(;i!=viewSpace.mViewSpace.end();i++)
    {
      if(i->second.second)
        {
          Pos3D p=i->second.second->pos;

          dmap.set(i->first,DepthInfo(i->second.first,projectShadow(p)));
        }
    }



  return dmap;
}





////////////////////////////////////////////////////////////////////
// FastView
////////////////////////////////////////////////////////////////////

long fvmax=0;
long fvruns=0;


void FastView::set
  (Voxel *v,int d)
  {
    mViewSpace[projection(v->pos)][d]=v;
  }

Color FastView::get
  (Pos2D p)
  {
    std::map<int,Voxel*> &l=mViewSpace[p];

    Color cn;
    float alpha=0;

    if(l.size())
      {
        std::map<int,Voxel*>::iterator i=l.begin();
        for(;i!=l.end();i++)
          {
            if(parent->inIso(i->second->pos))
              {
                Color ac=parent->getColor(i->second->pos);
                if(ac.a>0.0001f)
                  {
                    Color ac2=ac;
                    ac2*=(1-alpha);
                    ac2*=ac.a;
                    cn+=ac2;
                    alpha+=(1-alpha)*ac.a;
                    cn.a=std::min(alpha,1.0f);


                    if(alpha>0.0f)
                      {
                        cn/=cn.a;
                        cn.a=1.0f;
                        return cn;
                      }
                  }
              }
          }
      }

    return cn;
  }

float FastView::getShadow(Pos3D p)
{
  Pos2D p2=projection(p);
  std::map<int,Voxel*> &l=mViewSpace[p2];

  std::map<int,Voxel*>::iterator i=l.begin();
  float a=0.0f;
  for(;i!=l.end();i++)
    {
      Pos3D p3=i->second->pos;
      if(p==p3)
        return a;
      a+=i->second->color.a;
      if(a>0.9)
        return 1.0;
    }
  return a;
}




////////////////////////////////////////////////////////////////////
// FastView
////////////////////////////////////////////////////////////////////


void VeryFastView::set
  (Voxel *v,int d)
  {
    //  if(v->color.a<0.7)
    //    return;
    if(!parent->inIso(v->pos))
      return;



    Pos2D p2=projection(v->pos);
    //  cdebug("size:"<<mViewSpace.size());
    std::map<Pos2D,std::pair<int,Voxel*> >::iterator i=mViewSpace.find(p2);
    //  cdebug("size:"<<mViewSpace.size());

    //  CTRACE;
    //  cdebug(p2<<"//"<<v->pos);

    if(i!=mViewSpace.end())
      {
        if(i->second.first>d)
          {
            i->second=std::make_pair(d,v);
            //         cdebug("set:"<<i->second.first<<":"<<d);
            return;
          }
        //      cdebug(i->second.first<<":"<<d);
        return;
      }
    //  cdebug("set2:"<<d);
    mViewSpace[p2]=std::make_pair(d,v);
  }

Color VeryFastView::get
  (Pos2D p) const
    {
      std::map<Pos2D,std::pair<int,Voxel*> >::const_iterator i=mViewSpace.find(p);

      if(i==mViewSpace.end())
        return Color(0,0,0,0);

      Pos3D pos=i->second.second->pos;

      Color cn=parent->getColor(pos);

      //  cdebug(cn);
      //    cn/=cn.a;
      cn.a=1.0f;

      return cn;
    }














////////////////////////////////////////////////////////////////////
// MyApp
////////////////////////////////////////////////////////////////////



class MyApp:public AGApplication
  {
  public:
    bool eventIdle()
    {
      SDL_Delay(100);
      //    tryQuit();
      return true;
    }
  };

AGSurface test1()
{
  int w=128;
  VoxelView v(w,w,Pos3D(0,0,0),false);

  int x,y,z;

  int s=0;

  for(x=s;x<w;x++)
    for(y=0;y<1;y++)
      for(z=s;z<w;z++)
        v.set(Pos3D(x,y,z),Color(0,0,1.0));

  s=10;

  for(x=s;x<w-s;x++)
    for(y=0;y<1;y++)
      for(z=s;z<w-s;z++)
        v.set(Pos3D(x,y+10,z),Color(0,0,1.0));

  return v.getSurface();
}

AGSurface ball()
{
  int w=64;
  VoxelView v(w,w*2,Pos3D(0,0,0),false);

  int x,y,z;

  float r=w/2;

  for(x=0;x<w;x++)
    for(y=0;y<w;y++)
      for(z=0;z<w;z++)
        {
          int mx=x-w/2;
          int my=y-w/2;
          int mz=z-w/2;
          float d=sqrt((float)(mx*mx+my*my+mz*mz));
          if(d<r+3)
            {
              float a=r-d;
              if(a<0)
                a=0;

              if(a>0)
                v.set(Pos3D(x,y,z),Color(0,0,1,std::max(1.0f,a)));
            }
        }

  return v.getSurface();
}



#ifdef MYTEST

int main(int argc,char *argv[])
{
  AGMain main;
  MyApp app;

  AntargisView *av=new AntargisView(0,AGRect(0,0,100,100),Pos3D(0,0,0));

  int x,y,z;

  int w=64;
  int s=0;

  Terrain t(16,16);

  AGSurface ms;

  int test=4;

  if(argc>1)
    test=atoi(argv[1]);

  if(test==1)
    {
      ms=test1();
      av->insert(new VoxelImage(ms,Pos3D(0,0,0)));
    }
  if(test==2)
    {
      ms=ball();
      av->insert(new VoxelImage(ms,Pos3D(0,0,0)));
    }
  else if(test==3)
    {
      ms=getTerrain(t.getInfo(2,0));
      av->insert(new VoxelImage(ms,Pos3D(0,0,0)));
    }
  else if(test==4)
    {
      ms=getTerrain(t.getInfo(1,0));
      av->insert(new VoxelImage(ms,Pos3D(0,0,0)));


      ms=getTerrain(t.getInfo(2,0));
      av->insert(new VoxelImage(ms,Pos3D(64,0,0)));


      ms=getTerrain(t.getInfo(1,1));
      av->insert(new VoxelImage(ms,Pos3D(32,0,-32)));

      ms=getTerrain(t.getInfo(2,1));
      av->insert(new VoxelImage(ms,Pos3D(64+32,0,-32)));
    }
  else if(test==5)
    {
      int x,y;
      for(int x=0;x<4;x++)
        for(int y=0;y<4;y++)
          {
            ms=getTerrain(t.getInfo(x,y));
            av->insert(new VoxelImage(ms,Pos3D(x*64+(y%2)*32,0,-y*32)));
          }
    }
  else if(test==6)
    {
      SplineMap<float> m(16,16,50);
      SplineMap<float> gm(16,16,20);
      int w=64;
      VoxelView v(w,w*2,Pos3D(0,0,0),true);

      for(int x=0;x<w;x++)
        for(int z=0;z<w;z++)
          {
            float mx=x/16.0;
            float mz=z/16.0;

            float h=m.get(mx,mz);
            for(int y=0;y<h;y++)
              {
                float a=std::min(1.0f,h-y);
                //    cdebug(a);
                v.set(Pos3D(x,y,z),Color(0xAA,0xAA,0,a));
              }

            // grass above

            float gh=rand()%int(gm.get(mx,mz));
            if(gh>0)
              for(int y=0;y<gh;y++)
                {
                  float a=1.0f-(y/gh);
                  //    cdebug(a);
                  v.set(Pos3D(x,y+h,z),Color(0,0xAA,0,a));
                }

          }

      /*



      // small ball above

      for(int x=0;x<8;x++)
      for(int y=0;y<8;y++)
      for(int z=0;z<8;z++)
      {
       int mx=x-4;
       int my=y-4;
       int mz=z-4;
       if(sqrt(mx*mx+my*my+mz*mz)<4)
      v.set(Pos3D(x+10,y+40,z+20),Color(0xAA,0,0));
      }

      */
      av->insert(new VoxelImage(v.getSurface(),Pos3D(0,0,0)));

    }

  else if(test==7)
    {
      float v0=-20;
      float v1=60;
      float v2=60;
      float v3=-20;
      float v4=0;
      float v5=0;
      float v6=0;
      int x;

      SDL_Surface *s=getScreen().surface();
      Color c(0,0,1);
      for(x=0;x<32;x++)
        {
          int y=spline2(x/32.0,v0,v1,v2);//,v3);
          sge_PutPixel(s,x,y+50,c.toColor(s));
        }
      for(x=0;x<32;x++)
        {
          int y=spline2(x/32.0,v1,v2,v3);//,v4);
          sge_PutPixel(s,x+32,y+50,c.toColor(s));
        }
      for(x=0;x<32;x++)
        {
          int y=spline2(x/32.0,v2,v3,v4);//,v5);
          sge_PutPixel(s,x+64,y+50,c.toColor(s));
        }
      for(x=0;x<32;x++)
        {
          int y=spline2(x/32.0,v3,v4,v5);//,v6);
          sge_PutPixel(s,x+96,y+50,c.toColor(s));
        }
      SDL_Flip(s);
      //      SDL_Delay(1000);
    }
  else if(test==8)
    {
      float v0=-20;
      float v1=60;
      float v2=60;
      float v3=-20;
      float v4=0;
      float v5=0;
      float v6=0;
      int x;

      SDL_Surface *s=getScreen().surface();
      Color c(1,0,1);
      for(x=0;x<32;x++)
        {
          int y=spline3(x/32.0,v0,v1,v2,v3);
          sge_PutPixel(s,x,y+50,c.toColor(s));
        }
      for(x=0;x<32;x++)
        {
          int y=spline3(x/32.0,v1,v2,v3,v4);
          sge_PutPixel(s,x+32,y+50,c.toColor(s));
        }
      for(x=0;x<32;x++)
        {
          int y=spline3(x/32.0,v2,v3,v4,v5);
          sge_PutPixel(s,x+64,y+50,c.toColor(s));
        }
      for(x=0;x<32;x++)
        {
          int y=spline3(x/32.0,v3,v4,v5,v6);
          sge_PutPixel(s,x+96,y+50,c.toColor(s));
        }
      SDL_Flip(s);
      //      SDL_Delay(1000);
    }
  else if(test==9)
    {
      SplineMap<float> m(64,64,50);
      int w=64;
      VoxelView v(w,w*2,Pos3D(0,0,0),false);

      int z=0;
      for(int x=0;x<w;x++)
        {
          float h=20+x/16.0;
          for(int y=0;y<h;y++)
            {
              float a=std::min(1.0f,h-y);
              //      if(a<1)
              if(a>0)
                {
                  cdebug(a);
                  v.set(Pos3D(x,y,z),Color(0xFF,0xFF,0xFF,a));//Color(0xAA*a,0xAA*a,0));//,a));
                }
            }
        }

      av->insert(new VoxelImage(v.getSurface(),Pos3D(0,0,0)));

    }
  else if(test==10)
    {
      SDL_Surface *s=getScreen().surface();
      for(int x=0;x<100;x++)
        {
          float h=20+x/16.0;
          for(int y=0;y<h;y++)
            {
              float a=std::min(1.0f,h-y);
              if(a>0)
                {
                  sge_PutPixelAlpha(s,x,100-y,SDL_MapRGBA(s->format,0xFF,0xFF,0xFF,a*0xFF),a*0xFF);
                }
            }
        }
      SDL_Flip(s);
      SDL_Delay(1000);
    }
  else if(test==11 || test==12)
    {
      int w=8;
      SplineMap<float> m(POINTS_PER_TILE*(w+2),POINTS_PER_TILE*(w+2),40);
      SplineMap<float> gm(POINTS_PER_TILE*(w+2),POINTS_PER_TILE*(w+2),10);

      if(test==12)
        w=2;

      for(int y=0;y<w;y++)
        for(int x=0;x<w;x++)
          {
            int mx=x*(POINTS_PER_TILE);
            int my=y*(POINTS_PER_TILE/2);
            if(y&1)
              mx+=(POINTS_PER_TILE/2);

            av->insert(new VoxelImage(makeTerrainTile(m,gm,mx,my),Pos3D(mx*TILE_WIDTH/POINTS_PER_TILE,0,my*TILE_WIDTH/POINTS_PER_TILE)));
          }
    }
  else if(test==13)
    {
      AGSurface s=makeWaterTile();
      av->insert(new VoxelImage(s,Pos3D(64,0,0)));
      av->insert(new VoxelImage(s,Pos3D(0,0,0)));
      av->insert(new VoxelImage(s,Pos3D(64+32,0,32)));
      av->insert(new VoxelImage(s,Pos3D(32,0,32)));
      av->insert(new VoxelImage(s,Pos3D(64+32,0,-32)));
      av->insert(new VoxelImage(s,Pos3D(32,0,-32)));
    }
  else if(test==14)
    {
      AGSurface s=makeWaterTile();
      std::string ms=toPNG(s.surface());
      s=fromPNG(ms);
      av->insert(new VoxelImage(s,Pos3D(64,0,0)));
    }

  app.setMainWidget(av);

  app.run();
}



#endif
