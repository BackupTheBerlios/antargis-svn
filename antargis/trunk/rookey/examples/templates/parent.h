#ifndef PARENT_H
#define PARENT_H

// INCLUDE_SWIG


#include <rk_rubyobj.h>
#include "child.h"
#include <list>

class Parent:public AGRubyObject
{
public:
  Parent();
  virtual ~Parent() throw();
  virtual void mark() throw();

  Child *getFirst();
  std::list<Child*> getChildren();

private:
  std::list<Child*> mChildren;
};

#endif
