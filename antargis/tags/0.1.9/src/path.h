#ifndef PATH_H
#define PATH_H

#include "height_map.h"

/**
   This file is WORK-IN-PROGRESS !!!!!!!!!!!!!!!!!!!!!!

   The purpose is some path-finding-algorithm based upon "A-star". (A*)
   
   Because path-finding is a somewhat hard topic, we should try some
   hierarchical approach:
   * build some graph out of the height-map - weight come from height and terrain-type (swamp and deep sand vs. plain grass)
   * nodes that are near each other get eliminated (thrown together), if terrain connection is possible (and too much longer)
   * several levels of this graph can be used, according to the distance that has to be gone
   * for people that have boats, water can be crossed, so there has to be a different path-graph !

*/


class PathWeighter:public AGRubyObject
{
 public:
  virtual ~PathWeighter();
  virtual float weight(float h0,float h1);
};

class MapPathWeighter:public PathWeighter
{
 public:
  MapPathWeighter(HeightMap *pMap);
  virtual float weight(const AGVector2 &a,const AGVector2 &b);
 private:
  HeightMap *mMap;
};


class SimpleGraph
{
 public:
  struct Node;
  struct Edge;
  struct Edge
  {
    Node *a,*b;
    float w;
  };

  struct Node
  {
    AGVector2 p;
    std::list<Edge*> edges;
  };

  void addNode(const AGVector2 &p);
  void addEdge(const AGVector2 &a,const AGVector2 &b,float w0,float w1);

  ~SimpleGraph();

 private:

  Node *findNode(const AGVector2 &p);

  // FIXME: quadtree out of nodes !!!

  std::list<Node*> mNodes;
  std::list<Edge*> mEdges;
  
};



class Pathfinder:public AGRubyObject
{
 public:
  Pathfinder(HeightMap *pMap,PathWeighter *pWeighter);

  virtual void mark();

  std::vector<AGVector2> computePath(const AGVector2 &from, const AGVector2 &to);

 private:
  HeightMap *mMap;
  PathWeighter *mWeighter;
};

#endif
