#ifndef MESH_OPTIMIZER
#define MESH_OPTIMIZER

#include "ag_triangle.h"
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

  AGVector4 getV(size_t i);
};


#endif
