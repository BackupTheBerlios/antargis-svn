#ifndef AG_IMAGE_H
#define AG_IMAGE_H

#include "ag_widget.h"
#include "ag_surface.h"

class AGImage:public AGWidget
{
 public:
  AGImage(AGWidget *pParent,const AGPoint &p,AGSurface pSurface,bool pTile,const AGRect &pRect=AGRect(0,0,0,0));
  virtual ~AGImage();

  virtual void draw(const AGRect &r);

  void setSurface(AGSurface pSurface);

  //  void setSourceRect(const AGRect &pRect);

 private:
  AGTexture mTexture;
  bool mTile;
  AGRect mSrcRect;
};

#endif
