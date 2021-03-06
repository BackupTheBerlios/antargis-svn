#ifndef MINIMAP_H
#define MINIMAP_H

#include "map.h"
#include <ag_widget.h>

class Scene;

class MiniMap:public AGWidget
{
 public:
  MiniMap(AGWidget *p,const AGRect &r,AntMap *pMap);
  
  void mapChanged();
  void draw(AGPainter &p);

  void setMap(AntMap *pMap);
  void setScene(Scene *pScene);

 private:
  AntMap *mMap;
  Scene *mScene;
  AGTexture mTexture;
  AGSurface mSurface;
};

MiniMap *toMiniMap(AGWidget *w);

#endif
