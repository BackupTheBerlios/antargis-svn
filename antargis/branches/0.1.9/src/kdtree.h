/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * kdtree.h
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

#ifndef KDTREE_H
#define KDTREE_H

class KDTree
{
  struct Node
  {
    AGBox3 box;
    Node *a,*b;
    std::list<AGTriangle3> content;
    AGVector3 point;

    Node(const AGBox3 &pBox):box(pBox)
    {
      a=b=0;
    }

    bool insert(const AGTriangle3 &t)
    {
      if(a||b)
	{
	  assert(a);
	  assert(b);
	  if(a->insert(t))
	    return true;
	  if(b->insert(t))
	    return true;
	}
      if(box.contains(t))
	{
	  content.push_back(t);
	  return true;
	}
      return false;
    }
    bool intersect(const AGLine3 &pLine)
    {
      if(box.collides(pLine))
	{
	  if(
	}
    }

    /// very fast split
    void split()
    {
      assert(a==0);
      assert(b==0);
      
      int i=0,m=0;
      float v=0;
      for(;i<3;i++)
	if(box.dir[i]>v)
	  {
	    v=box.dir[i];
	    m=i;
	  }

      AGBox3 b0(box);
      AGBox3 b1(box);
      
      b0.dir[m]=b1.dir[m]=b0.dir[m]/2;
      b1.base[m]+=b0.dir[m];

      a=new Node(b0);
      b=new Node(b1);
    }
  };


 public:
  KDTree(
};


#endif
