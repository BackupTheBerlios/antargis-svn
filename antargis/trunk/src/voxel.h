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

#define POINTS_PER_TILE 2
#define TILE_WIDTH 64
#define TILE_SIZE 32
#define WATER_HEIGHT 10

#define MAP_SPLINE_ORDER 3

//typedef SlineMap<float,3> SplineMapD;
#define SplineMapD SplineMap<float,4> //int brings some small improvement - but makes problems with water

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

class AntargisView;


class AVItem
{
 public:
  AVItem(const Pos3D &p);
  
  virtual ~AVItem()
  {
  }

  AGSurface &getSurface();
  AGTexture &getTexture();
  AGPoint getPosition(const Pos3D &pPos) const;
  AGRect getRect(const Pos3D &pPos) const;

  int getZ(const Pos3D &pPos) const;

  virtual void init()=0;
  void setPosition(const Pos3D &pPos);
  Pos3D getPosition() const
  {
    return mPos;
  }
  
  void setCenter(const Pos2D &pCenter);
  const Pos2D &getCenter() const;
  
  Uint8 getAlpha(const AGPoint &p) const;
  
  void setVirtualY(int v)
  {
    virtualY=v;
  }
  
//  virtual void draw(const Pos3D &diff,const AGRect &r);
  virtual void draw(AntargisView *view,const AGRect &r);


 private:
  bool inited;
  Pos3D mPos;
  Pos2D mCenter;
  
  int virtualY;
  
 protected:
  AGSurface mSurface;
  AGTexture mTexture;
};


class AntargisView:public AGWidget
{
 protected: 
  std::vector<AVItem*> mItems;
  Pos3D mPos;
  bool ownsItems;
 public:
  AntargisView(AGWidget *parent,const AGRect &pRect,const Pos3D &pPos,bool pOwnsItems);
  
  AGRect getRect(AVItem *item) const;

  void draw(const AGRect &r);
  void insert(AVItem *v);
  
  void clear();
};







class VoxelImage:public AVItem
{
  std::string mName;
 public:
  VoxelImage(AGSurface pSurface,Pos3D pPos);
  VoxelImage(const std::string &pFilename);
  virtual void init();
  
  void setTexture(const std::string &pFilename);
  

  void save(const std::string &pFilename);

  void setName(const std::string &pName);
  virtual void draw(AntargisView *view,const AGRect &r);
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
