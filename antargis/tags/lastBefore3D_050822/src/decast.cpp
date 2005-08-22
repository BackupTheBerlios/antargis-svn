/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * decast.cpp
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

#include "decast.h"

////////////////////////////////////////////////////////////////
// Bitmap
////////////////////////////////////////////////////////////////

#define myspline(t,a0,a1,a2,a3) catmull_rom(t,a0,a1,a2,a3)

// FIXME:
#define myspline_derive(t,a0,a1,a2,a3) spline_derive(t,a0,a1,a2,a3)


Bitmap::Bitmap()
{
  w=h=0;
}
Bitmap::Bitmap(int W,int H):std::vector<float>(W*H),
			    w(W),h(H)
{}
Bitmap::Bitmap(const Bitmap &b):
  std::vector<float>(b),
  w(b.w),h(b.h)
{}
float &Bitmap::operator[](std::pair<int,int> p)
{
  while(p.first>=w)
    p.first-=w;
  while(p.second>=w)
    p.second-=w;
  assert(p.first>=0 && p.second>=0 && p.first<w && p.second<h);
  return std::vector<float>::operator[](p.first+p.second*w);
}

std::string Bitmap::toString() const
{
  const float *p=&(front());
  
  return std::string((char*)const_cast<float*>(p),w*h*sizeof(float));
}

const float &Bitmap::operator[](std::pair<int,int> p) const
{
  while(p.first>=w)
    p.first-=w;
  while(p.second>=w)
    p.second-=w;
  while(p.first<0)
    p.first+=w;
  while(p.second<0)
    p.second+=w;
  
  /*
  if(!(p.first>=0 && p.second>=0 && p.first<w && p.second<h))
    {
      cdebug(w<<":"<<h<<" p:"<<p.first<<";"<<p.second);
    }
  */
  //  assert(p.first>=0 && p.second>=0 && p.first<w && p.second<h);
  
  return std::vector<float>::operator[](p.first+p.second*w);
}


////////////////////////////////////////////////////////////////
// Bitmap
////////////////////////////////////////////////////////////////

SplineMap::SplineMap()
      {
	w=h=0;
      }
SplineMap::SplineMap(int W,int H,int tile,int pmin,int pmax,bool rrand):
  values(W,H),w(W),h(H),
      rMap(tile,4) // was tile,1 - was 4=good, 

    {
      //      rrand=true;
      realRandom=rrand;
      // init
      int x,y;
      for(x=0;x<w;x++)
        for(y=0;y<h;y++)
          {
            if(rrand)
              {
                values[P(x,y)]=(rand()%tile)/float(tile-1)*(float)(pmax-pmin)+pmin;
              }
            else
              {
                int i=rMap.get(x,y);
                if(tile<=1)
                  values[P(x,y)]=(pmax+pmin)/2;
                else
                  values[P(x,y)]=rMap.get(i)/float(tile-1)*(pmax-pmin)+pmin;
              }
          }

    }

SplineMap::SplineMap(const Bitmap &b):values(b),w(b.w),h(b.h),rMap(1)
{}
  
int SplineMap::width() const
    {
      return w;
    }
int SplineMap::height() const
    {
      return h;
    }

std::string SplineMap::toString() const
      {
        return md5(values.toString());
      }

    float SplineMap::getPoint(int x,int y) const
      {
        if(x>=0 && y>=0 && x<w && y<h)
          return values[P(x,y)];
        else
          return 0;
      }

    void SplineMap::setPoint(int x,int y,const float&t)
    {
      if(x>=0 && y>=0 && x<w && y<h)
        values[P(x,y)]=t;
    }

    void SplineMap::edit(int x,int y,int v)
    {
      cdebug("edit:"<<x<<","<<y<<","<<v);
      if(x>=0 && y>=0 && x<w && y<h)
        {
          cdebug("settingt:"<<x<<","<<y<<","<<v);
          values[P(x,y)]+=v;
        }
    }

    std::string SplineMap::nice() const
      {
        std::ostringstream os;

        for(int i=0;i<h;i++)
          {
            for(int j=0;j<w;j++)
              {
                os<<values[P(j,i)]<<"\t";
              }
            os<<endl;
          }
        return os.str();
      }


    SplineMap SplineMap::getPatch(float px,float py,int pw,int ph) const
      {
        int x=(int)px;
        int y=(int)py;
#ifndef HIGH_VOXEL_DETAIL
	pw-=1;
	ph-=1;
#endif
        Bitmap b(MAP_SPLINE_ORDER+pw,MAP_SPLINE_ORDER+ph);

        for(int i=0;i<MAP_SPLINE_ORDER+pw;i++)
          for(int j=0;j<MAP_SPLINE_ORDER+ph;j++)
            b[P(i,j)]=values[P(x+i,y+j)];


#ifdef HIGH_VOXEL_DETAIL
	float v=b[P(2,2)];

	b[P(0,0)]=v;
	b[P(4,0)]=v;
	b[P(0,4)]=v;
	b[P(4,4)]=v;
	
	  // FIXME: this can only be included if the the control points are not control points but interpolation points!!!!!!!
	/*
	b[P(0,1)]=v;
	b[P(1,0)]=v;
	
	b[P(0,3)]=v;
	b[P(1,4)]=v;

	b[P(3,0)]=v;
	b[P(4,1)]=v;

	b[P(3,4)]=v;
	b[P(4,3)]=v;*/
#endif

        return SplineMap(b);
      }

    Pos3D SplineMap::getNormal(float x,float y) const
      {
        float mdx=dx(x,y);
        float mdy=dy(x,y);
//        if(mdx!=0 || mdy!=0)
//          cdebug(mdx<<"///"<<mdy);
        return Pos3D(mdx,1,mdy).normalized();
      }
