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
  virtual bool accessible(const AGVector2 &a);
 private:
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

    Edge(Node *pa,Node *pb,float p0,float p1)
    {
      if(pa<pb)
	{
	  a=pa;
	  b=pb;
	  w0=p0;
	  w1=p1;
	}
      else
	{
	  a=pb;
	  b=pa;
	  w0=p1;
	  w1=p0;
	}
    }
    ~Edge();

    bool operator<(const Edge &e) const
    {
      return a<e.a || (a==e.a && b<e.b);
    }

    float maxWeight() const
    {
      return std::max(w0,w1);
    }

    HalfEdge *getHalfEdgeFrom(Node *n)
    {
      HalfEdge *h;
      if(a==n)
	{
	  h->a=a;
	  h->b=b;
	  h->w=w0;
	}
      else
	{
	  h->b=a;
	  h->a=b;
	  h->w=w1;
	}
      return h;
    }
    HalfEdge *getHalfEdgeTo(Node *n)
    {
      HalfEdge *h;
      if(a!=n)
	{
	  h->a=a;
	  h->b=b;
	  h->w=w0;
	}
      else
	{
	  h->b=a;
	  h->a=b;
	  h->w=w1;
	}
      return h;
    }
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

 protected:

  Node *findNode(const AGVector2 &p);

  // FIXME: quadtree out of nodes !!!

  typedef std::map<AGVector2,Node*,AGVector2Sort> NodeMap;
  typedef std::set<Edge*,EdgeSort> EdgeSet;
  NodeMap mNodes;
  EdgeSet mEdges;
  
};


SimpleGraph *makeGraph(HeightMap *pMap, MapPathWeighter *pWeighter);

class DecimatedGraph:public SimpleGraph
{
 public:
  DecimatedGraph()
    {
    }
  DecimatedGraph(const SimpleGraph &g):SimpleGraph(g)
    {
    }
  void decimate(float amount);
 private:
  void tryRemove(Edge *e);
  void collapseEdge(Edge *e);
};



class Pathfinder:public AGRubyObject
{
 public:
  Pathfinder(SimpleGraph *pGraph);

  virtual void mark();

  std::vector<AGVector2> computePath(const AGVector2 &from, const AGVector2 &to);

 private:
  SimpleGraph *mGraph;
};

#endif
