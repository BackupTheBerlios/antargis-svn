#ifndef QUADTREE_H
#define QUADTREE_H

#include "ant_rect.h"

#include <set>

template<class T>
class QuadTree
  {
    struct Node
      {
        std::set
          <T*> ts;
        std::list<Node*> children;
        Rect2D r;

        Node(Rect2D R):r(R)
      {}
      
        void get(const Rect2D &r,std::list<T*> &l)
        {
          typename std::set<T*>::iterator i=ts.begin();
          for(;i!=ts.end();i++)
            l.push_back(*i);
            
          typename std::list<Node*>::iterator j=children.begin();
          for(;j!=children.end();j++)
            if((*j)->r.intersects(r))
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
          typename std::set<T*>::iterator j=ts.begin();
          for(;j!=ts.end();j++)
          {
            delete *j;
          }
          ts.clear();
          
        }

        void split()
        {
          std::set
            <T*> ot=ts;
          ts.clear();
          std::list<Rect2D> rs=r.split();
          std::list<Rect2D>::iterator i=rs.begin();
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
    QuadTree(Rect2D r)
    {
      root=new Node(r);
    }

    void insert(T* t)
    {
      root->insert(t);
    }
    
    std::list<T*> get(const Rect2D &r)
    {
      std::list<T*> l;
      root->get(r,l);
      return l;
    }
    
    void remove
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
