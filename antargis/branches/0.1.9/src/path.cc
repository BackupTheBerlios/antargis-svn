#include "path.h"
#include <math.h>

///////////////////////////////////////////////////////////////////////
// PathWeighter
///////////////////////////////////////////////////////////////////////

PathWeighter::~PathWeighter()
{
}
float PathWeighter::weight(float h0,float h1)
{
  if(h0<0.3 || h1<0.3)
    return 100000; // high weight for non-passable water ;-)

  return fabs(h0-h1);
}

///////////////////////////////////////////////////////////////////////
// MapPathWeighter
///////////////////////////////////////////////////////////////////////

MapPathWeighter::MapPathWeighter(HeightMap *pMap):
  mMap(pMap)
{
}
float MapPathWeighter::weight(const AGVector2 &a,const AGVector2 &b)
{
  throw std::runtime_error("not implemented");

  return 0;
}


///////////////////////////////////////////////////////////////////////
// Graph
///////////////////////////////////////////////////////////////////////



void SimpleGraph::addNode(const AGVector2 &p)
{
  Node *n=findNode(p);
  if(!n)
    {
      n=new Node;
      n->p=p;
      mNodes.push_back(n);
    }
}
void SimpleGraph::addEdge(const AGVector2 &a,const AGVector2 &b,float w0,float w1)
{
  Node *n1=findNode(a);
  Node *n2=findNode(b);
  assert(n1);
  assert(n2);
  Edge *e1=new Edge;
  e1->a=n1;
  e1->b=n2;
  e1->w=w0;

  n1->edges.push_back(e1);

  Edge *e2=new Edge;
  e2->a=n2;
  e2->b=n1;
  e2->w=w1;

  n2->edges.push_back(e2);
}


SimpleGraph::~SimpleGraph()
{
  for(std::list<Node*>::iterator i=mNodes.begin();i!=mNodes.end();i++)
    delete *i;
  for(std::list<Edge*>::iterator i=mEdges.begin();i!=mEdges.end();i++)
    delete *i;
}

SimpleGraph::Node *SimpleGraph::findNode(const AGVector2 &p)
{
  for(std::list<Node*>::iterator i=mNodes.begin();i!=mNodes.end();i++)
    if((*i)->p==p)
      return *i;
  return 0;  
}



///////////////////////////////////////////////////////////////////////
// Pathfinder
///////////////////////////////////////////////////////////////////////


Pathfinder::Pathfinder(HeightMap *pMap,PathWeighter *pWeighter):
  mMap(pMap),
  mWeighter(pWeighter)
{
}

void Pathfinder::mark()
{
  markObject(mMap);
  markObject(mWeighter);
}

