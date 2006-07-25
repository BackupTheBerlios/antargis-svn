#ifndef PATH_H
#define PATH_H

#include "height_map.h"

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
