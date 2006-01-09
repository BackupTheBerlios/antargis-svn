#ifndef MINIMAP_H
#define MINIMAP_H

#include "map.h"
#include <ag_widget.h>

class Scene;

class MiniMap:public AGWidget
{
 public:
  MiniMap(AGWidget *p,const AGRect &r,AntMap *pMap);
  
  bool mapChanged(AGEvent *e);
  bool mapChangedComplete(AGEvent *e);
  void draw(AGPainter &p);

  void setMap(AntMap *pMap);
  void setScene(Scene *pScene);

  virtual bool eventMouseClick(AGEvent *m);

 private:
  void mapChangedP(bool force);

  void drawEntities(AGPainter &p);

  AntMap *mMap;
  Scene *mScene;
  AGTexture mTexture;
  AGSurface mSurface;
};

MiniMap *toMiniMap(AGWidget *w);

#endif
