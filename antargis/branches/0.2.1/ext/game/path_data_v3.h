#ifndef PATH_DATA_V3_H
#define PATH_DATA_V3_H

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include <ag_geometry.h>

#include "path_weighter.h"

#include <vector>
#include <map>
#include <set>

namespace PathFinding
{

  class AGEXPORT Field
  {
  public:
    Field(const AGVector2 &pMiddle,size_t pID);

    void insert(const AGVector2 &v,DistanceComputer &pDist);

    bool hasVector(const AGVector2 &v) const;
    bool isNeighbor(const AGVector2 &v) const;

    AGVector2 getMiddle() const;

  private:
    size_t mID;
    std::set<AGVector2> mVectors;

    std::set<AGVector2> mNeighbors;

    AGVector2 mMiddle;
  };

  class AGEXPORT FieldCollection
  {
  public:
    void assign(const AGVector2 &v,size_t pField,DistanceComputer &pDist);
    size_t newField(const AGVector2 &pMiddle);

    Field *getField(size_t pField);

    size_t getFieldId(const AGVector2 &pVector) const;

    size_t getFieldCount() const;

  private:
    std::vector<Field> mFields;
    std::map<AGVector2,size_t> mVec2Field;
  };


AGEXPORT void assignFields(FieldCollection &pCollection,DistanceComputer &pDist,size_t pFieldCount);


  class AGEXPORT FieldAssigner
  {
  public:
    
  };
}

#endif

