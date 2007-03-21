/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_debug.cc
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

#include "ag_debug.h"
#include "ag_main.h"

#include <fstream>
#include <iostream>

#ifdef __WIN32__
#include <winsock2.h>
#endif
#include <ruby.h>
#include <SDL.h>

bool quietLog=false;
void setQuiet()
{
  quietLog=true;
}

static bool gRubyRaising=true;
void agRaise(const std::string &s)
{
  cdebug("assertion failed:"<<s);
  if(gRubyRaising)
    rb_raise(rb_eRuntimeError,s.c_str(),"");
  else
    throw std::runtime_error(s);
}

void setRubyRaising(bool flag)
{
  gRubyRaising=flag;
}

size_t gDebugLevel=0;

size_t getDebugLevel()
{
  return gDebugLevel;
}
void setDebugLevel(size_t t)
{
  gDebugLevel=t;
}


#ifndef MNDEBUG
int D::d=0;

std::ofstream debugOFS("debug.txt");

std::ostream &getDebug()
{
  if(quietLog)
    return debugOFS;
  else
    return std::cout;
}

size_t gDebugIndex=0;

size_t getDebugIndex()
{
  return gDebugIndex;
}



D::D(std::string c):
  m(c)
{
  indent();
  gDebugIndex++;

  dbout(2,"start of:"<<c<<"("<<gDebugIndex<<")"<<std::endl);
  d++;
}
AGEXPORT D::~D()
{
  d--;
  indent();
  dbout(2,"end   of:"<<m<<std::endl);
}
void D::indent()
{
  for(int i=0;i<d;i++)
    debugout_checked(2,"  ");
}


#endif
void agAssertGL(std::string s)
{
  if(glMode())
    {
      GLenum error = glGetError();
      if(error != GL_NO_ERROR) {
	std::ostringstream msg;
	msg <<s<< ": OpenGLError "
	    << gluErrorString(error);
	
	std::cerr<<msg.str()<<std::endl;
	throw std::runtime_error(msg.str());
	agRaise(msg.str());
      }
    }

  // asserting SDL_errors
  char *se=SDL_GetError();
  if(se)
    {
      if(strlen(se)>0)
	{
	  std::cerr<<"SDL_Error:"<<s<<":"<<se<<std::endl;
	  if(std::string(se).substr(0,37)=="Failed loading glXGetSwapIntervalMESA")
	    std::cerr<<"IGNORING THIS ERROR!"<<std::endl;
	  else
	    agRaise(se);
	  SDL_ClearError();
	}
    }
  else
    {
      std::cerr<<"MUH"<<std::endl;
    }

}



