#ifndef AG_VTILER_H
#define AG_VTILER_H

#include "ag_widget.h"

class AGVTiler:public AGWidget
{
 public:
  AGVTiler(AGWidget *pParent,const AGRect&pRect,bool pAdaptMyHeight=true);


  virtual void addChild(AGWidget *pWidget);

  virtual void rePosition();
  //void adaptWidth();
  void adaptHeightFromChildren();
  void adaptWidthFromChildren();
 protected:
  bool mAdaptMyHeight;
};

class AGHTiler:public AGVTiler
{
 public:
  AGHTiler(AGWidget *pParent,const AGRect&pRect,bool pAdaptMyHeight=true);
  virtual void rePosition();
  void adaptHeightFromChildren();
  void adaptWidthFromChildren();
  void adaptWidthFromParent();
};

#endif
