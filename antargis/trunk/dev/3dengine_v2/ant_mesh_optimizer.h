#ifndef MESH_OPTIMIZER
#define MESH_OPTIMIZER

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include "ag_geometry.h"
#include "ant_static_mesh_data.h"

#include <map>

/** This file provides functionality for:
    building of an indexed mesh out of a set of (vertex,color,normal,tex2dcoords)
    each 3 of them a triangle. Similar vertices will be joined automatically.
    Use it like this:
    
    1) generate a MeshOptimizer objet
    2) add 3*n vertices
    3) get a AntStaticMeshData by calling getArray()
*/

struct AGEXPORT AntMeshVertex
{
  AGVector4 v,c;
  AGVector3 n;
  AGVector2 t;

  // sorting
  bool operator<(const AntMeshVertex &p) const;
};

// generates index list for same vertices (with same colors/normals..)
class AGEXPORT AntMeshOptimizer
{
  std::map<AntMeshVertex,int> mMap;
  std::vector<int> mIndices;

  std::vector<AntMeshVertex> mVertices;
  int saved;
 public:
  AntMeshOptimizer();

  void add(const AntMeshVertex &v);
  AntStaticMeshData *getNewMesh();

  AGVector4 getV(size_t i);
};


// some old function ???
// used in gen_tree ?? this gen_tree shouldn't be used anymore, too
AGEXPORT AntMeshOptimizer loadFromText(const std::string &pText, bool withTex, float zoom);

#endif
