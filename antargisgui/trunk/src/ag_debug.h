/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_debug.h
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "Antargis" project, including all files needed to compile it,
 * is free software; you can redistribute it and/or use it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 */

#ifndef AG_DEBUG_h
#define AG_DEBUG_h
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <sstream>
#include <string>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>

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

#define debug(c) mydebug(::toString(__FILE__),::toString(__LINE__),c)

template<class T>
inline void mydebug(std::string f,std::string l,const T&t)
{
  getDebug()<<"File "<<f<<" Line "<<l<<" : "<<t<<std::endl;
}


using std::endl;

#define debugout(x) getDebug()<<x

#define cdebug(x) debugout("("<<__FILE__<<":"<<__LINE__<<":"<<__PRETTY_FUNCTION__<<"):"<<x<<endl)
#define ccdebug(x) debugout("("<<__FILE__<<":"<<__LINE__<<":"<<__PRETTY_FUNCTION__<<":"<<((void*)this)<<"):"<<x<<endl)
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
      debugout("start of:"<<c<<std::endl);
      d++;
    }
  ~D()
    {
      d--;
      indent();
      debugout("end   of:"<<m<<std::endl);
    }
private:
  void indent()
  {
    for(int i=0;i<d;i++)
      debugout("  ");
  }
};

#define TRACE D test__LINE__(::toString(__FILE__)+::toString(" ")+::toString(__LINE__)+::toString(" ")+::toString( __PRETTY_FUNCTION__))
#define CTRACE D test__LINE__(::toString(__FILE__)+::toString(" ")+::toString(__LINE__)+::toString(" ")+::toString( __PRETTY_FUNCTION__)+::toString(" ")+::toString(((void*)this)))

#define Assert(x) assert(x)

#define STUB cdebug("STUB in File "<<__FILE__<<" line:"<<__LINE__<<" function:"<<__PRETTY_FUNCTION__)

#ifdef __WIN32__
#define CHECK_ZERO(x) assert(x);
#else
#define CHECK_ZERO(x) if(!x) throw AGException(std::string(__STRING(x))+" is zero in "+__FILE__+" line:"+toString(__LINE__)+" functin:"+__PRETTY_FUNCTION__);
#endif

inline void myAssertGL(std::string s)
{
  //#ifdef DEBUG
  GLenum error = glGetError();
  if(error != GL_NO_ERROR) {
    std::ostringstream msg;
    msg <<s<< ": OpenGLError "
        << gluErrorString(error);
    
    cdebug(msg.str());
    throw std::runtime_error(msg.str());
  }
  //#endif

}

#define assertGL myAssertGL(::toString(__FILE__)+::toString(" ")+::toString(__LINE__)+::toString(" ")+::toString( __PRETTY_FUNCTION__))

#endif
