#ifndef PATH_H
#define PATH_H

#include "height_map.h"
#include "heuristic.h"

#include <ag_surface.h>

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

class Heuristic;

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
  virtual bool accessible(const AGVector2 &a);
 private:

  virtual float complexWeight(const AGVector2 &a,const AGVector2 &b);
  virtual float weightHeight(float a,float b) const;

  HeightMap *mMap;
};


struct AGVector2Sort
{
  public:
  bool operator()(const AGVector2 &a,const AGVector2 &b)
  {
    return a[0]<b[0] || (a[0]==b[0] && a[1]<b[1]);
  }
};



class SimpleGraph:public AGRubyObject
{
 public:
  struct Node;
  struct Edge;

  typedef std::list<Edge*> Edges;
  typedef std::list<Node*> Nodes;

  // only used for processing and not for storing !
  struct HalfEdge
  {
    Node *a,*b;
    float w;
  };
  struct Edge
  {
    Node *a,*b;
    float w0,w1;

    Edge(Node *pa,Node *pb,float p0,float p1);
    ~Edge();

    bool operator<(const Edge &e) const;
    float maxWeight() const;

    Node *getOther(Node *n);

    HalfEdge *getHalfEdgeFrom(Node *n);

    HalfEdge *getHalfEdgeTo(Node *n);
  };

  struct Node
  {
    AGVector2 p;
    Edges edges;
    
    ~Node();

    std::map<Node*,float> getNextNodes();

  };

  struct EdgeSort
  {
    public:
    bool operator()(const Edge*a,const Edge *b)
    {
      float w0=weight(a);
      float w1=weight(b);
      return w0<w1 || (w0==w1 && a<b);
    }
    private:
    float weight(const Edge *a)
    {
      return a->w0+a->w1;
    }
  };

  SimpleGraph();
  

  Node *addNode(const AGVector2 &p);
  /**
     @param w0 is weight of going from a to b
     @param w1 is weight of goinf from b to a
  */
  Edge *addEdge(Node *a,Node *b,float w0,float w1);

  void removeEdge(Edge *e);
  void removeNode(Node *n);
  void print();
  void printInfo();

  ~SimpleGraph();


  void check();
  

  Node *findNearest(const AGVector2 &p);
  AGVector2 findNearestVector(const AGVector2 &p);

  float width() const;

  size_t size() const;

  void paint(const AGRect2& r,AGPaintTarget &t,Heuristic &heuristic);
  void paintNode(const AGRect2& r,AGPaintTarget &t,const AGVector2 &p,const AGColor &c);


 protected:

  Node *findNode(const AGVector2 &p);

  // FIXME: quadtree out of nodes !!!

  typedef std::map<AGVector2,Node*,AGVector2Sort> NodeMap;
  typedef std::set<Node*> NodeSet;
  typedef std::set<Edge*,EdgeSort> EdgeSet;
  NodeMap mNodeMap;
  NodeSet mNodes;
  EdgeSet mEdges;
  float mWidth;


  friend HeuristicFunction *computeHeuristic(SimpleGraph *g);

};

SimpleGraph *makeGraph(HeightMap *pMap, MapPathWeighter *pWeighter,size_t res=1);
HeuristicFunction *computeHeuristic(SimpleGraph *g);


class DecimatedGraph:public SimpleGraph
{
 public:
  DecimatedGraph()
    {
    }
  DecimatedGraph(const SimpleGraph &g):SimpleGraph(g)
    {
    }
  void decimate(float amount,MapPathWeighter *pWeighter);
 private:
  void tryRemove(Edge *e,MapPathWeighter *pWeighter);
  void collapseEdge(Edge *e,MapPathWeighter *pWeighter);

  Edge makeEdge(Node *a,Node *b,MapPathWeighter *pWeighter);

};

struct Path;

class Heuristic
{
  AGVector2 to;
  HeuristicFunction *p;
 public:
  Heuristic(const AGVector2 &pTo,HeuristicFunction *f):to(pTo),p(f)
    {
    }
  bool operator()(const Path &a,const Path &b);
  float distance(const AGVector2&from);
};

struct Path:public std::list<SimpleGraph::Node*>
{
  float weight;
  
  Path();
  
  float getWeight(Heuristic *h) const;
  
  void push(SimpleGraph::Node *n,float w);
  
  void paint(const AGRect2 &r,AGPaintTarget &t,float scale);
  
};


class PathDebugging
{
 public:
  virtual ~PathDebugging()
    {
    }
  virtual void debugPath(Path &p,float heuristic)
  {
  }
};
  

class Pathfinder:public AGRubyObject
{
 public:

  Pathfinder(SimpleGraph *pGraph,HeuristicFunction *pHeuristic,PathDebugging *pDebugger=0);

  virtual void mark();

  std::vector<AGVector2> computePath(const AGVector2 &from, const AGVector2 &to);

  std::vector<AGVector2> refinePath(const std::vector<AGVector2> &p,MapPathWeighter *pWeighter);

 private:
  SimpleGraph *mGraph;
  HeuristicFunction *mHeuristic;
  PathDebugging *mDebug;
};

#endif
