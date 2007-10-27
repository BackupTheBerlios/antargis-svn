#include "ag_rubyobj.h"
#include "ag_debug.h"
#include "ag_profiler.h"
#include <assert.h>
#include <typeinfo>
#include <set>
#include <iostream>


#ifdef __WIN32__
#include <winsock2.h> // fix problem with fd_set
#endif
#include <ruby.h>

#include <ag_main.h>

std::set<AGRubyObject*> gExistingRubies;

AGRubyObject::AGRubyObject()
{
  mRubyObject=false;
  mDeleted=false;
  gExistingRubies.insert(this);
  mRUBY=0;
#ifdef GCDEBUG
  printf("NEW ME:%lx\n",this);
#endif
  
}
AGRubyObject::~AGRubyObject()
{
  for(std::set<AGBaseObject*>::iterator i=mReferences.begin();i!=mReferences.end();i++)
    (*i)->baseClear();


  //  cdebug("DEL:"<<mRUBY);
#ifdef GCDEBUG
  printf("DEL:%lx  %s (me:%lx)\n",mRUBY,mObjName.c_str(),this);
#endif
  mDeleted=true;
  gExistingRubies.erase(this);
  //  cdebug("DEL:"<<mRUBY);
}


/// override this function to mark your children
void AGRubyObject::mark()
{
}

// call this function with any object you want to mark.
void AGRubyObject::markObject(AGRubyObject *o, bool recursive)
{
  assert(o);

#ifdef GCDEBUG
	printf("marking : %lx\n",o);
  assert(gExistingRubies.find(o)!=gExistingRubies.end());
#endif

  if(o->mRubyObject)
    {
#ifdef GCDEBUG
			printf("marking ruby : %lx\n",o->mRUBY);
#endif
      rb_gc_mark(o->mRUBY);
    }
  if(recursive)
    o->mark(); // call this directly
#ifdef GCDEBUG
	printf("endmarking : %lx\n",o);
#endif
}

void AGRubyObject::clear()
{
  CTRACE;
}

void AGRubyObject::addRef(AGBaseObject *o)
{
  mReferences.insert(o);
}

void AGRubyObject::deleteRef(AGBaseObject *o)
{
  mReferences.erase(o);
}



/// this is the marking function, that gets called by ruby
/// it handles all the AGRubyObjects
void general_markfunc(void *ptr)
{
  STACKTRACE;
  //  TRACE;
  if(!ptr)
    {
      cdebug("WARNING: a ptr==0 was given in general_markfunc!");
      //      throw std::runtime_error("WARNING: a ptr==0 was given in general_markfunc!");
      return; // ignore
    }
  assert(ptr);
  AGRubyObject *o=static_cast<AGRubyObject*>(ptr);

#ifdef GCDEBUG
  printf("mark: 0x%lx\n",o->mRUBY);
#endif

  assert(o);
  o->mark();
}


/**
   saveDelete is used to delete AGRubyObjects savely.

   ruby does a gc-run and then deletes everything, which is not marked
   so eventually sub-instances of a rubyobject are killed before
   a workaround for this:
   1) put all deleted objects in a set
   2) check set before clearing/deleting
   3) clear this set when marking
*/
bool saveDelete(AGRubyObject *o)
{
  if(hasQuit())
    return false; // we are quitting - so memory is discarded anyway - hopefully ;-)
  
  assert(o);
  if(gExistingRubies.find(o)==gExistingRubies.end())
    {
#ifdef GCDEBUG
      cdebug("already deleted!");
#endif
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








AGBaseObject::AGBaseObject(AGRubyObject *p)
  :mp(p)
{
  if(p)
    p->addRef(this);
}

AGBaseObject::~AGBaseObject()
{
  if(mp)
    mp->deleteRef(this);
}

void AGBaseObject::baseClear()
{
  mp=0;
}
