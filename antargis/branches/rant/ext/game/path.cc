#include "path.h"
#include <math.h>
#include <stdexcept>
#include <ag_profiler.h>
#include <ag_debug.h>
//#include <SDL.h>
#include <ag_surface.h>
#include <ag_painter.h>



///////////////////////////////////////////////////////////////////////
// Node
///////////////////////////////////////////////////////////////////////


SimpleGraph::Node::~Node()
{

}

bool SimpleGraph::NodePtrCompare::operator()(const Node *a,const Node *b)
{
  return a->p<b->p;
}


SimpleGraph::Node::NodeMap SimpleGraph::Node::getNextNodes()
{
  SimpleGraph::Node::NodeMap m;

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
// Edge
///////////////////////////////////////////////////////////////////////

SimpleGraph::Edge::Edge(Node *pa,Node *pb,float p0,float p1)
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
SimpleGraph::Edge::~Edge()
{
}

bool SimpleGraph::EdgePtrCompare::operator()(const Edge *a,const Edge *b)
{
  return(a->a->p < b->a->p || (a->a->p==b->a->p && a->b->p<b->b->p));
}


bool SimpleGraph::Edge::operator<(const Edge &e) const
{
  return a<e.a || (a==e.a && b<e.b);
}

float SimpleGraph::Edge::maxWeight() const
{
  return std::max(w0,w1);
}

SimpleGraph::Node *SimpleGraph::Edge::getOther(Node *n)
{
  if(a==n)
    return b;
  if(b==n)
    return a;
  return 0;
}

/*
SimpleGraph::HalfEdge *SimpleGraph::Edge::getHalfEdgeFrom(Node *n)
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
SimpleGraph::HalfEdge *SimpleGraph::Edge::getHalfEdgeTo(Node *n)
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
  }*/


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


  if(b<0.2)
    return (0.2-b)*1000;

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
  float hmin=10;
  for(int x=-1;x<2;x++)
    for(int y=-1;y<2;y++)
      {
	float px=a[0]+x;
	float py=a[1]+y;
	px=std::max(0.0f,std::min(px,mMap->getW()-1.0f));
	py=std::max(0.0f,std::min(py,mMap->getH()-1.0f));
	hmin=std::min(hmin,mMap->getHeight(px,py));
      }
  return hmin>0;
}



///////////////////////////////////////////////////////////////////////
// Graph
///////////////////////////////////////////////////////////////////////

SimpleGraph::SimpleGraph()
{
  CTRACE;
  mWidth=0.00001;
}

SimpleGraph::SimpleGraph(const SimpleGraph &g)
{
  mWidth=g.mWidth;

  std::map<Node*,Node*> nodemap;
  for(NodeMap::const_iterator i=g.mNodeMap.begin();i!=g.mNodeMap.end();i++)
    {
      nodemap[i->second]=addNode(i->first);
    }
  for(EdgeSet::const_iterator i=g.mEdges.begin();i!=g.mEdges.end();i++)
    addEdge(nodemap[(*i)->a],nodemap[(*i)->b],(*i)->w0,(*i)->w1);
}



SimpleGraph::Node *SimpleGraph::addNode(const AGVector2 &p)
{
  Node *n=findNode(p);
  if(!n)
    {
      n=new Node;
      n->p=p;
      mNodes.insert(n);
      mNodeMap[p]=n;
    }

  mWidth=std::max((double)mWidth,std::max(fabs(p[0]),fabs(p[1])));

  return n;
}

void SimpleGraph::paintNode(const AGRect2& r,AGPaintTarget &t,const AGVector2 &p,const AGColor &c)
{
  AGPainter painter(t);
  float xscale=r.width()/width();
  float yscale=r.height()/width();


  float x=p[0]*xscale+r.x();
  float y=p[1]*yscale+r.y();
  
  painter.fillRect(AGRect2(x,y,10,10),c);
  
}


