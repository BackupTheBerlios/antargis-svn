#include "path.h"
#include <math.h>
#include <stdexcept>
#include <ag_profiler.h>
#include <ag_debug.h>
//#include <SDL.h>

SimpleGraph::Edge::~Edge()
{
  //  CTRACE;
}
SimpleGraph::Node::~Node()
{
  //  CTRACE;
}

std::map<SimpleGraph::Node*,float> SimpleGraph::Node::getNextNodes()
{
  std::map<SimpleGraph::Node*,float> m;

  for(Edges::iterator i=edges.begin();i!=edges.end();i++)
    {
      if((*i)->a==this)
	m[(*i)->b]=(*i)->w0;
      else
	m[(*i)->a]=(*i)->w1;
    }

  return m;
}


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
  return complexWeight(a,b);



  float w=(a-b).length();
  if(w>1)
    throw std::runtime_error("not implemented");
  float h0=mMap->getHeight(a[0],a[1]);
  float h1=mMap->getHeight(b[0],b[1]);

  w+=weightHeight(h0,h1);

  return w;
}

float MapPathWeighter::complexWeight(const AGVector2 &a,const AGVector2 &b)
{
  // simply measure all "1 units"
  AGVector2 delta=b-a;
  float dist=delta.length();
  AGVector2 dir=delta/dist;

  float weight=0;
  float stepping=0.1;


  float i=std::min(stepping,dist);
  float oi=0;


  while(oi!=i)
    {
      float d=i-oi;
      AGVector2 ca=a+dir*oi;
      AGVector2 cb=a+dir*i;

      float h0=mMap->getHeight(ca[0],ca[1]);
      float h1=mMap->getHeight(cb[0],cb[1]);
      float cw=d+weightHeight(h0,h1);

      weight+=cw;

      oi=i;
      i=std::min(i+stepping,dist);
    }
  return weight;
}

float MapPathWeighter::weightHeight(float a,float b) const
{
  float f=1; // some factor - to be estimated

  if(std::min(a,b)<0.5)
    return 10;

  if(std::min(a,b)<0)
    return 1000;

  if(b>a)
    {
      // uphill
      return (b-a)*f;
    }
  else
    return -(b-a)*f*0.3; 
}




bool MapPathWeighter::accessible(const AGVector2 &a)
{
  //  return true;
  return mMap->getHeight(a[0],a[1])>0;
}



///////////////////////////////////////////////////////////////////////
// Graph
///////////////////////////////////////////////////////////////////////



SimpleGraph::Node *SimpleGraph::addNode(const AGVector2 &p)
{
  Node *n=findNode(p);
  if(!n)
    {
      n=new Node;
      n->p=p;
      mNodes[p]=n;
    }
  return n;
}
SimpleGraph::Edge *SimpleGraph::addEdge(Node *a, Node *b,float w0,float w1)
{
  Node *n1=a;
  Node *n2=b;
  assert(n1);
  assert(n2);
  Edge *e=new Edge(n1,n2,w0,w1);

  n1->edges.push_back(e);

  n2->edges.push_back(e);

  mEdges.insert(e);

  return e;
}

void SimpleGraph::removeEdge(Edge *e)
{
  //  cdebug("1");
  mEdges.erase(e);
  //  cdebug("2");

  e->a->edges.remove(e);
  e->b->edges.remove(e);
  //  cdebug("3");

  //  delete e;
}

void SimpleGraph::removeNode(Node *n)
{
  //  CTRACE;
  //  cdebug("nodes-size:"<<mNodes.size());
  mNodes.erase(n->p);
  //  cdebug("nodes-size:"<<mNodes.size());
}



SimpleGraph::~SimpleGraph()
{
  for(NodeMap::iterator i=mNodes.begin();i!=mNodes.end();i++)
    delete i->second;
  for(EdgeSet::iterator i=mEdges.begin();i!=mEdges.end();i++)
    delete *i;
}

