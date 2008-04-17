#ifndef ANT_STATIC_MESH_DATA_H
#define ANT_STATIC_MESH_DATA_H

#include "ag_geometry.h"

#include <vector>

class AntStaticMeshData
  {
    std::vector<AGVector4> mVertices,mColors;
    std::vector<AGVector3> mNormals;
    std::vector<AGVector2> mTexCoords2D;
    std::vector<AGVector3> mTexCoords3D;
    std::vector<Uint16> mIndices;

    AGBox3 mBBox;
    size_t mChangeLevel;
public:
  AntStaticMeshData();
  ~AntStaticMeshData();

  void addVertex(AGVector4 pVertex, AGVector4 pColor, AGVector3 pNormal, AGVector2 pTex);
  void addVertex(AGVector4 pVertex, AGVector4 pColor, AGVector3 pNormal, AGVector3 pTex);
  void addTriangle(size_t p0,size_t p1,size_t p2);

  void save(const AGString &pFilename);

  static AntStaticMeshData *load(const AGString &pFilename,float zoom=1.0f);

  size_t getTriangleCount() const;
  size_t getIndexCount() const;
  size_t getVertexCount() const;
  size_t getColorsCount() const;
  size_t getNormalsCount() const;
  size_t getTexCoords2DCount() const;
  size_t getTexCoords3DCount() const;

  size_t getIndex(size_t i);
  AGVector4 getVertex(size_t i);
  AGVector4 getColor(size_t i);
  AGVector3 getNormal(size_t i);
  AGVector2 getTexCoord2D(size_t i);
  AGVector3 getTexCoord3D(size_t i);
  
  Uint16 *getIndices();
  float *getVertices();
  float *getColors();
  float *getNormals();
  float *getTexCoords2D();
  float *getTexCoords3D();
  
  bool has2DTextures() const;
  bool has3DTextures() const;
  
  size_t getChangeLevel() const;
private:
  void changed();

  };

#endif
