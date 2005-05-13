/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * voxel.h
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

#ifndef VOXEL_H
#define VOXEL_H

#include <deque>
#include <map>
#include "SDL.h"
#include "sge.h"
#include "ag_application.h"
#include "ag_main.h"
#include "ag_surface.h"
#include "ag_debug.h"
#include <assert.h>

#include "ant_geometry.h"
#include "decast.h"

#include "cvinfo.h"
#include "voxel_gen.h"


extern bool gDrawingTerrain; // changes lighting to optimize for terrain, if set

struct Color
{
  float r,g,b,a; // values between 0 and 1

  Color(float R,float G,float B,float A=1.0f);
  Color();

  Color operator=(const Color &c)
  {
    r=c.r;
    g=c.g;
    b=c.b;
    a=c.a;
    return *this;
  }

  Uint32 toColor(SDL_Surface *s) const;

  Color operator%=(const Color &c);
  
  // operate only on color - alpha is discarded !!!
  Color operator+=(const Color &c);
  
  Color operator/=(float f);
  Color operator*=(float f);
  

  Color light(float d);
};

std::ostream&operator<<(std::ostream&o,const Color &c);

// slow space
template<class T>
class VoxelSpace
{
  std::map<Pos3D,T> mMap;
 public:

  VoxelSpace();
  void set(Pos3D p,T v);
  T get(Pos3D p);
};


class VoxelView:public AVItem
{
  bool iso;
  VoxelSpace<Color> mSpace;
 protected:
  int w,h;
  SDL_Surface *s;
 public:
  VoxelView(int pw,int ph,Pos3D pPos,bool pIso=true); // simple screen coordinates
  virtual ~VoxelView();

  void set(Pos3D p,Color v);
  float diffuseLight(Pos3D p);
  virtual Color getColor(Pos3D p);

  bool inIso(Pos3D p);


 protected:

  virtual void init();

  bool isIn(const Pos3D &p3) const;
  Pos3D unwind(Pos3D p);

  float blurDiffuseLight(Pos3D p);

  float has1(Pos3D p);
  bool hasShadow(Pos3D p);
  virtual void calcShadow();


  virtual Color trace(Pos2D p);
  //  Color getColor(Pos3D p);
};

struct Voxel
{
  Pos3D pos;
  Color color;
  //  Pos2D view,shadow;

  Voxel(Pos3D p,Color c):pos(p),color(c)
  {
    //    diffuse=light=-1.0;
  }
};

typedef Pos2D (*Project2D)(const Pos3D &p);
typedef int (*Project1D)(const Pos3D &p);

Pos2D projectView(const Pos3D &p);
Pos2D projectShadow(const Pos3D &p);

int depthView(const Pos3D &p);
int depthShadow(const Pos3D &p);


class FastView
{
 public:
  std::map<Pos2D,std::map<int,Voxel*> > mViewSpace;
  Project2D projection;
  VoxelView *parent;

 public:
  FastView(Project2D p,VoxelView *pa):projection(p),parent(pa)
    {
    }
    void set(Voxel *v,int d);
    
    Color get(Pos2D p);
    float getShadow(Pos3D p);
};

class VeryFastView
{
 public:
  std::map<Pos2D,std::pair<int,Voxel*> > mViewSpace;
  Project2D projection;
  VoxelView *parent;

 public:
  VeryFastView(Project2D p,VoxelView *pa):projection(p),parent(pa)
    {
    }
    void set(Voxel *v,int d);
    
    Color get(Pos2D p) const;

    float getShadow(Pos3D p) const
    {
      Pos2D p2=projection(p);
      std::map<Pos2D,std::pair<int,Voxel*> >::const_iterator i=mViewSpace.find(p2);

      if(i==mViewSpace.end())
	return 1;
      
      if(i->second.first==depthShadow(p))
	return 0;
      return 1;
    }
};


class FastVoxelView:public VoxelView
{
  std::list<Voxel*> mVoxels;
  VeryFastView viewSpace;
  VeryFastView shadowSpace; // should be FastView only, because shadow is better this way
  int mZoom;

 public:
  FastVoxelView(int pw,int ph,Pos3D pPos,bool pIso=true,int zoom=1); // simple screen coordinates
  ~FastVoxelView()
    {
      std::list<Voxel*>::iterator i=mVoxels.begin();
      for(;i!=mVoxels.end();i++)
	delete *i;

    }

  void set(Pos3D p,Color c)
  {
    Voxel *v=new Voxel(p,c);
    mVoxels.push_back(v);
    viewSpace.set(v,depthView(p));
    shadowSpace.set(v,depthShadow(p));
    VoxelView::set(p,c);
  }

  virtual Color getColor(Pos3D p)
  {
    float s=shadowSpace.getShadow(p);
    Color c=VoxelView::getColor(p);
    s/=4;
    c.light(-s);

    return c;
  }
  
  Color trace(Pos2D p)
  {
    return viewSpace.get(p);
  }

  void init();

  CVBitmap<ShadowInfo> getShadowMap();
  CVBitmap<DepthInfo> getDepthMap();


 protected:
  void calcShadow();
};


class ComplexVoxelImage;
//AGSurface makeTerrainTile(const SplineMap<float> &m,const SplineMap<float> &gm,int px,int py);


ComplexVoxelImage *makeTerrainTileComplex(const SplineMapD &m,const SplineMapD &gm,int px,int py);
VoxelImage *makeTerrainTile(const SplineMapD &m,const SplineMapD &gm,int px,int py);
VoxelImage *makeWaterTile();

//#include "cvoxel.h"

#endif