SimpleGraph::Node *SimpleGraph::findNearest(const AGVector2 &p)
{
  if(mNodes.size()==0)
    return 0;

  NodeMap::iterator i=mNodes.begin();
  Node *n=i->second;
  float dist=(n->p-p).length();
  for(;i!=mNodes.end();i++)
    {
      float d=(i->second->p-p).length();
      if(d<dist)
	{
	  n=i->second;
	  dist=d;
	}
    }
  return n;
}

SimpleGraph::Node *SimpleGraph::findNode(const AGVector2 &p)
{
  return mNodes[p];
}

void SimpleGraph::print()
{
  for(EdgeSet::iterator i=mEdges.begin();i!=mEdges.end();i++)
    std::cout<<(*i)->a->p<<":"<<(*i)->b->p<<":"<<(*i)->w0<<":"<<(*i)->w1<<std::endl;
}

void SimpleGraph::printInfo()
{
  std::cout<<"nodes:"<<mNodes.size()<<std::endl;
  std::cout<<"edges:"<<mEdges.size()<<std::endl;
}

void SimpleGraph::check()
{
  // check if all nodes have an edge
  for(NodeMap::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      if(i->second->edges.size()==0)
	{
	  cdebug("empty node:"<<i->second->p<<"("<<i->second);
	  throw std::runtime_error("check failed 0");
	}
      for(Edges::iterator j=i->second->edges.begin();j!=i->second->edges.end();j++)
	{
	  if((*j)->a!=i->second && (*j)->b!=i->second)
	    throw std::runtime_error("check failed 1");
	  if(mEdges.find(*j)==mEdges.end())
	    throw std::runtime_error("check failed 2");
	}
    }
}


///////////////////////////////////////////////////////////////////////
// DecimatedGraph
///////////////////////////////////////////////////////////////////////

class SortEdges
{
public:
  bool operator()(const SimpleGraph::Edge*e1,const SimpleGraph::Edge*e2)
  {
    return e1->maxWeight()<e2->maxWeight();
  }
};

void DecimatedGraph::decimate(float amount,MapPathWeighter *pWeighter)
{
  size_t m=mNodes.size()*(1.0-amount);

  m=std::max(m,2U);

  while(mNodes.size()>m)
    {
      Edge *s=*mEdges.begin();

      tryRemove(*mEdges.begin(),pWeighter);
    }

}

void DecimatedGraph::tryRemove(Edge *e,MapPathWeighter *pWeighter)
{
  collapseEdge(e,pWeighter);
}

//   w0->  w0-> w0->
// X  --  a -- b -- Y
//   w0<-  w0-> w0<-
//


