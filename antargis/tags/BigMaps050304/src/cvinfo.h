#ifndef CVINFO_H
#define CVINFO_H

#include <list>
#include "ant_geometry.h"
#include <ag_surface.h>

inline Uint32 toColor(int i)
{
  return i;
}

template<class T>
class CVBitmap
  {
    int w,h;
    int add
      ;
    std::vector<T> buffer;
  public:
    std::list<std::pair<Pos2D,T> > mBuffer;

    typedef typename std::list<std::pair<Pos2D,T> >::const_iterator buffer_iterator;


    CVBitmap(int W,int H,int _add):
        w(W),h(H),buffer(w*h+_add)
    {
      add
        =_add;
    }
    ~CVBitmap()
  {}

    void reset()
    {
      typename std::vector<T>::iterator i=buffer.begin();

      for(;i!=buffer.end();++i)
        i->reset();
    }

    void set
      (int i,const T &b)
      {
        i+=add
           ;
        assert(indexOk(i));
        buffer[i]=b;
      }
    void set
      (Pos2D p,const T &b)
      {
        int i=getIndex(p);
        i+=add
           ;
        if(!indexOk(i))
          {
            cdebug("failed:"<<p.x<<";"<<p.y);
            return;
          }
        assert(indexOk(i));
        buffer[i]=b;
        mBuffer.push_back(std::make_pair(p,b));
      }
    T *get
    (int i)
    {
      i+=add
         ;
      assert(indexOk(i));
      return &(buffer[i]);
    }
    T *get
    (Pos2D p)
    {
      int i=getIndex(p);
      return get
               (i);
    }
    T *getChecked(int i)
    {
      i+=add
         ;
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
      return (int)(p.x+p.y*w);
    }

  };

struct ShadowInfo
  {
    int depth;
    bool visible;
    Pos2D viewPos;

    ShadowInfo(int d,bool v,Pos2D p):depth(d),visible(v),viewPos(p)
    {}
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

struct DepthInfo
  {
    int depth;
    Pos2D shadowPos;

    DepthInfo(int d,Pos2D s):depth(d),shadowPos(s)
    {}
    DepthInfo():depth(10000),shadowPos(0,0)
    {}
    void reset()
    {
      depth=10000;
    }
  };


#endif
