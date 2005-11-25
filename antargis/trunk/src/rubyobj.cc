#include "rubyobj.h"
#include <assert.h>
#include <typeinfo>
//#include "ag_debug.h"
#include <set>
#include <iostream>

std::set<RubyObject*> gDeletedRubies;

RubyObject::RubyObject()
{
  mRubyObject=false;
  mDeleted=false;
}
RubyObject::~RubyObject()
{
  mDeleted=true;
  gDeletedRubies.insert(this);
}

void RubyObject::mark()
{
  //  cdebug(typeid(*this).name());
}
void RubyObject::markObject(RubyObject *o)
{
  assert(o);
  if(o->mRubyObject)
    rb_gc_mark(o->mRUBY);
  else
    o->mark();
}

void RubyObject::clear()
{
}

void general_markfunc(void *ptr)
{
  
  if(!ptr)
    {
      //      cdebug("WARNING: a ptr==0 was given in general_markfunc!");
      return; // ignore
    }
  assert(ptr);
  RubyObject *o=static_cast<RubyObject*>(ptr);
  assert(o);
  o->mark();
  gDeletedRubies.clear();
}

// ruby does a gc-run and then deletes everything, which is not marked
// so eventually sub-instances of a rubyobject are killed before
// a workaround for this:
// 1) put all deleted objects in a set
// 2) check set before clearing/deleting
// 3) clear this set when marking

void saveDelete(RubyObject *o)
{
  //  cdebug("SAVEDEL:"<<o);
  assert(o);
  if(gDeletedRubies.find(o)!=gDeletedRubies.end())
    return; // already deleted
  assert(!o->mDeleted);
  o->clear();
  if(!o->mRubyObject)
    delete o;
}
