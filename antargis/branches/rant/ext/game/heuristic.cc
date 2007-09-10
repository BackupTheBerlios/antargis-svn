#include "heuristic.h"
#include <ag_debug.h>
#include <ag_profiler.h>
#include "ag_serial_vec.h"

#include <set>

HeuristicFunction::Output HeuristicFunction::operator()(const Input &input) 
{
  return (input.second-input.first).length()*1.5;
}

void HeuristicFunction::printTo(BinaryOut &pOut)
{
}

float HeuristicFunction::get(const AGVector2 &a,const AGVector2 &b)
{
  return (*this)(std::make_pair(a,b));
}



StoredHeuristicFunction::StoredHeuristicFunction()
{
}

StoredHeuristicFunction::StoredHeuristicFunction(BinaryIn &pIn)
{
#ifdef OLD_STORE
  Uint32 s;
  AGVector2 v;
  Uint16 ai,bi;
  float w;
  pIn>>s;
  cdebug("S:"<<s);

  assert(s<10000);

  std::vector<AGVector2> allVecs;

  cdebug("reading vecs..");
  for(size_t i=0;i<s;i++)
    {
      pIn>>v;
      allVecs.push_back(v);
      //      cdebug(i<<":"<<v);
    }


  pIn>>s;

  cdebug("S:"<<s);

  assert(s<2000000); // sanity check

  cdebug("reading edges..");
  for(size_t i=0;i<s;i++)
    {
      {
	STACKTRACE;
	pIn>>ai>>bi>>w;
      }
      {
	STACKTRACE;
	//      cdebug("ai:"<<ai<<" bi:"<<bi<<" w:"<<w);
	mMap.insert(std::make_pair(std::make_pair(allVecs[ai],allVecs[bi]),w));
      }
    }
  cdebug("ready.");
#else
  Uint32 s;
  AGVector2 v;
  Uint16 ai,bi;
  float w;
  pIn>>s;

  assert(s<10000);

  for(size_t i=0;i<s;i++)
    {
      pIn>>v;
      mVecs[v]=i;
      //      cdebug(i<<":"<<v);
    }


  pIn>>s;

  assert(s==mVecs.size()*mVecs.size());
  assert(s<2000000); // sanity check

  mMapVec=std::vector<float>(s);

  for(size_t i=0;i<s;i++)
    {
      {
	STACKTRACE;
	pIn>>ai>>bi>>w;
      }
      {
	STACKTRACE;

	mMapVec[ai+bi*mVecs.size()]=w;
      }
    }
#endif
}


void StoredHeuristicFunction::store(Input in,Output out)
{
#ifdef NEW_STORE
  assert(mMapVec.size()==0);
#endif
  mMap[in]=out;
}

void StoredHeuristicFunction::store(const AGVector2 &from,const AGVector2 &to,float value)
{
#ifdef NEW_STORE
  assert(mMapVec.size()==0);
#endif
  mMap[std::make_pair(from,to)]=value;
}


  //  void display();

StoredHeuristicFunction::Output StoredHeuristicFunction::operator()(const Input &input)
{
#ifdef NEW_STORE
  if(mMapVec.size()>0)
    {
      size_t i=getIndex(input);
      return mMapVec[i];
    }
#endif
  return mMap[input];
}


#ifdef NEW_STORE
size_t StoredHeuristicFunction::getIndex(const Input &input)
{
  size_t x=mVecs[input.first];
  size_t y=mVecs[input.second];

  return x+y*mVecs.size();
}
#endif



void StoredHeuristicFunction::printTo(BinaryOut &pOut)
{
  std::set<AGVector2> allVecs;

  assert(mMapVec.size()==0);

  for(std::map<Input,Output>::iterator i=mMap.begin();i!=mMap.end();i++)
    {
      allVecs.insert(i->first.first);
      allVecs.insert(i->first.second);
    }

  std::vector<AGVector2> vecs;
  std::copy(allVecs.begin(),allVecs.end(),std::back_inserter(vecs));
  std::map<AGVector2,size_t> indices;

  pOut<<(Uint32)vecs.size();

  for(size_t i=0;i<vecs.size();i++)
    {
      indices[vecs[i]]=i;
      pOut<<vecs[i];
    }

  pOut<<(Uint32)mMap.size();

  for(std::map<Input,Output>::iterator i=mMap.begin();i!=mMap.end();i++)
    {
      AGVector2 a=i->first.first,b=i->first.second;
      Uint16 ai=indices[a];
      Uint16 bi=indices[b];
      float w=i->second;
      pOut<<ai;
      pOut<<bi;
      pOut<<w;
    }

}
