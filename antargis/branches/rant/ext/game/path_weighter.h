#ifndef PATH_WEIGHTER_H
#define PATH_WEIGHTER_H

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include <path_base.h>

#include <map>

class HeightMap;

/**
 * Computes distances from a height field
 */
class AGEXPORT DistanceComputer
{
 public:
  DistanceComputer(HeightMap *pHeightMap,float pStepX=1,float pStepY=1);

  float beginX() const;
  float beginY() const;
  float stepX() const;
  float stepY() const;
  float endX() const;
  float endY() const;

  float width() const;
  float height() const;

  std::map<AGVector2,float> getAllReachableFrom(const AGVector2 &p) const;

  // compute weight for to neighboring points (from a to b)
  float simpleWeight(const AGVector2 &a,const AGVector2 &b) const;

  bool isPassable(const AGVector2 &pPoint) const;

  std::list<AGVector2> getNeighbors(const AGVector2 &p) const;


  std::list<AGVector2> getAllPassable() const;

 private:

  HeightMap *mHeightMap;
  float mStepX,mStepY;
};


#endif
