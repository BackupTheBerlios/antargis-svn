#ifndef MESH_DATA_H
#define MESH_DATA_H

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

  AGBox3 mBBox;

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

};


#endif
