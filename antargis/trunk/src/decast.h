#ifndef DECAST_H
#define DECAST_H

#include <vector>
#include <sstream>
#include <map>


#include "ag_debug.h"
#include "ant_geometry.h"
#include "store.h"
#include "md5.h"
#include "random_map.h"

#define P(x,y) std::make_pair(x,y)


template<class T>
class Bitmap:public std::vector<T>
  {
  public:
    int w,h;
  public:
    Bitmap(int W,int H):std::vector<T>(W*H),
        w(W),h(H)
    {}
    Bitmap(const Bitmap<T> &b):
        std::vector<T>(b),
        w(b.w),h(b.h)
    {}
    T &operator[](std::pair<int,int> p)
    {
      while(p.first>=w)
        p.first-=w;
      while(p.second>=w)
        p.second-=w;
      assert(p.first>=0 && p.second>=0 && p.first<w && p.second<h);
      return std::vector<T>::operator[](p.first+p.second*w);
    }
    
    std::string toString() const
    {
      const T *p=&(front());
      
      return std::string((char*)const_cast<T*>(p),w*h*sizeof(T));
    }

    const T &operator[](std::pair<int,int> p) const
      {
//        static int maxx=0,maxy=0;
        /*if(p.first>maxx||p.second>maxy)
          {
            maxx=std::max(p.first,maxx);
            maxy=std::max(p.second,maxy);
            cdebug("maxa:"<<maxx<<";"<<maxy);
          }*/
        while(p.first>=w)
          p.first-=w;
        while(p.second>=w)
          p.second-=w;
        while(p.first<0)
          p.first+=w;
        while(p.second<0)
          p.second+=w;

        //  cdebug(p.first<<" "<<w);

        assert(p.first>=0 && p.second>=0 && p.first<w && p.second<h);
        return std::vector<T>::operator[](p.first+p.second*w);
      }

  };

/*
  0   1   2   3   4   5
  |---|---|---|---|---|
  ........|--x|
  ....|------x|
  ........|--x----|
  |----------x|
  ....|------x----|
  ........|--x--------|
*/

template<class T>
T splineV(double x,const std::vector<T> &v)
{
  if(v.size()==1)
    return *v.begin();

  double s=1.0/(v.size()-1),is,add
           =v.size()-1;
  std::vector<T> a;
  typename std::vector<T>::const_iterator i=v.begin();
  typename std::vector<T>::const_iterator j=v.begin();
  j++;

  for(;j!=v.end();i++,j++)
    {
      is=(x+add
         )*s;
      add
        -=1.0;
      a.push_back((*i)*(1.0-is) + (*j)*is);
    }
  return splineV(x,a);
}

/** (a0,a1,a2,a3) are at (-1,0,1,2) and x is in [0,1) */
template<class T>
T spline(double x,T a0,T a1,T a2,T a3)
{
  double i;

  i=(x+2.0)/3.0;
  T b0=a0*(1.0-i)+a1*i;

  i=(x+1.0)/3.0;
  T b1=a1*(1.0-i)+a2*i;

  i=x/3.0;
  T b2=a2*(1.0-i)+a3*i;


  i=(x+1.0)*0.5;
  T c0=b0*(1.0-i)+b1*i;

  i-=0.5;
  T c1=b1*(1.0-i)+b2*i;


  i=x;
  T d0=c0*(1.0-i)+c1*i;

  return d0;
}

template<class T>
T spline(double x,T a0,T a1,T a2)
{
  double i;

  T b0=a0;
  T b1=a1;
  T b2=a2;


  i=(x+1.0)*0.5;
  T c0=b0*(1.0-i)+b1*i;

  i-=0.5;
  T c1=b1*(1.0-i)+b2*i;


  i=x;
  T d0=c0*(1.0-i)+c1*i;

  return d0;
}

// recursive
template<class T>
T bezierV(float x,const std::vector<T> &v)
{
  float ix=1-x;
  std::vector<T> a;
  typename std::vector<T>::const_iterator i=v.begin();
  typename std::vector<T>::const_iterator j=v.begin();
  j++;
  if(j==v.end())
    return *i;
  for(;j!=v.end();i++,j++)
    a.push_back(ix*(*i)+x*(*j));

  return bezierV(x,a);
}


