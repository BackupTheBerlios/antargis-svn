#ifndef __MESH_H
#define __MESH_H

#include "scene.h"
#include "vertex_array.h"

#include <map>

// simple Mesh building

struct MeshVertex
{
  AGVector4 v,c;
  AGVector3 n;
  AGVector2 t;

  // sorting
  bool operator<(const MeshVertex &p) const;
};

// generates index list for same vertices (with same colors/normals..)
class MeshOptimizer
{
  std::map<MeshVertex,int> mMap;
  std::vector<int> mIndices;

  std::vector<MeshVertex> mVertices;
  int saved;
 public:
  MeshOptimizer();

  void add(const MeshVertex &v);
  VertexArray getArray();
};




// Mesh organizing

class MeshData:public SceneNode
{
  VertexArray mArray;
  AGTexture mTexture;
  bool mWithTexture;
  bool mShadow;
  //  bool mAlpha;

 public:
  MeshData(const std::string &filename,float zoom,const std::string &pTexture="",bool pShadow=true);
  MeshData(const VertexArray &va,const std::string &pTexture,bool pShadow=true);
  ~MeshData();

  //  void setAlpha(bool pAlpha);
  //  bool getAlpha() const;

  void draw();
  void drawShadow();
  void drawDepth();

  void save(const std::string &pFilename);

  virtual size_t getTriangles() const;

  virtual AGVector4 lineHit(const AGLine3 &pLine) const
  {
    return mArray.lineHit(pLine);
  }
  void texCoordFromPos(float scale);

};

class Mesh:public SceneNode
{
  MeshData *mData;
  AGVector4 mPos;
  float mRotation;
  
 public:
  Mesh();
  Mesh(MeshData &data,const AGVector4 &pPos,float pRot);
  virtual ~Mesh();

  void draw();
  void drawDepth();
  void drawShadow();

  virtual AGVector4 lineHit(const AGLine3 &pLine) const;

  virtual size_t getTriangles() const;
  void setPos(const AGVector3&pPos);
  void setRotation(float r);

  MeshData *getData();

 private:
  void begin();
  void end();
};

Mesh *toMesh(SceneNode *node);

#endif
