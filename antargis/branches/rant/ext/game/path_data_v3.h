#ifndef PATH_DATA_V3_H
#define PATH_DATA_V3_H

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include <ag_geometry.h>

#include "path_weighter.h"

#include <vector>
#include <map>
#include <set>
#include <ag_rubyobj.h>

//namespace PathFinding
//{

  class AGEXPORT Field:public AGRubyObject
  {
  public:
    Field(const AGVector2 &pMiddle,size_t pID);
    virtual ~Field();

    void insert(const AGVector2 &v,DistanceComputer &pDist);

    bool hasVector(const AGVector2 &v) const;
    bool isNeighbor(const AGVector2 &v) const;

    AGVector2 getMiddle() const;

    std::vector<AGVector2> getVectors() const;
    std::vector<AGVector2> getNeighbors() const;

  private:
    size_t mID;
  protected:
    std::set<AGVector2> mVectors;

    std::set<AGVector2> mNeighbors;
  private:
    AGVector2 mMiddle;
  };

  class AGEXPORT FieldCollection:public AGRubyObject
  {
  public:
    virtual ~FieldCollection();

    void assign(const AGVector2 &v,size_t pField,DistanceComputer &pDist);
    size_t newField(const AGVector2 &pMiddle);

    Field *getField(size_t pField);

    size_t getFieldId(const AGVector2 &pVector) const;

    size_t getFieldCount() const;

    void mark();

  protected:
    virtual Field *createField(const AGVector2 &pMiddle,size_t pId);

  private:


    std::vector<Field*> mFields;
    std::map<AGVector2,size_t> mVec2Field;
  };

class AGEXPORT FieldCollectionWithDistances:public FieldCollection
{
 public:
  /*
    Does the following:
    1) save distances from vec a => vec b (in neighboring fields) into fieldwithneighbordistances.neighbordistances
    2) compute this.mDistances from middle to middle
    * 3) save distances from vec a => neighbor middle from there to field x in fieldwithneighbordistances.fielddistance
    * last is not needed necessarily
   */
  void computeDistances(const DistanceComputer &pDist);

  void computeFieldNeighbors();

  std::set<size_t> getNeighborFields(size_t fieldNum);


  protected:
  virtual Field *createField(const AGVector2 &pMiddle,size_t pId);
    
 private:
  std::map<size_t,size_t> mDistances;
  std::map<size_t,std::set<size_t> > mFieldNeighbors;
};


  AGEXPORT void assignFields(FieldCollection &pCollection,DistanceComputer &pDist,size_t pFieldCount);


  class AGEXPORT FieldAssigner
  {
  public:
    
  };

  class AGEXPORT FieldWithDistances:public Field
  {
  public:
    FieldWithDistances(const AGVector2 &pMiddle,size_t pID);


    void initLocalDistances(const DistanceComputer &pComputer,const std::vector<AGVector2> &pVectors);
    
    float getDistance(const AGVector2 &a,const AGVector2 &b);

    
  private:
    std::map<std::pair<AGVector2,AGVector2>,float> mDistances;
  };

class AGEXPORT FieldWithNeighborDistances:public FieldWithDistances
{
 public:
  FieldWithNeighborDistances(const AGVector2 &pMiddle,size_t pID);

  // from my a to b, that is in neighbor field
  void setNeighborDistance(const AGVector2 &a,const AGVector2 &b,float value);
  float getNeighborDistance(const AGVector2 &a,const AGVector2 &b);

  void setFieldDistance(const AGVector2 &a,size_t pField,float value);
  float getFieldDistance(const AGVector2 &a,size_t pField);

 private:
  std::map<std::pair<AGVector2,AGVector2>,float> mNeighborDistances;
  std::map<std::pair<AGVector2,size_t>, float> mFieldDistances;
};

//}

#endif

