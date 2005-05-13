/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * kill.h
 * by David Kamphausen (david.kamphausen@web.de)
 *
 * The "AGargis" project, including all files needed to compile it,
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

#ifndef __ANT_KILL_H__
#define __ANT_KILL_H__

#include <list>

class AGInstanceBase
{
 public:
  virtual void kill()=0;
};

template<class T>
class AGInstance:public AGInstanceBase
{
  T *instance;
 public:
  AGInstance(T *t)
    {
      instance=t;
    }
  virtual void kill()
  {
    delete instance;
  }
};

template<class T>
AGInstanceBase *createKiller(T *t)
{
  return new AGInstance<T>(t);
}

class AGInstanceKiller
{
  std::list<AGInstanceBase*> bs;
 public:
  AGInstanceKiller()
    {
    }
  ~AGInstanceKiller()
    {
      std::list<AGInstanceBase*>::iterator i=bs.begin();
      for(;i!=bs.end();i++)
	{
	  (*i)->kill();
	  delete *i;
	}
      
    }
  void reg(AGInstanceBase *b)
  {
    bs.push_back(b);
  }
  
};

AGInstanceKiller *getInstanceKiller();

#define REGISTER_SINGLETON(s) getInstanceKiller()->reg(createKiller(s));

#endif
