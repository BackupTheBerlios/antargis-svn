#include "mystring.h"

#include <string>
#include <deque>

std::string replace(const std::string &s,const std::string &a,const std::string &b)
{
  std::string str=s;
  size_t i=str.find(a);
  while(i!=str.npos)
    {
      str=str.substr(0,i)+b+str.substr(i+a.length(),str.length()-i-a.length());
      i=str.find(a,i+b.length());
    }
  return str;
}
