//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include<vector>
#include<algorithm>

class RandomMap
{
  std::vector<int> v;
  int mMax;
  int mVar;
  public:
  RandomMap(int pMax,int pVar=1):mMax(pMax),mVar(pVar)
  {
    for(int i=0;i<mMax*mVar;i++)
      v.push_back(i);
    std::random_shuffle(v.begin(),v.end());
  }
  
  int get(int p) const
  {
    p%=mMax*mVar;
    if(p>=0 && p<mMax*mVar)
      return v[p]%mMax;
    return -1;
  }
   
};
