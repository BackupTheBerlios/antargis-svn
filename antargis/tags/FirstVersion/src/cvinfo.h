#ifndef CVINFO_H
#define CVINFO_H

#include <list>
#include "ant_geometry.h"
#include "serialize.h"
#include <ag_surface.h>

inline Uint32 toColor(int i)
{
  return i;
}

template<class T>
class CVBitmap
{
  int w,h;
  int add;
  std::vector<T> buffer;
 public:
  std::list<std::pair<Pos2D,T> > mBuffer;

  typedef typename std::list<std::pair<Pos2D,T> >::const_iterator buffer_iterator; 
  

  CVBitmap(int W,int H,int _add):
    w(W),h(H),buffer(w*h+_add)
    {
      add=_add;
    }
    ~CVBitmap()
      {
      }

    void reset()
    {
      typename std::vector<T>::iterator i=buffer.begin();
      
      for(;i!=buffer.end();++i)
	i->reset();
    }

    void set(int i,const T &b)
    {
      i+=add;
      assert(indexOk(i));
      buffer[i]=b;
    }
    void set(Pos2D p,const T &b)
    {
      int i=getIndex(p);
      i+=add;
      if(!indexOk(i))
	{
	  cdebug("failed:"<<p.x<<";"<<p.y);
	  return;
	}
      assert(indexOk(i));
      buffer[i]=b;
      mBuffer.push_back(std::make_pair(p,b));
    }
    T *get(int i)
    {
      i+=add;
      assert(indexOk(i));
      return &(buffer[i]);
    }
    T *get(Pos2D p)
    {
      int i=getIndex(p);
      return get(i);
    }
    T *getChecked(int i)
    {
      i+=add;
      if(indexOk(i))
	return &(buffer[i]);
      return 0;
    }
    T *getChecked(Pos2D p)
    {
      int i=getIndex(p);
      return getChecked(i);
    }
    bool valid(Pos2D p)
    {
      return p.x>=-w && p.y>=-h && p.x<w && p.y<h;
    }

    bool indexOk(int i)
    {
      return i>=0 && i<w*h*4;
    }

    int getIndex(Pos2D p)
    {
      return p.x+p.y*w;
    }

    void load(const std::string &ps)
    {
      Serial s(unzip(ps));
      s>>w>>h>>add;
      s>>buffer;
    }
    
    std::string toString() const
      {
	Serial s;
	s<<w<<h<<add;
	s<<buffer;
	return zip(s.str());
      }
    /*
    SDL_Surface *toSurface() const
      {
	AGSurface as(w*2,h*2);
	SDL_Surface *s=as.surface();
	typename std::vector<T>::const_iterator i=buffer.begin();
	for(int y=0;y<h*2;y++)
	  for(int x=0;x<w*2;x++)
	    {
	      sge_PutPixel(s,x,y,toColor(*i));
	      i++;
	    }
	return s;
      }
    */
};

struct ShadowInfo
{
  int depth;
  bool visible;
  Pos2D viewPos;

  ShadowInfo(int d,bool v,Pos2D p):depth(d),visible(v),viewPos(p)
  {
  }
  ShadowInfo():viewPos(0,0)
  {
    depth=10000;
    visible=false;
  }
  void reset()
  {
    depth=10000;
    visible=false;
  }
};

inline Serial &operator<<(Serial &s,const ShadowInfo &i)
{
  s<<i.depth<<i.visible<<i.viewPos;
  return s;
}

inline Serial &operator>>(Serial &s,ShadowInfo &i)
{
  s>>i.depth>>i.visible>>i.viewPos;
  return s;
}

inline std::ostream &operator<<(std::ostream &o,const ShadowInfo &i)
{
  o<<"("<<i.depth<<","<<i.visible<<","<<i.viewPos<<")";
  return o;
}

struct DepthInfo
{
  int depth;
  Pos2D shadowPos;

  DepthInfo(int d,Pos2D s):depth(d),shadowPos(s)
  {
  }
  DepthInfo():depth(10000),shadowPos(0,0)
  {
  }
  void reset()
  {
    depth=10000;
  }
};


inline Serial &operator<<(Serial &s,const DepthInfo &i)
{
  s<<i.depth<<i.shadowPos;
  return s;
}

inline Serial &operator>>(Serial &s,DepthInfo &i)
{
  s>>i.depth>>i.shadowPos;
  return s;
}

inline std::ostream &operator<<(std::ostream &o,const DepthInfo &i)
{
  o<<"("<<i.depth<<","<<i.shadowPos<<")";
  return o;
}

#endif
