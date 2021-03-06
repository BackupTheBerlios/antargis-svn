#ifndef MESH_DATA_H
#define MESH_DATA_H

#include "scenenode.h"
#include "mesh_data.h"
#include "vertex_array.h"

// Mesh organizing

class MeshData:public SceneNode
{
  VertexArray mArray;
  AGTexture mTexture;
  bool mWithTexture;
  bool mShadow;

  AGBox3 mBBox;

  bool mTransparent;

  bool overdraw;
  bool drawColors;
  bool mPickable;
  bool mLighting;


 public:
  MeshData(const std::string &filename,float zoom,const std::string &pTexture="",bool pShadow=true);
  MeshData(const VertexArray &va,const std::string &pTexture,bool pShadow=true);
  ~MeshData();

  //  void setAlpha(bool pAlpha);
  //  bool getAlpha() const;

  void draw();
  void drawShadow();
  void drawDepth();
  void drawPick();

  AGBox3 bbox() const;

  void save(const std::string &pFilename);

  virtual size_t getTriangles() const;

  virtual AGVector4 lineHit(const AGLine3 &pLine) const
  {
    return mArray.lineHit(pLine);
  }
  void texCoordFromPos(float scale);
  void setPickable(bool p);

  void setTransparent(bool p);
  virtual bool transparent();
  void setLighting(bool l);

  void setOverdraw(bool o);
  void setColors(bool c);
};


#endif
