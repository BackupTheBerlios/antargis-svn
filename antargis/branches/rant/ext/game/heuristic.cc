#include "heuristic.h"
#include <ag_debug.h>
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
  Uint32 s;
  AGVector2 v;
  Uint16 ai,bi;
  float w;
  pIn>>s;
  cdebug("S:"<<s);

  assert(s<10000);

  std::vector<AGVector2> allVecs;
  
  for(size_t i=0;i<s;i++)
    {
      pIn>>v;
      allVecs.push_back(v);
      cdebug(i<<":"<<v);
    }


  pIn>>s;

  cdebug("S:"<<s);

  assert(s<2000000); // sanity check

  for(size_t i=0;i<s;i++)
    {
      pIn>>ai>>bi>>w;
      //      cdebug("ai:"<<ai<<" bi:"<<bi<<" w:"<<w);
      mMap.insert(std::make_pair(std::make_pair(allVecs[ai],allVecs[bi]),w));
    }
}


void StoredHeuristicFunction::store(Input in,Output out)
{
  mMap[in]=out;
}

void StoredHeuristicFunction::store(const AGVector2 &from,const AGVector2 &to,float value)
{
  mMap[std::make_pair(from,to)]=value;
}


  //  void display();

StoredHeuristicFunction::Output StoredHeuristicFunction::operator()(const Input &input)
{
  return mMap[input];
}



void StoredHeuristicFunction::printTo(BinaryOut &pOut)
{
  std::set<AGVector2> allVecs;
  

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
