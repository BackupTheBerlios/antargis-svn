#include "mesh_optimizer.h"
#include "ag_debug.h"

//////////////////////////////////////////////////////////////////////
// Mesh-optimizing
//////////////////////////////////////////////////////////////////////

// sorting
bool MeshVertex::operator<(const MeshVertex &p) const
{
  int i;
  for(i=0;i<3;i++)
    if(v[i]<p.v[i])
      return true;
  
  for(i=0;i<4;i++)
    if(c[i]<p.c[i])
      return true;
  
  for(i=0;i<3;i++) 
    if(n[i]<p.n[i]) 
      return true; 
  
  for(i=0;i<2;i++)
    if(t[i]<p.t[i]) 
      return true;
  
    return false;
}


MeshOptimizer::MeshOptimizer()
{
  saved=0;
}
void MeshOptimizer::add(const MeshVertex &v)
{
  int index=mVertices.size();
  std::map<MeshVertex,int>::iterator i=mMap.find(v);
  if(i!=mMap.end())
    {
      saved++;
      index=i->second;
    }
  else
    {
      mVertices.push_back(v);
      mMap[v]=index;
    }
  mIndices.push_back(index);
  return;
}


struct MeshTriangle
{
  size_t i0,i1,i2;
  MeshTriangle(size_t p0,size_t p1,size_t p2)
  {
    i0=p0;
    i1=p1;
    i2=p2;
  }

  bool operator<(const MeshTriangle &t) const;
};

struct TriCompare
{
  MeshOptimizer *o;
  AGVector3 c;
  TriCompare(MeshOptimizer *p,AGVector3 cam):o(p),c(cam)
  {
  }
  bool operator()(const MeshTriangle &t1,const MeshTriangle &t2)
  {
    AGVector3 m0=(o->getV(t1.i0).dim3()+o->getV(t1.i1).dim3()+o->getV(t1.i2).dim3())/3.0;
    AGVector3 m1=(o->getV(t2.i0).dim3()+o->getV(t2.i1).dim3()+o->getV(t2.i2).dim3())/3.0;

    return (c-m0).length2()>(c-m1).length2();
  }
};



VertexArray MeshOptimizer::getArray()
{
  VertexArray a;
  for(size_t i=0;i<mIndices.size();i++)
    {
      MeshVertex v=mVertices[i];
      a.addVertex(v.v,v.c,v.n,v.t);
    }


  std::vector<MeshTriangle> tris;
  for(size_t i=0;i<mIndices.size();i+=3)
    {
      tris.push_back(MeshTriangle(mIndices[i],mIndices[i+1],mIndices[i+2]));
    }  
  sort(tris.begin(),tris.end(),TriCompare(this,AGVector3(0,-10,10)));

  /*
  for(size_t i=0;i<mIndices.size();i+=3)
    {
      a.addTriangle(mIndices[i],mIndices[i+1],mIndices[i+2]);
    }
  */
  for(std::vector<MeshTriangle>::iterator i=tris.begin();i!=tris.end();i++)
    a.addTriangle(i->i0,i->i1,i->i2);
  



  if(mIndices.size()==0)
    cdebug("NO TRIANGLES FOUND!");
  cdebug("SAVED:"<<1.0f-float(saved)/mIndices.size());
  return a;
}

AGVector4 MeshOptimizer::getV(size_t i)
{
  return mVertices[i].v;
}
