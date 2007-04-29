#ifndef MESH_OPTIMIZER
#define MESH_OPTIMIZER

#include "ag_geometry.h"
#include "vertex_array.h"

#include <map>

/** This file provides functionality for:
    building of an indexed mesh out of a set of (vertex,color,normal,tex2dcoords)
    each 3 of them a triangle. Similar vertices will be joined automatically.
    Use it like this:
    
    1) generate a MeshOptimizer objet
    2) add 3*n vertices
    3) get a VertexArray by calling getArray()
*/

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


// some old function ???
// used in gen_tree ?? this gen_tree shouldn't be used anymore, too
MeshOptimizer loadFromText(const std::string &pText, bool withTex, float zoom);

#endif
