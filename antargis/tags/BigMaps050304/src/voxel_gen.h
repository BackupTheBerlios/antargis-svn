#ifndef __VOXEL_GEN_H__
#define __VOXEL_GEN_H__

#include <ag_gsurface.h>
#include <ag_widget.h>

#include "decast.h"

#define POINTS_PER_TILE 2
#define TILE_WIDTH 64
#define TILE_SIZE 32
#define WATER_HEIGHT 10

#define MAP_SPLINE_ORDER 3

#define SplineMapD SplineMap<float,4> //int brings some small improvement - but makes problems with water

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



VoxelImage *makeTerrainTile(const SplineMapD &m,const SplineMapD &gm,int px,int py);
VoxelImage *makeWaterTile();


#endif
