#ifndef PATH_V2_H
#define PATH_V2_H

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include <path_base.h>
#include <ag_algebra.h>

#include <map>

#include "path_weighter.h"

class AGEXPORT PathV3Data:public AGRubyObject
{
 public:
  PathV3Data(size_t pFieldCount=64);


  void compute(const DistanceComputer &pCalc);

  /**
   * \return -1 if they're no neighbors
   *         >0 the distance when they're neighbors somehow
   */
  float directNeighbors(const AGVector2 &a,const AGVector2 &b) const;

  size_t getField(const AGVector2 &a) const;
  std::map<size_t,float> getNeighbors(size_t pField) const;

  bool isInField(const AGVector2 &pPoint,size_t pField) const;

  // pFrom and pTo must be in neighboring fields
  float getDistanceFromTo(const AGVector2 &pFrom,const AGVector2 &pTo) const;
  

  std::list<AGVector2> vecList(size_t field) const;
  size_t fieldCount() const;


 private:
  
  size_t mFieldCount;


  std::map<AGVector2,size_t> mVec2Field;
  std::vector<std::list<AGVector2> > mFields;
  std::map<AGVector2,std::map<AGVector2,float> > mDirectMaps;
  std::map<size_t,std::map<size_t,float> > mDirectFieldMaps;
  
};


class AGEXPORT PathV2Data:public AGRubyObject
{
 public:
  PathV2Data(size_t pBaseSize=8);

  void compute(const DistanceComputer &pCalc);

 private:
  void traceFrom(const DistanceComputer &pCalc,const AGVector2 &p);

  

  std::map<AGVector2,AGMatrixN> mMatrices;
  size_t mBaseSize;
};



class AGEXPORT PathBase:public AGRubyObject
{
};

class AGEXPORT PathV2:public PathBase
{
 public:
  PathV2(PathV2Data *pData);
  ~PathV2();

  virtual float weight(const AGVector2 &a,const AGVector2 &b);

  virtual void mark();
 private:
  
  PathV2Data *mData;
};

#endif