void SimpleGraph::paint(const AGRect2& r,AGPaintTarget &t,Heuristic &heuristic)
{
  AGColor green(0,0xFF,0,0xFF);
  AGColor red(0xFF,0,0,0xFF);
  float xscale=r.width()/width();
  float yscale=r.height()/width();
  AGPainter painter(t);

  for(EdgeSet::iterator i=mEdges.begin();i!=mEdges.end();i++)
    {
      AGVector2 p0=(*i)->a->p;
      float x0=p0[0]*xscale+r.x();
      float y0=p0[1]*yscale+r.y();
      AGVector2 p1=(*i)->b->p;
      float x1=p1[0]*xscale+r.x();
      float y1=p1[1]*yscale+r.y();

      painter.drawLine(AGVector2(x0,y0),AGVector2(x1,y1),red);

      AGVector2 m((x0+x1)/2-10,(y0+y1)/2-10);

      painter.renderText(AGStringUtf8((int)(*i)->w0)+":"+AGStringUtf8((int)(*i)->w1),m,AGFont());
      
    }


  for(NodeSet::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      AGVector2 p=(*i)->p;
      float x=p[0]*xscale+r.x();
      float y=p[1]*yscale+r.y();

      painter.fillRect(AGRect2(x,y,2,2),green);

      painter.renderText(AGStringUtf8(int(heuristic.distance((*i)->p))),AGVector2(x-10,y-10),AGFont());

    }
}



float SimpleGraph::width() const
{
  return mWidth;
}

size_t SimpleGraph::size() const
{
  return mNodes.size();
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
  mNodes.erase(n);
  mNodeMap.erase(n->p);
}



SimpleGraph::~SimpleGraph()
{
  CTRACE;
  for(NodeSet::iterator i=mNodes.begin();i!=mNodes.end();i++)
    delete *i;
  for(EdgeSet::iterator i=mEdges.begin();i!=mEdges.end();i++)
    delete *i;
}

SimpleGraph::Node *SimpleGraph::findNearest(const AGVector2 &p)
{
  if(mNodes.size()==0)
    return 0;

  NodeSet::iterator i=mNodes.begin();
  Node *n=*i;
  float dist=(n->p-p).length();
  for(;i!=mNodes.end();i++)
    {
      float d=((*i)->p-p).length();
      if(d<dist)
	{
	  n=*i;
	  dist=d;
	}
    }
  return n;
}

AGVector2 SimpleGraph::findNearestVector(const AGVector2 &p)
{
  return findNearest(p)->p;
}



