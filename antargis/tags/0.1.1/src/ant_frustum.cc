/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * ant_frustum.h
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

#include "ant_frustum.h"
#include "ag_debug.h"

/////////////////////////////////////////////////////////////////////
// AntPlane
/////////////////////////////////////////////////////////////////////

AntPlane::AntPlane(const AGVector3 &dir,float offset):
  mDir(dir),
  mOffset(offset)
{
}

bool AntPlane::inside(const AGVector3 &v) const
{
  return v*mDir-mOffset>0;
}

AntPlane makePlane(const AGVector3 &p0,const AGVector3 &p1,const AGVector3 &p2)
{
  AGVector3 up=p1-p0;
  AGVector3 right=p2-p0;
  up/=up.length();
  right/=right.length();
#warning "maybe this must be swapped!"
  AGVector3 dir=up%right;
  
  return AntPlane(dir,dir*p0);
}


/////////////////////////////////////////////////////////////////////
// AntFrustum
/////////////////////////////////////////////////////////////////////
AntFrustum::AntFrustum(const std::vector<AntPlane> &pPlanes):
  mPlanes(pPlanes)
{
  assert(mPlanes.size()==6);

  //  cdebug(*this);
}

bool AntFrustum::inside(const AGVector3 &v) const
{
  for(std::vector<AntPlane>::const_iterator i=mPlanes.begin();i!=mPlanes.end();++i)
    if(!i->inside(v))
      return false;
  return true;
}

bool AntFrustum::collides(const AGBox3 &b) const
{
  for(std::vector<AntPlane>::const_iterator i=mPlanes.begin();i!=mPlanes.end();++i)
    {
      std::vector<AGVector4> vs=b.getVertices();
      bool outside=true;
      for(std::vector<AGVector4>::iterator j=vs.begin();j!=vs.end();++j)
	{
	  if(i->inside(j->dim3()))
	    {
	      outside=false;
	      break;
	    }
	}
      if(outside)
	return false;
    }
  return true;
}


std::ostream &operator<<(std::ostream &o,const AntPlane &p)
{
  o<<"("<<p.mDir<<","<<p.mOffset<<")";
  return o;
}
std::ostream &operator<<(std::ostream &o,const AntFrustum &p)
{
  for(std::vector<AntPlane>::const_iterator i=p.mPlanes.begin();i!=p.mPlanes.end();++i)
    o<<*i<<";";
  return o;
}
