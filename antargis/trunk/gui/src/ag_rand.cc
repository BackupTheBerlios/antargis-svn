/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ag_rand.cc
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

#include "ag_rand.h"
#include "ag_debug.h"
#include "mtwist.h"

#include <sstream>

static mt_state mState;


AGRandomizer::AGRandomizer(const std::string &pSeed)
{
  std::istringstream is;
  is.str(pSeed);

  is>>mState.stateptr;
  is>>mState.initialized;

  for(unsigned long i=0;i<MT_STATE_SIZE;i++)
    is>>mState.statevec[i];
  
  //  mts_seed(&mState);

}
float AGRandomizer::operator()(float f)
{
  float d=(float)mts_drand(&mState);
  cdebug("d:"<<d<<" f:"<<f);
  d*=f;
  cdebug("d2:"<<d);
  
  return d;
}
int AGRandomizer::operator()(int i)
{
  int r=mts_lrand(&mState);
  cdebug("r:"<<r<<" i:"<<i);
  r%=i;
  cdebug("r2:"<<r);
  return r;
}

std::string AGRandomizer::stateToString() const
{
  std::ostringstream os;
  os<<mState.stateptr<<" "<<mState.initialized<<" ";

  for(unsigned long i=0;i<MT_STATE_SIZE;i++)
    os<<mState.statevec[i]<<" ";

  return os.str();

}



