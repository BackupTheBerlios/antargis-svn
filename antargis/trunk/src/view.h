#ifndef __ANT_ISOVIEW
#define __ANT_ISOVIEW

#include "rain.h"
#include "map.h"
#include <math.h>
#include <ag_button.h>

class IsoView:public AntargisView
  {
  public:
    struct IVTile
      {
        int x,y;
        bool operator<(const IVTile &t) const
        {
          return x<t.x || (x==t.x && y<t.y);
        }
      };
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


  protected:
    virtual void init();
    void initTileCache();


    VoxelImage *getTree();

    VoxelImage *getSurface(const SplineMapD &h,const SplineMapD &g);
    VoxelImage *getWaterTile();

  public:
    // p is local window-coordinate
    std::list<AntEntity *> getEntity(const AGPoint &pp);

    IVTile getTile(const AGPoint &pp);

  protected:
    virtual void draw(const AGRect &r);
    void updatePositions();

    AGColor getSelectColor();
    Pos2D getTilePos(const IVTile &t);
  };


class CompleteIsoView: public IsoView
  {
  public:
    CompleteIsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map);

    AntHero *getHero(std::list<AntEntity*> &es);
    virtual bool eventDragBy(const AGEvent *event,const AGPoint &pDiff);
    bool eventMouseClick(const AGEvent *m);

    bool isMyHero(AntHero *h);

  };

class EditIsoView: public CompleteIsoView
  {
    std::list<AVItem*> mPoints;
    VoxelImage *mOldPoint;
    bool mShowPoints;
    bool mEditing;
    int editSize;
    
    std::list<AGWidget*> mEditWidgets;
    
    std::map<AGMessageObject*,int> mEditButtons;
    AGButton *allWater,*allGrass;
    
    std::string mAddEntity;
    
  public:
    EditIsoView(AGWidget *parent,AGRect r,Pos3D p,AntargisMap *map);

    void toggleEdit();
    
    bool selectSize(const char *name,const AGEvent *e,AGMessageObject *pCaller);
    bool setAll(const char *name,const AGEvent *e,AGMessageObject *pCaller);
    bool addEntity(const char *name,const AGEvent *e,AGMessageObject *pCaller);
    bool setRubber(const char *name,const AGEvent *e,AGMessageObject *pCaller);

    // moving about
    virtual bool eventDragBy(const AGEvent *event,const AGPoint &pDiff);
    bool eventMouseClick(const AGEvent *m);
    AVItem *getClosest(const AGPoint &p) const;

    void toggleShowPoints();
    bool eventMouseMotion(const AGEvent *m);

    void editAt(const Pos3D &p,bool dir);
    
    AGSignal sigMapEdited;
    
  protected:
    virtual void init();
  };

#endif
