#ifndef NEW_DECAL_H
#define NEW_DECAL_H

#include "scenenode.h"
#include "height_map.h"
#include "vertex_array.h"
#include "ag_texture.h"

/** not yet used
    decals are currently a little bad in BoA.
    Should be reworked!!!
*/
class NewDecal:public SceneNode
{
  AGVector2 mPos;
  float mSize;
  HeightMap *mMap;
  AGTexture mTexture;
  VertexArray mArray;
  bool inited;

 public:
  NewDecal(Scene *pScene,AGVector2 pos,float size, HeightMap *pMap,const std::string &pTexture);
  
  void setPos(const AGVector3&pPos);
  void draw();

  void init();
  virtual void mapChanged();


};

#endif