#ifdef HIGH_VOXEL_DETAIL

    float SplineMap::dy(float x,float y) const
      {
        int ix((int)x);
        int iy((int)y);

        float v1=myspline(x-ix,
                    values[P(ix  ,iy  )],
                    values[P(ix+1,iy  )],
                    values[P(ix+2,iy  )],
                    values[P(ix+3,iy  )]);
        float v2=myspline(x-ix,
                    values[P(ix  ,iy+1)],
                    values[P(ix+1,iy+1)],
                    values[P(ix+2,iy+1)],
                    values[P(ix+3,iy+1)]);
        float v3=myspline(x-ix,
                    values[P(ix  ,iy+2)],
                    values[P(ix+1,iy+2)],
                    values[P(ix+2,iy+2)],
                    values[P(ix+3,iy+2)]);
        float v4=myspline(x-ix,
                    values[P(ix  ,iy+3)],
                    values[P(ix+1,iy+3)],
                    values[P(ix+2,iy+3)],
                    values[P(ix+3,iy+3)]);
        return myspline_derive(y-iy,v1,v2,v3,v4);
      }

    float SplineMap::dx(float x,float y) const
      {
        int ix((int)x);
        int iy((int)y);

        float v1=myspline(y-iy,
                    values[P(ix  ,iy  )],
                    values[P(ix  ,iy+1)],
                    values[P(ix  ,iy+2)],
                    values[P(ix  ,iy+3)]);
        float v2=myspline(y-iy,
                    values[P(ix+1,iy  )],
                    values[P(ix+1,iy+1)],
                    values[P(ix+1,iy+2)],
                    values[P(ix+1,iy+3)]);
        float v3=myspline(y-iy,
                    values[P(ix+2,iy  )],
                    values[P(ix+2,iy+1)],
                    values[P(ix+2,iy+2)],
                    values[P(ix+3,iy+3)]);
        float v4=myspline(y-iy,
                    values[P(ix+3,iy  )],
                    values[P(ix+3,iy+1)],
                    values[P(ix+3,iy+2)],
                    values[P(ix+3,iy+3)]);
        return myspline_derive(x-ix,v1,v2,v3,v4);
      }


    float SplineMap::get
      (float x,float y) const
        {
          // assume that if you take x=0, then we take values[1,0] as middle

          int ix((int)x);
          int iy((int)y);

          float v1=myspline(x-ix,
                      values[P(ix  ,iy  )],
                      values[P(ix+1,iy  )],
                      values[P(ix+2,iy  )],
                      values[P(ix+3,iy  )]);
          float v2=myspline(x-ix,
                      values[P(ix  ,iy+1)],
                      values[P(ix+1,iy+1)],
                      values[P(ix+2,iy+1)],
                      values[P(ix+3,iy+1)]);
          float v3=myspline(x-ix,
                      values[P(ix  ,iy+2)],
                      values[P(ix+1,iy+2)],
                      values[P(ix+2,iy+2)],
                      values[P(ix+3,iy+2)]);
          float v4=myspline(x-ix,
                      values[P(ix  ,iy+3)],
                      values[P(ix+1,iy+3)],
                      values[P(ix+2,iy+3)],
                      values[P(ix+3,iy+3)]);

          return myspline(y-iy,v1,v2,v3,v4);

        }
