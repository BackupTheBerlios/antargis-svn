/*
 * Copyright (c) 2005 by David Kamphausen. All rights reserved.
 *
 * quadtree.h
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

#ifndef QUADTREE_H
#define QUADTREE_H

#include <ag_triangle.h>

#include <set>

template<class T>
class QuadTree
  {
    struct Node
      {
        std::set
          <T*> ts;
        std::list<Node*> children;
        AGRect2 r;

        Node(AGRect2 R):r(R)
      {}
      
        void get(const AGRect2 &r,std::list<T*> &l)
        {
          typename std::set<T*>::iterator i=ts.begin();
          for(;i!=ts.end();i++)
            l.push_back(*i);
            
          typename std::list<Node*>::iterator j=children.begin();
          for(;j!=children.end();j++)
            if((*j)->r.collide(r))
              (*j)->get(r,l);
        }

        void insert(T* t)
        {
          if(children.size())
            {
              typename std::list<Node*>::iterator i=children.begin();
              for(;i!=children.end();i++)
                if((*i)->r.contains(t->getRect()))
                  {
                    (*i)->insert(t);
                    return;
                  }
            }
          ts.insert(t);
        }
        
        void clear()
        {
          typename std::list<Node*>::iterator i=children.begin();
          for(;i!=children.end();i++)
          {
            (*i)->clear();
            delete *i;
          }
          children.clear();
	  /* expect that we don't own elements
          typename std::set<T*>::iterator j=ts.begin();
          for(;j!=ts.end();j++)
          {
            delete *j;
	    }*/
          ts.clear();
          
        }

        void split()
        {
          std::set
            <T*> ot=ts;
          ts.clear();
          std::list<AGRect2> rs=r.split();
          std::list<AGRect2>::iterator i=rs.begin();
          for(;i!=rs.end();i++)
            {
              children.push_back(new Node(*i));
            }
          typename std::set
            <T*>::iterator j=ot.begin();
          for(;j!=ot.end();j++)
            insert(*j);
        }

        void remove
          (T* t)
          {
            if(ts.find(t)!=ts.end())
              {
                ts.erase(t);
                return;
              }
            if(children.size())
              {
                typename std::list<Node*>::iterator i=children.begin();
                for(;i!=children.end();i++)
                  {
                    if((*i)->r.contains(t->getRect()))
                      {
                        (*i)->remove
                        (t);
                        return;
                      }
                  }
              }
          }

      };

    Node *root;

  public:
    QuadTree(AGRect2 r)
    {
      root=new Node(r);
    }

    void insert(T* t)
    {
      root->insert(t);
    }
    
    std::list<T*> get(const AGRect2 &r)
    {
      std::list<T*> l;
      root->get(r,l);
      return l;
    }
    
    void erase
      (T* t)
      {
        root->remove
        (t);
      }
      
    void clear()
    {
      root->clear();
    }
  };

#endif