void DecimatedGraph::collapseEdge(Edge *e,MapPathWeighter *pWeighter)
{
  if(mNodes.size()<=2)
    return;
  

  AGVector2 np=(e->a->p+e->b->p)*0.5;
  //  e->a->p=np;

  Node *nn=addNode(np);

  float d0=e->w0/2;
  float d1=e->w1/2;

  //  cdebug("collapsing:"<<e->a->p<<"  "<<e->b->p);

  //  cdebug(e->w0<<":"<<e->w1);

  Edges all;
  std::copy(e->a->edges.begin(),e->a->edges.end(),std::back_inserter(all));
  std::copy(e->b->edges.begin(),e->b->edges.end(),std::back_inserter(all));

  //  cdebug("all:"<<all.size());

  std::set<Edge> nedges; // collect edges and sort out doubles

  for(Edges::iterator i=all.begin();i!=all.end();i++)
    {
      if(*i!=e)
	{
	  if((*i)->a==e->a)
	    {
	      // X <- a -> b
	      nedges.insert(makeEdge((*i)->b,nn,pWeighter));
	      //	      nedges.insert(Edge((*i)->b,nn,(*i)->w1+e->w0/2, (*i)->w0+e->w1/2));
	    }
	  else if((*i)->b==e->a)
	    {
	      // X -> a -> b
	      nedges.insert(makeEdge((*i)->a,nn,pWeighter));
	      //	      nedges.insert(Edge((*i)->a,nn,(*i)->w0+e->w0/2, (*i)->w1+e->w1/2));
	    }
	  else if((*i)->a==e->b)
	    {
	      // a -> b -> X
	      //nedges.insert(Edge(nn,(*i)->b,e->w0/2+(*i)->w0, e->w1/2+(*i)->w1));
	      nedges.insert(makeEdge(nn,(*i)->b,pWeighter));
	    }
	  else if((*i)->b==e->b)
	    {
	      // a -> b <- X
	      //nedges.insert(Edge(nn,(*i)->a, e->w0/2+(*i)->w1, e->w1/2+(*i)->w0));
	      nedges.insert(makeEdge(nn,(*i)->a,pWeighter));
	    }
	}
    }

  for(std::set<Edge>::iterator i=nedges.begin();i!=nedges.end();i++)
    addEdge(i->a,i->b,i->w0,i->w1);
  

  //  cdebug("ok");
  //  print();
  //  cdebug("removing:"<<e->b->p<<"("<<e->b);
  removeNode(e->b);
  //  cdebug("removing:"<<e->a->p<<"("<<e->a);
  removeNode(e->a);

  for(Edges::iterator i=all.begin();i!=all.end();i++)
    removeEdge(*i);

  //  cdebug("ok........");
  
}


SimpleGraph::Edge DecimatedGraph::makeEdge(Node *a,Node *b,MapPathWeighter *pWeighter)
{
  return Edge(a,b,pWeighter->weight(a->p,b->p),pWeighter->weight(b->p,a->p));
}



///////////////////////////////////////////////////////////////////////
// Graph generation
///////////////////////////////////////////////////////////////////////

SimpleGraph *makeGraph(HeightMap *pMap, MapPathWeighter *pWeighter)
{
  size_t x,y;
  SimpleGraph *graph=new SimpleGraph;
  std::map<std::pair<size_t,size_t>,SimpleGraph::Node*> nodes;

  size_t w=pMap->getW();
  size_t h=pMap->getH();


  //  w=h=4;

  for(x=0;x<w;x++)
    for(y=0;y<h;y++)
      {
	if(pWeighter->accessible(AGVector2(x,y)))
	  nodes[std::make_pair(x,y)]=graph->addNode(AGVector2(x,y));
      }

  for(x=0;x<w;x++)
    for(y=0;y<h;y++)
      {
	std::pair<size_t,size_t> p(x,y);
	std::pair<size_t,size_t> p1(x+1,y);
	std::pair<size_t,size_t> p2(x,y+1);
	if(x<w-1)
	  {
	    SimpleGraph::Node *a=nodes[p];
	    SimpleGraph::Node *b=nodes[p1];
	    if(a && b)
	      graph->addEdge(a,b,pWeighter->weight(AGVector2(x,y),AGVector2(x+1,y)),pWeighter->weight(AGVector2(x+1,y),AGVector2(x,y)));
	  }
	if(y<h-1)
	  {
	    SimpleGraph::Node *a=nodes[p];
	    SimpleGraph::Node *b=nodes[p2];
	    if(a && b)
	      graph->addEdge(a,b,pWeighter->weight(AGVector2(x,y),AGVector2(x,y+1)),pWeighter->weight(AGVector2(x,y+1),AGVector2(x,y)));
	  }
      }
  return graph;

}


///////////////////////////////////////////////////////////////////////
// Pathfinder
///////////////////////////////////////////////////////////////////////


Pathfinder::Pathfinder(SimpleGraph *pGraph):
  mGraph(pGraph)
{
}

void Pathfinder::mark()
{
  markObject(mGraph);
}

struct Path;

