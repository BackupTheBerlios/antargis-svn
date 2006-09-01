#ifndef MINIMAP_H
#define MINIMAP_H

#include "map.h"
#include <ag_widget.h>
#include <ag_texture.h>

class Scene;

/**
   MiniMap is the small map in lower corner.
   it's displayed through some single texture. the people are drawn on top of this.
   when the height-map changes, the texture has to be changed.
   So there are two different functions (mapChanged*)
 */
class MiniMap:public AGWidget
{
 public:
  MiniMap(AGWidget *p,const AGRect2 &r,AntMap *pMap);
  virtual ~MiniMap();
  
  /// changes only part of the texture - shouldn't be called if the whole texture is changed, because this is SLOW!
  bool mapChanged(AGEvent *e);
  /// call this only if the complete texture is changed - otherwise this is SLOW!
  bool mapChangedComplete(AGEvent *e);

    void draw(AGPainter &p);

  void setMap(AntMap *pMap);
  void setScene(Scene *pScene);

  virtual bool eventMouseClick(AGEvent *m);
  virtual bool eventMouseButtonDown(AGEvent *m);

 private:
  // adds some fancy noise to the map display
  float getNoise(int x,int y);

  AGVector2 toMapCoords(AGVector2 v) const;
  AGVector2 fromMapCoords(AGVector2 v) const;

  void mapChangedP(bool force);

  void drawEntities(AGPainter &p);

  float mMapBorder;
  AntMap *mMap;
  Scene *mScene;
  AGTexture *mTexture;
  AGSurface mSurface;
};

MiniMap *toMiniMap(AGWidget *w);

#endif
