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
//#include "ag_gl.h"

#include "ag_exception.h"
#include "ag_tools.h"
#include "ag_base.h"


AGEXPORT void setQuiet();

#ifdef MNDEBUG
#define dbout(x,l)
#define cdebug(x)
#define ccdebug(x)
#define debug(x)
#define TRACE
#define CTRACE
#define CHECK_ZERO(x)
#define STUB
//#define assertGL

#else

std::ostream & AGEXPORT getDebug();

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

AGEXPORT size_t getDebugIndex();
AGEXPORT size_t getDebugLevel();
AGEXPORT void setDebugLevel(size_t t);

#define debugout_checked(level,x) { if(level>getDebugLevel()) { getDebug()<<x; }}
#define dbout(level,x) {if(level>getDebugLevel()) { cdebug(x); }}

/** A helper class for tracing the program's flow
    Use it by instantiating it with a proper name, or simply use TRACE; (or CTRACE for classes) in your functions */
class AGEXPORT D
{
  std::string m;
  static int d;

 public:
  /// the given text will be output on creation and deletion of this object
  D(std::string text);
  ~D();
private:
  void indent();
};

#define LINEINFO(x) (::toString(__FILE__)+::toString(" ")+::toString(__LINE__)+::toString(" ")+::toString( __PRETTY_FUNCTION__)+::toString(" ")+::toString(x)).c_str()

#define TRACE D test__LINE__(::toString(__FILE__)+::toString(" ")+::toString(__LINE__)+::toString(" ")+::toString( __PRETTY_FUNCTION__))
#define CTRACE D test__LINE__(::toString(__FILE__)+::toString(" ")+::toString(__LINE__)+::toString(" ")+::toString( __PRETTY_FUNCTION__)+::toString(" ")+::toString(((void*)this))+::toString(" ")+typeid(*this).name())

AGEXPORT void agRaise(const std::string &s);

#ifndef __WIN32__
#undef assert
#define assert(x) {if(!(x)) agRaise((::toString("assert failed ")+LINEINFO(__STRING(x))).c_str()); }
#endif

#define Assert(x) assert(x)

#define STUB cdebug("STUB in File "<<__FILE__<<" line:"<<__LINE__<<" function:"<<__PRETTY_FUNCTION__)

#ifdef __WIN32__
#define CHECK_ZERO(x) assert(x);
#else
#define CHECK_ZERO(x) if(!x) throw AGException(std::string(__STRING(x))+" is zero in "+__FILE__+" line:"+toString(__LINE__)+" functin:"+__PRETTY_FUNCTION__);
#endif


#endif

void setRubyRaising(bool flag);


#endif