struct Heuristic
{
  AGVector2 to;
public:
  Heuristic(const AGVector2 &pTo)
  {
    to=pTo;
  }
  virtual ~Heuristic(){}

  virtual float distance(const AGVector2 &from)
  {
    return (to-from).length()*4;
  }
  bool operator()(const Path &a,const Path &b);
};


struct Path:public std::list<SimpleGraph::Node*>
{
  float weight;

  Path()
  {
    weight=0;
  }

  float getWeight(Heuristic *h) const
  {
    if(size()>0)
      return weight+h->distance(back()->p);
    return -1;
  }

  void push(SimpleGraph::Node *n,float w)
  {
    push_back(n);
    weight+=w;
  }
};

// Heuristic
bool Heuristic::operator()(const Path &a,const Path &b)
{
  return a.getWeight(this)<b.getWeight(this);
}



std::vector<AGVector2> Pathfinder::computePath(const AGVector2 &pFrom, const AGVector2 &pTo)
{
  STACKTRACE;

  Uint32 t0=SDL_GetTicks();
  
  SimpleGraph::Node *from=mGraph->findNearest(pFrom);
  SimpleGraph::Node *to=mGraph->findNearest(pTo);

  Path initial;
  initial.push(from,0);

  Heuristic heuristic(to->p);
  std::set<Path,Heuristic> pathSet(heuristic);
  std::vector<AGVector2> result;

  pathSet.insert(initial);

  size_t tries=0;
  /*
  cdebug("pfrom:"<<pFrom);
  cdebug("pto:"<<pTo);

  cdebug("from:"<<from->p);
  cdebug("to:"<<to->p);
  */
  //  SDL_Delay(3000);

  while(tries<1000 && pathSet.size()>0)
    {
      Path path=*pathSet.begin();
      pathSet.erase(pathSet.begin());
      SimpleGraph::Node *last=path.back();
      if(last==to)
	{
	  Uint32 t1=SDL_GetTicks();
	  std::cout<<"TIME:"<<t1-t0<<std::endl;
	  cdebug("time:"<<t1-t0);
	  cdebug("len:"<<path.size());
	  cdebug("tries:"<<tries);
	  cdebug("PATHHHHHHHHHHHHHHHHHHHH:");
	  for(Path::iterator i=path.begin();i!=path.end();i++)
	    {
	      cdebug((*i)->p);
	      result.push_back((*i)->p);
	    }

	  return result; // ready
	}
      
      /*
      cdebug("heuristic:"<<path.getWeight(&heuristic));
      cdebug("gone:"<<path.weight);
      cdebug("setsize:"<<pathSet.size());
      cdebug("rest:"<<(last->p-to->p).length()<<"  p:"<<last->p);
      cdebug("tries:"<<tries);
      */

      std::map<SimpleGraph::Node*,float> nextNodes=last->getNextNodes();
      for(std::map<SimpleGraph::Node*,float>::iterator i=nextNodes.begin();i!=nextNodes.end();i++)
	{
	  Path npath=path;
	  npath.push(i->first,i->second);
	  pathSet.insert(npath);
	}


      tries++;
    }


  return result;

}

std::vector<AGVector2> Pathfinder::refinePath(const std::vector<AGVector2> &p,MapPathWeighter *pWeighter)
{
  std::vector<AGVector2> result=p;
  
  if(result.size()<3)
    return result;

  std::vector<AGVector2>::iterator i=result.begin()+1;

  for(;i!=result.end()-1;)
    {
      // check if waypoint is discardable
      AGVector2 a=*(i-1);
      AGVector2 b=*i;
      AGVector2 c=*(i+1);

      float w0=pWeighter->weight(a,b);
      float w1=pWeighter->weight(b,c);
      float w2=pWeighter->weight(a,c);

      cdebug("WEIGHTS:"<<w0<<"  "<<w1<<"  "<<w2);

      if(w0+w1>w2)
	i=result.erase(i);
      else
	i++;
    }


  return result;
}