SimpleGraph::Node *SimpleGraph::findNode(const AGVector2 &p)
{
  return mNodeMap[p];
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
  for(NodeSet::iterator i=mNodes.begin();i!=mNodes.end();i++)
    {
      if((*i)->edges.size()==0)
	{
	  cdebug("empty node:"<<(*i)->p<<"("<<(*i));
	  throw std::runtime_error("check failed 0");
	}
      for(Edges::iterator j=(*i)->edges.begin();j!=(*i)->edges.end();j++)
	{
	  if((*j)->a!=(*i) && (*j)->b!=(*i))
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


DecimatedGraph::DecimatedGraph()
{
  CTRACE;
}

DecimatedGraph::DecimatedGraph(const SimpleGraph &g):SimpleGraph(g)
{
  CTRACE;
  cdebug("old:"<<(&g));
}

DecimatedGraph::~DecimatedGraph()
{
  CTRACE;
}



void DecimatedGraph::decimate(float amount,MapPathWeighter *pWeighter)
{
  size_t m=(size_t)(mNodes.size()*(1.0-amount));

  m=std::max((unsigned int)m,2U);

  while(mNodes.size()>m)
    {
      cdebug(mNodes.size()<<" vs. "<<m);

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

SimpleGraph *makeGraph(HeightMap *pMap, MapPathWeighter *pWeighter,size_t res)
{
  size_t x,y;
  SimpleGraph *graph=new SimpleGraph;
  std::map<std::pair<size_t,size_t>,SimpleGraph::Node*> nodes;

  size_t w=pMap->getW();
  size_t h=pMap->getH();


  //  w=h=4;

  for(x=0;x<w;x+=res)
    for(y=0;y<h;y+=res)
      {
	if(pWeighter->accessible(AGVector2(x,y)))
	  nodes[std::make_pair(x,y)]=graph->addNode(AGVector2(x,y));
      }

  for(x=0;x<w;x+=res)
    for(y=0;y<h;y+=res)
      {
	std::pair<size_t,size_t> p(x,y);
	std::pair<size_t,size_t> p1(x+res,y);
	std::pair<size_t,size_t> p2(x,y+res);
	if(x<w-1)
	  {
	    SimpleGraph::Node *a=nodes[p];
	    SimpleGraph::Node *b=nodes[p1];
	    if(a && b)
	      graph->addEdge(a,b,pWeighter->weight(AGVector2(x,y),AGVector2(x+res,y)),pWeighter->weight(AGVector2(x+res,y),AGVector2(x,y)));
	  }
	if(y<h-1)
	  {
	    SimpleGraph::Node *a=nodes[p];
	    SimpleGraph::Node *b=nodes[p2];
	    if(a && b)
	      graph->addEdge(a,b,pWeighter->weight(AGVector2(x,y),AGVector2(x,y+res)),pWeighter->weight(AGVector2(x,y+res),AGVector2(x,y)));
	  }
      }
  return graph;

}

HeuristicFunction *computeHeuristic(SimpleGraph *g)
{
  StoredHeuristicFunction *h=new StoredHeuristicFunction(32,g->width());

  size_t c=0;

  AGSurface ms(32,32);


  //  SimpleGraph::NodeSet testSet;
  //  testSet.insert(g->findNearest(AGVector2(176,167)));



  //?????????????????ß

  for(SimpleGraph::NodeSet::iterator i=g->mNodes.begin();i!=g->mNodes.end();i++)
    //for(SimpleGraph::NodeSet::iterator i=testSet.begin();i!=testSet.end();i++)
    {
      std::map<SimpleGraph::Node*,float> weights;
      std::set<SimpleGraph::Node*> modified;
      modified.insert(*i);
      while(modified.size())
	{
	  SimpleGraph::Node *n=*modified.begin();
	  modified.erase(modified.begin());
	  
	  SimpleGraph::Node::NodeMap ns=n->getNextNodes();
	  float old=weights[n];

	  for(SimpleGraph::Node::NodeMap::iterator j=ns.begin();j!=ns.end();j++)
	    {
	      float now=weights[j->first];
	      //	      cdebug("now:"<<now<<"  old:"<<old<<" plus:"<<j->second);
	      if(now==0 || (now>j->second+old))
		{
		  modified.insert(j->first);
		  weights[j->first]=j->second+old;
		  //		  cdebug("saved:"<<weights[j->first]);

		  if(false)
		  {
		    int x=(int)j->first->p[0];
		    int y=(int)j->first->p[1];
		    //		    cdebug("w:"<<g->width());

		    x=(int)(x*32/g->width());
		    y=(int)(y*32/g->width());
		    int c=(int)weights[j->first]*4;
		    int c2=(int)weights[j->first];
		    AGColor mc(c,c2,c2,255);
		    //		    cdebug(x<<";"<<y<<":"<<mc);

		    ms.putPixel(x,y,mc);

		  }




		}
	    }
	}

      //FIXME: store data !!

      for(std::map<SimpleGraph::Node*,float>::iterator j=weights.begin();j!=weights.end();j++)
	{
	  h->store(std::make_pair((*i)->p,j->first->p),j->second);
	}

      h->store(std::make_pair((*i)->p,(*i)->p),0);

      //      ms.save("path.png");
      //      exit(1);


      c++;
      cdebug(c<<" out of "<<g->mNodes.size());
    }

  //  h->display();

  return h;
}

///////////////////////////////////////////////////////////////////////
// Path
///////////////////////////////////////////////////////////////////////


Path::Path()
{
  weight=0;
}

float Path::getWeight(Heuristic *h) const
{
  if(size()>0)
    return weight+h->distance(back()->p);
  return -1;
}

void Path::push(SimpleGraph::Node *n,float w)
{
  push_back(n);
  weight+=w;
}

void Path::paint(const AGRect2 &r,AGPaintTarget &t,float scale)
{
  AGVector2 old;
  AGColor green(0,0xFF,0,0xFF);
  AGColor red(0xFF,0,0,0xFF);
  AGPainter painter(t);

  for(iterator i=begin();i!=end();i++)
    {
      AGVector2 current((*i)->p[0]*r.width()/scale+r.x0(),
			(*i)->p[1]*r.height()/scale+r.y0());
      if(i!=begin())
	{
	  painter.drawLine(old,current,green);
	}
      painter.fillRect(AGRect2(current[0],current[1],3,3),red);
      old=current;
    }
  painter.renderText(AGStringUtf8(weight),AGVector2(0,0),AGFont());
  painter.renderText(AGStringUtf8(size()),AGVector2(0,50),AGFont());
  

}

///////////////////////////////////////////////////////////////////////
// Pathfinder
///////////////////////////////////////////////////////////////////////


Pathfinder::Pathfinder(SimpleGraph *pGraph,HeuristicFunction *pHeuristic,PathDebugging *d):
  mGraph(pGraph),mHeuristic(pHeuristic),
  mDebug(d)
{
  CTRACE;
  cdebug("Simplegraph:"<<mGraph);
}

void Pathfinder::mark()
{
  markObject(mGraph);
}

struct Path;



// Heuristic
bool Heuristic::operator()(const Path &a,const Path &b)
{
  return a.getWeight(this)<b.getWeight(this);
}

float Heuristic::distance(const AGVector2&from)
{
  //  cdebug("from:"<<from<<" to:"<<to);
  if((to-AGVector2(90,90)).length()<1)
    throw int();
  return (*p)(std::make_pair(from,to));
}



std::vector<AGVector2> Pathfinder::computePath(const AGVector2 &pFrom, const AGVector2 &pTo)
{
  STACKTRACE;

  Uint32 t0=SDL_GetTicks();
  
  SimpleGraph::Node *from=mGraph->findNearest(pFrom);
  SimpleGraph::Node *to=mGraph->findNearest(pTo);


  cdebug("FROM:"<<from->p);
  cdebug("TO:"<<to->p);

  Path initial;
  initial.push(from,0);

  //  Heuristic heuristic(to->p);
  std::set<Path,Heuristic> pathSet(Heuristic(to->p,mHeuristic));
  std::vector<AGVector2> result;

  pathSet.insert(initial);

  size_t tries=0;

  while(tries<1000 && pathSet.size()>0)
    {
      Path path=*pathSet.begin();

      if(mDebug)
	mDebug->debugPath(path,path.getWeight(new Heuristic(to->p,mHeuristic)));

      {
	SimpleGraph::Node *oldi=0;
	  for(Path::iterator i=path.begin();i!=path.end();i++)
	    {
	      /*
	      std::cout<<(*i)->p<<":";
	      if(oldi)
		{
		  for(SimpleGraph::Edges::iterator j=oldi->edges.begin();j!=oldi->edges.end();j++)
		    {
		      if((*j)->a==*i)
			std::cout<<"("<<(*j)->w0<<")";
		      else if((*j)->b==*i)
			std::cout<<"("<<(*j)->w1<<")";
		    }
		}
	      */
	      oldi=*i;
	    }
	std::cout<<std::endl;
      }

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
	  cdebug(path.weight);
	  for(Path::iterator i=path.begin();i!=path.end();i++)
	    {
	      cdebug((*i)->p);
	      result.push_back((*i)->p);
	    }

	  return result; // ready
	}
      
      
      cdebug("heuristic:"<<path.getWeight(new Heuristic(to->p,mHeuristic)));
      cdebug("gone:"<<path.weight);
      cdebug("setsize:"<<pathSet.size());
      cdebug("rest:"<<(last->p-to->p).length()<<"  p:"<<last->p);
      cdebug("tries:"<<tries);
      

      SimpleGraph::Node::NodeMap nextNodes=last->getNextNodes();

      std::set<SimpleGraph::Node*> alreadyGone;
      std::copy(path.begin(),path.end(),std::inserter(alreadyGone,alreadyGone.begin()));

      for(SimpleGraph::Node::NodeMap::iterator i=nextNodes.begin();i!=nextNodes.end();i++)
	{
	  if(alreadyGone.find(i->first)==alreadyGone.end())
	    {
	      cdebug("possible:"<<i->first<<"   "<<i->second);
	      Path npath=path;
	      npath.push(i->first,i->second);
	      pathSet.insert(npath);
	    }
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
