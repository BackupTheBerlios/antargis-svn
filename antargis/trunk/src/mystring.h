#ifndef __mystring_h
#define __mystring_h

#include <string>
#include <deque>

using namespace std;

inline deque<string> split(string needle,string haystack)
{
  deque<string> d;
  size_t i,last=0;
  for(i=haystack.find(needle);i!=string::npos;i=haystack.find(needle,i+1))
    {
      d.push_back(haystack.substr(last,i-last));
      last=i+needle.length();
    }
  i=haystack.length();
  d.push_back(haystack.substr(last,i-last));
  return d;
}

inline string replace(string str,string a,string b)
{
  size_t i=str.find(a);
  while(i!=str.npos)
    {
      str=str.substr(0,i)+b+str.substr(i+a.length(),str.length()-i-a.length());
      i=str.find(a,i+b.length());
    }
  return str;
}



#endif
