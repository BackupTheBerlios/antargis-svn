/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * voxel_gen.h
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

#ifndef __VOXEL_GEN_H__
#define __VOXEL_GEN_H__

#include <ag_surface.h>
#include <ag_widget.h>

#include "decast.h"

#define POINTS_PER_TILE 2
#define TILE_WIDTH 64
#define TILE_SIZE 32
#define WATER_HEIGHT 10

#define MAP_SPLINE_ORDER 3

#define TILEDIR std::string("tiles/")

#define SplineMapD SplineMap<float,4> //int brings some small improvement - but makes problems with water

class AntargisView;

class VoxelImageData
{
  std::map<std::string,Pos2D> mPositions;
  public:
    VoxelImageData();
    virtual ~VoxelImageData();
    Pos2D getCenter(const std::string &pFilename);
    void setCenter(const std::string &pFilename,const Pos2D &pPos);

    private:
void loadXML(const xmlpp::Node &n);
void saveXML(xmlpp::Node &n) const;
};

VoxelImageData *getVoxelID();


class AVItem
{
 public:
  AVItem(const Pos3D &p);
  
  virtual ~AVItem();


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
  virtual void draw(AntargisView *view,AGPainter &p);//7const AGRect &r);


 private:
  bool inited;
  Pos3D mPos;
  protected:
  Pos2D mCenter;
  
  int virtualY;
  
 protected:
  AGSurface mSurface;
  AGTexture *mTexture;
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

  void draw(AGPainter &p);//const AGRect &r);
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
  virtual void draw(AntargisView *view,AGPainter &p);//const AGRect &r);
  
  void cutBorders();

};



VoxelImage *makeTerrainTile(const SplineMapD &m,const SplineMapD &gm,int px,int py);
VoxelImage *makeWaterTile();


#endif
