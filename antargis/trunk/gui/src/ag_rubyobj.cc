#include "ag_rubyobj.h"
#include <assert.h>
#include <typeinfo>
//#include "ag_debug.h"
#include <set>
#include <iostream>


#ifdef __WIN32__
#include <winsock2.h> // fix problem with fd_set
#endif
#include <ruby.h>

#include <ag_main.h>

std::set<AGRubyObject*> gDeletedRubies;

AGRubyObject::AGRubyObject()
{
  mRubyObject=false;
  mDeleted=false;
}
AGRubyObject::~AGRubyObject()
{
  mDeleted=true;
  gDeletedRubies.insert(this);
}

void AGRubyObject::mark()
{
  //  cdebug(typeid(*this).name());
}
void AGRubyObject::markObject(AGRubyObject *o)
{
  assert(o);
  if(o->mRubyObject)
    rb_gc_mark(o->mRUBY);
  else
    o->mark();
}

void AGRubyObject::clear()
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
  AGRubyObject *o=static_cast<AGRubyObject*>(ptr);
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

bool saveDelete(AGRubyObject *o)
{
  if(hasQuit())
    return false; // we are quitting - so memory is discarded anyway - hopefully ;-)
  
  //  cdebug("SAVEDEL:"<<o);
  assert(o);
  if(gDeletedRubies.find(o)!=gDeletedRubies.end())
    {
      return false; // already deleted
    }
  assert(!o->mDeleted);
  o->clear();
  if(!o->mRubyObject)
    {
      delete o;
      return true;
    }
  return false;
}
