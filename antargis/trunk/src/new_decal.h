#ifndef NEW_DECAL_H
#define NEW_DECAL_H

#include "scenenode.h"
#include "height_map.h"
#include "vertex_array.h"

class NewDecal:public SceneNode
{
  AGVector2 mPos;
  float mSize;
  HeightMap *mMap;
  AGTexture mTexture;
  VertexArray mArray;
  AGVector3 mPos3;
  AGBox3 mBBox;
  bool inited;

 public:
  NewDecal(AGVector2 pos,float size, HeightMap *pMap,const std::string &pTexture);
  
  void setPos(const AGVector3&pPos);
  void draw();
  AGBox3 bbox() const;

  void init();
  virtual void mapChanged();


};

#endif