template<class T>
T bezier(float x,T a0,T a1,T a2,T a3)
{
  T b0=(1.0f-x)*a0+x*a1;
  T b1=(1.0f-x)*a1+x*a2;
  T b2=(1.0f-x)*a2+x*a3;


  T c0=(1.0f-x)*b0+x*b1;
  T c1=(1.0f-x)*b1+x*b2;

  return (1.0f-x)*c0+x*c1;
}


template<class T>
T bezier(float x,T a0,T a1,T a2)
{
  T b0=a0;
  T b1=a1;
  T b2=a2;

  T c0=b0*(1.0f-x)+b1*x;
  T c1=b1*(1.0f-x)+b2*x;

  return c0*(1.0f-x)+c1*x;
}


// order for cubic is 4
// order for quadratic is 3

// Cubic Uniform Spline map
template<class T,int order>
class SplineMap
  {
    Bitmap<T> values;
    int w,h;
    RandomMap rMap;
    bool realRandom;
  public:
    SplineMap(int W,int H,int tile,int pmin,int pmax,bool rrand=false):
        values(W,H),w(W),h(H),
#ifdef EDITING        
        rMap(tile,1)
#else
        rMap(tile,4)
#endif        
        
    {
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
            int i=rMap.get(x)+rMap.get(y);
            if(tile<=1)
              values[P(x,y)]=(pmax+pmin)/2;
            else              
              values[P(x,y)]=rMap.get(i)/float(tile-1)*(pmax-pmin)+pmin;
             }
          }

    }

    SplineMap(Bitmap<T> b):values(b),w(b.w),h(b.h),rMap(1)
    {}

    std::string toString() const
      {
      /*
        std::ostringstream os;

        os<<"sp";
        for(int i=0;i<h;i++)
          for(int j=0;j<w;j++)
            os<<"_"<<values[P(j,i)];*/
        return md5(values.toString());//os.str());
      }
      
    T getPoint(int x,int y) const
    {
      if(x>=0 && y>=0 && x<w && y<h)
        return values[P(x,y)];
      else
        return 0;
    }
    
    void setPoint(int x,int y,const T&t)
    {
      if(x>=0 && y>=0 && x<w && y<h)
        values[P(x,y)]=t;
    }
      
    void edit(int x,int y,int v)
    {
      cdebug("edit:"<<x<<","<<y<<","<<v);
      if(x>=0 && y>=0 && x<w && y<h)
      {
        cdebug("settingt:"<<x<<","<<y<<","<<v);
        values[P(x,y)]+=v;
      }
    }

    std::string nice() const
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


    SplineMap<T,order> getPatch(float px,float py,int pw,int ph) const
      {
        int x=(int)px;
        int y=(int)py;
        Bitmap<T> b(order+pw,order+ph);
        for(int i=0;i<order+pw;i++)
          for(int j=0;j<order+ph;j++)
            b[P(i,j)]=values[P(x+i,y+j)];
        return SplineMap<T,order>(b);
      }
      
    Pos3D getNormal(float x,float y) const
    {
      Pos3D p(0,1,0);
      
      return p;
    }

    T get
      (float x,float y) const
        {
          /*      std::vector<T> a,v;

          int ix((int)x);
          int iy((int)y);

          float dx=x-ix;
          float dy=y-iy;

          for(int mx=0;mx<order;mx++)
          {
          for(int my=0;my<order;my++)
          {
           v.push_back(values[P(ix+mx,iy+my)]);
          }
          a.push_back(splineV(dy,v));
          v.clear();
          }
          return splineV(dx,a);


          */

          // assume that if you take x=0, then we take values[1,0] as middle

          int ix((int)x);
          int iy((int)y);

          T v1=spline(x-ix,
                      values[P(ix  ,iy  )],
                      values[P(ix+1,iy  )],
                      values[P(ix+2,iy  )],
                      values[P(ix+3,iy  )]);
          T v2=spline(x-ix,
                      values[P(ix  ,iy+1)],
                      values[P(ix+1,iy+1)],
                      values[P(ix+2,iy+1)],
                      values[P(ix+3,iy+1)]);
          T v3=spline(x-ix,
                      values[P(ix  ,iy+2)],
                      values[P(ix+1,iy+2)],
                      values[P(ix+2,iy+2)],
                      values[P(ix+3,iy+2)]);
          T v4=spline(x-ix,
                      values[P(ix  ,iy+3)],
                      values[P(ix+1,iy+3)],
                      values[P(ix+2,iy+3)],
                      values[P(ix+3,iy+3)]);

          return spline(y-iy,v1,v2,v3,v4);

        }
  };


#endif