#else
    float SplineMap::dy(float x,float y) const
      {
        int ix((int)x);
        int iy((int)y);

	float ty=y-iy;
	ty*=0.5;
	float tx=x-ix;
	tx*=0.5;

        float v1=bezier(tx,
                    values[P(ix  ,iy  )],
                    values[P(ix+1,iy  )]);
	//                    values[P(ix+2,iy  )]);
        float v2=bezier(tx,
                    values[P(ix  ,iy+1)],
                    values[P(ix+1,iy+1)]);
	//                    values[P(ix+2,iy+1)]);
	/*        float v3=bezier(tx,
                    values[P(ix  ,iy+2)],
                    values[P(ix+1,iy+2)],
                    values[P(ix+2,iy+2)]);*/
        return bezier_derive(ty,v1,v2);
      }

    float SplineMap::dx(float x,float y) const
      {
        int ix((int)x);
        int iy((int)y);

	float ty=y-iy;
	ty*=0.5;
	float tx=x-ix;
	tx*=0.5;

        float v1=bezier(ty,
                    values[P(ix  ,iy  )],
                    values[P(ix  ,iy+1)]);
		    //                    values[P(ix  ,iy+2)]);
        float v2=bezier(ty,
                    values[P(ix+1,iy  )],
                    values[P(ix+1,iy+1)]);
		    //                    values[P(ix+1,iy+2)]);
	/*        float v3=bezier(ty,
                    values[P(ix+2,iy  )],
                    values[P(ix+2,iy+1)],
                    values[P(ix+2,iy+2)]);*/
        return bezier_derive(tx,v1,v2);
      }


    float SplineMap::get
      (float x,float y) const
        {
          // assume that if you take x=0, then we take values[1,0] as middle

          int ix((int)x);
          int iy((int)y);

	  float ty=y-iy;
	  ty*=0.5;
	  float tx=x-ix;
	  tx*=0.5;
	  
          float v1=bezier(tx,
                      values[P(ix  ,iy  )],
                      values[P(ix+1,iy  )]);
          float v2=bezier(tx,
                      values[P(ix  ,iy+1)],
                      values[P(ix+1,iy+1)]);


          return bezier(ty,v1,v2);

        }
#endif







/** (a0,a1,a2,a3) are at (-1,0,1,2) and x is in [0,1) */
float spline(float x,float a0,float a1,float a2,float a3)
{
  float i;

  i=(x+2.0)/3.0;
  float b0=a0*(1.0-i)+a1*i;

  i=(x+1.0)/3.0;
  float b1=a1*(1.0-i)+a2*i;

  i=x/3.0;
  float b2=a2*(1.0-i)+a3*i;


  i=(x+1.0)*0.5;
  float c0=b0*(1.0-i)+b1*i;

  i-=0.5;
  float c1=b1*(1.0-i)+b2*i;


  i=x;
  float d0=c0*(1.0-i)+c1*i;

  return d0;
}

float spline_derive(float x,float a0,float a1,float a2,float a3)
{
  float i;

  i=(x+2.0)/3.0;
  float b0=a0*(1.0-i)+a1*i;

  i=(x+1.0)/3.0;
  float b1=a1*(1.0-i)+a2*i;

  i=x/3.0;
  float b2=a2*(1.0-i)+a3*i;


  i=(x+1.0)*0.5;
  float c0=b0*(1.0-i)+b1*i;

  i-=0.5;
  float c1=b1*(1.0-i)+b2*i;


  i=x;
  float d0=3*(c1-c0);//c0*(1.0-i)+c1*i;

  return d0;
}


float spline(float x,float a0,float a1,float a2)
{
  float i;

  float b0=a0;
  float b1=a1;
  float b2=a2;


  i=(x+1.0)*0.5;
  float c0=b0*(1.0-i)+b1*i;

  i-=0.5;
  float c1=b1*(1.0-i)+b2*i;


  i=x;
  float d0=c0*(1.0-i)+c1*i;

  return d0;
}


float bezier(float x,float a0,float a1,float a2,float a3)
{
  float b0=(1.0f-x)*a0+x*a1;
  float b1=(1.0f-x)*a1+x*a2;
  float b2=(1.0f-x)*a2+x*a3;


  float c0=(1.0f-x)*b0+x*b1;
  float c1=(1.0f-x)*b1+x*b2;

  return (1.0f-x)*c0+x*c1;
}


template<class T>
T bezier(float x,const T&a0,const T& a1,const T& a2)
{
  T c0=a0*(1.0f-x)+a1*x;
  T c1=a1*(1.0f-x)+a2*x;

  return c0*(1.0f-x)+c1*x;
}

template<>
float bezier<float>(float x,const float&a0,const float&a2,const float&a3);

float bezier(float x,float a0,float a1)
{
  return a0*(1.0f-x)+a1*x;
}

float bezier_derive(float x,float a0,float a1,float a2)
{
  float c0=a0*(1.0f-x)+a1*x;
  float c1=a1*(1.0f-x)+a2*x;

  return 2*(c1-c0);
}


float bezier_derive(float x,float a0,float a1)
{
  return (a1-a0);
}

float catmull_rom(float t,float a0,float a1,float a2,float a3)
{
  float b0=a1;
  float b1=(-a0+6.0f*a1+a2)/6.0f;
  float b2=(a1+6.0f*a2-a3)/6.0f;
  float b3=a2;

  return bezier(t,b0,b1,b2,b3);
}
