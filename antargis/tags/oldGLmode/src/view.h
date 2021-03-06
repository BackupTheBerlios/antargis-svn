/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * view.h
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

#ifndef __ANT_ISOVIEW
#define __ANT_ISOVIEW

#include "rain.h"
#include "map.h"
#include <math.h>
#include <ag_button.h>

struct IVTile
{
  int x,y;
  bool operator<(const IVTile &t) const
  {
    return x<t.x || (x==t.x && y<t.y);
  }
};

class IsoView:public AntargisView, public MapListener
  {
  public:
  private:

    AntargisMap *mMap;
    std::map<AVItem*,IVTile> mTiles;
    std::map<IVTile,AVItem*> mTileCache;
    float mTime;
    bool inited;
    Rain mRain;
    
  protected:
    std::map<AVItem*,AntEntity*> mEntities;
    std::map<AntEntity*,AVItem*> mEntitiesInv;
    std::set
      <AVItem*> mSelected;
    bool shallUpdate;
    Pos3D maxPos;
    
  public:
    IsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map);
    void completeUpdate()
    {
      initTileCache();
      update();
    }
    void update();
    void checkView();

    void mapUpdate();


  protected:
    virtual void init();
    void initTileCache();


    VoxelImage *getTree();

    VoxelImage *getSurface(const SplineMapD &h,const SplineMapD &g);
    VoxelImage *getWaterTile();

  public:
    // p is local window-coordinate
    std::vector<AntEntity *> getEntity(const AGPoint &pp);

    IVTile getTile(const AGPoint &pp);

    //  protected:
    virtual void draw(AGPainter &p);//const AGRect &r);
  protected:
    void updatePositions();

    AGColor getSelectColor();
    Pos2D getTilePos(const IVTile &t);
  };


class CompleteIsoView: public IsoView
  {
    /*
    std::vector<AntEntity*> mClicked;
    Pos2D mMapClick;
    */
  public:
    CompleteIsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map);

    AntHero *getHero(std::vector<AntEntity*> &es);
    virtual bool eventDragBy(const AGEvent *event,const AGPoint &pDiff);
    bool eventMouseClick(const AGEvent *m);

    bool isMyHero(AntEntity *h);
    /*
    Pos2D mapClick() const;
    std::vector<AntEntity*> entityClick() const;

    AGSignal sigMapClick;
    AGSignal sigEntityClick;
*/
    virtual void clickEntities(const std::vector<AntEntityPtr> &ents);
    virtual void clickMap(const Pos2D &p);

  };

class EditIsoView: public CompleteIsoView
  {
    std::list<AVItem*> mPoints;
    VoxelImage *mOldPoint;
    bool mShowPoints;
    bool mEditing;
    int editSize;
    
    //std::list<AGWidget*> mEditWidgets;
    AGWidget *mLayout;
    
    //std::map<AGMessageObject*,int> mEditButtons;
    //AGButton *allWater,*allGrass;
    
    std::string mAddEntity;
    
  public:
    EditIsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map);

    void toggleEdit();
    
    bool selectSize(const std::string&name,const AGEvent *e,AGMessageObject *pCaller);
    bool setAll(const std::string&name,const AGEvent *e,AGMessageObject *pCaller);
    bool addEntity(const std::string&name,const AGEvent *e,AGMessageObject *pCaller);
    bool setRubber(const std::string&name,const AGEvent *e,AGMessageObject *pCaller);
    bool togglePoints(const std::string&name,const AGEvent *e,AGMessageObject *pCaller);

    virtual void editMarkClicked(const Pos2D &p,const AGSDLEvent *e);

    // moving about
    virtual bool eventDragBy(const AGEvent *event,const AGPoint &pDiff);
    bool eventMouseClick(const AGEvent *m);
    AVItem *getClosest(const AGPoint &p) const;

    void toggleShowPoints();
    bool eventMouseMotion(const AGEvent *m);

    void editAt(const Pos2D &p,bool dir);
    
    AGSignal sigMapEdited;
    
  protected:
    virtual void init();
  };

EditIsoView &toEditIsoView(AGWidget &w);

#endif
