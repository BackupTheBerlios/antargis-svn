#include "parent.h"

Parent::Parent()
{
  mChildren.push_back(new Child());
  mChildren.push_back(new Child());
  mChildren.push_back(new Child());
}

Parent::~Parent() throw()
{
  for(std::list<Child*>::iterator i=mChildren.begin();i!=mChildren.end();i++)
    saveDelete(*i);
}

void Parent::mark() throw()
{
  for(std::list<Child*>::iterator i=mChildren.begin();i!=mChildren.end();i++)
    markObject(*i);
}

Child *Parent::getFirst()
{
  return *mChildren.begin();
}

std::list<Child*> Parent::getChildren()
{
  return mChildren;
}
