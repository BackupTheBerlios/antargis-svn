#include "ag_debug.h"

int D::d=0;

/*
std::vector<std::string> split(const std::string &n,const std::string &h)
{
  cdebug(n<<" "<<h);
  std::vector<std::string> a;
  size_t i=0;
  for(;i!=h.npos && i<h.length();i++)
    {
      size_t j=h.find(n,i);
      if(j!=std::string::npos)
	a.push_back(h.substr(i,j-i));
      else
	{
	  a.push_back(h.substr(i,j));
	  break;
	}
      cdebug("i:"<<i<<" j:"<<j);
      cdebug(a[a.size()-1]);
      i=j+1;
    }
  
return a;
}*/


std::vector<std::string> split(const std::string &needle,const std::string &haystack)
{
  std::vector<std::string> d;
  size_t i,last=0;
  for(i=haystack.find(needle);i!=std::string::npos;i=haystack.find(needle,i+1))
    {
      d.push_back(haystack.substr(last,i-last));
      last=i+needle.length();
    }
  i=haystack.length();
  d.push_back(haystack.substr(last,i-last));
  return d;
}

