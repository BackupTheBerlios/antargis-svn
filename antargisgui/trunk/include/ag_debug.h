#ifndef AG_DEBUG_h
#define AG_DEBUG_h

#include <iostream>
#include <fstream>
#include <assert.h>
#include <sstream>
#include <string>
#include <vector>

#include "ag_exception.h"


std::string replace(const std::string &s,const std::string &a,const std::string &b);

template<class T>
inline std::string toString(const T&t)
{
  std::ostringstream os;
  os<<t;
  return os.str();
}

template<>
inline std::string toString(const bool&b)
{
  if(b)
    return "true";
  else
    return "false";
}

std::ostream &getDebug();

#define debug(c) mydebug(toString(__FILE__),toString(__LINE__),c)

template<class T>
inline void mydebug(std::string f,std::string l,const T&t)
{
  getDebug()<<"File "<<f<<" Line "<<l<<" : "<<t<<std::endl;
}


using std::endl;

#define cdebug(x) getDebug()<<"("<<__FILE__<<":"<<__LINE__<<":"<<__PRETTY_FUNCTION__<<"):"<<x<<endl
#define ccdebug(x) getDebug()<<"("<<__FILE__<<":"<<__LINE__<<":"<<__PRETTY_FUNCTION__<<":"<<((void*)this)<<"):"<<x<<endl
//#define cdebug(x) std::cout<<"("<<__FILE__<<":"<<__LINE__<<":"<<__func__<<"):"<<x<<endl


std::vector<std::string> split(const std::string &n,const std::string &h);

class D
{
  std::string m;
  static int d;

 public:
  D(std::string c):
    m(c)
    {
      indent();
      getDebug()<<"start of:"<<c<<endl;
      d++;
    }
  ~D()
    {
      d--;
      indent();
      getDebug()<<"end   of:"<<m<<endl;
    }
private:
  void indent()
  {
    for(int i=0;i<d;i++)
      getDebug()<<"  ";
  }
};

#define TRACE D test__LINE__(toString(__FILE__)+toString(" ")+toString(__LINE__)+toString(" ")+toString( __PRETTY_FUNCTION__))
#define CTRACE D test__LINE__(toString(__FILE__)+toString(" ")+toString(__LINE__)+toString(" ")+toString( __PRETTY_FUNCTION__)+toString(" ")+toString(((void*)this)))

#define Assert(x) assert(x)

#define STUB cdebug("STUB in File "<<__FILE__<<" line:"<<__LINE__<<" function:"<<__PRETTY_FUNCTION__)

#ifdef __WIN32__
#define CHECK_ZERO(x) assert(x);
#else
#define CHECK_ZERO(x) if(!x) throw AGException(std::string(__STRING(x))+" is zero in "+__FILE__+" line:"+toString(__LINE__)+" functin:"+__PRETTY_FUNCTION__);
#endif

#endif
