#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include <ag_widget.h>
#include <ag_gsurface.h>

class AntargisMap;

class MiniMap:public AGWidget
  {
  public:
    MiniMap(AGWidget *pParent,const AGRect &r,AntargisMap *pMap);

    virtual void draw(const AGRect &r);

    void update();

  private:
    AntargisMap *mMap;
    AGTexture mTexture;
    AGTexture mBG;
    AGSurface mSurface;
    bool mustUpdate;
  };

#endif
