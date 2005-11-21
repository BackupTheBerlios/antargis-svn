#include "rubyobj.h"
#include <assert.h>
#include <typeinfo>
#include "ag_debug.h"

RubyObject::RubyObject()
{
  mRubyObject=false;
}
RubyObject::~RubyObject()
{
}

void RubyObject::mark()
{
  //  cdebug(typeid(*this).name());
}
void RubyObject::markObject(RubyObject *o)
{
  if(o->mRubyObject)
    rb_gc_mark(o->mRUBY);
  else
    o->mark();
}


void general_markfunc(void *ptr)
{
  assert(ptr);
  RubyObject *o=static_cast<RubyObject*>(ptr);
  assert(o);
  o->mark();
}

