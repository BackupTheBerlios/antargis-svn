/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * kill.cc
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

#include "ag_kill.h"
#include "ag_debug.h"

AGInstanceKiller *mInstanceKiller=0;
bool mIndirectInstance=false;

void newInstanceKiller()
{
  assert(mInstanceKiller==0 || mIndirectInstance);
  mInstanceKiller=new AGInstanceKiller;
}
void deleteInstanceKiller()
{
  assert(mInstanceKiller);
  delete mInstanceKiller;
}

AGInstanceKiller *getInstanceKiller()
{
  if(!mInstanceKiller)
    {
      mIndirectInstance=true;
      newInstanceKiller();
    }
  assert(mInstanceKiller);
  return mInstanceKiller;
}


AGInstanceKiller::AGInstanceKiller()
    {
    }
AGInstanceKiller::~AGInstanceKiller()
    {
      CTRACE;
      std::list<AGInstanceBase*>::iterator i=bs.begin();
      for(;i!=bs.end();i++)
	{
	  (*i)->kill();
	  delete *i;
	}
      
    }
void AGInstanceKiller::reg(AGInstanceBase *b)
  {
    bs.push_back(b);
  }
