#include "ag_base.h"
#include "ag_rubyobj.h"
#include <assert.h>
#include <typeinfo>
#include <set>
#include <map>
#include <iostream>


#ifdef __WIN32__
#include <winsock2.h> // fix problem with fd_set
#endif
#include <ruby.h>

bool mRubyQuitting=false;

// this set keeps track of all valid instances of AGRubyObject
std::set<AGRubyObject*> mRubyObjects;
std::set<AGRubyObject*> mRemovedRubyObjects;

void setQuitting()
  {
    mRubyQuitting=true;
  }

// swig-function for handling "tracked" objects
// BEWARE: this does not support multiple libraries yet (FIXME)
VALUE convertCpp2Ruby(AGRubyObject *cObject);

AGEXPORT void *getAddressOfRubyObject(AGRubyObject *o)
  {
    return o;
  }

AGEXPORT bool rubyObjectExists(void *po)
  {
    VALUE v=convertCpp2Ruby((AGRubyObject*)po);
    return(v!=Qnil);
  }

AGRubyObject::AGRubyObject()
  {
    //std::cerr<<"AGRubyObject::new:"<<this<<std::endl;
    mRubyObjects.insert(this);
  }
AGRubyObject::~AGRubyObject()
  {
    //std::cerr<<"AGRubyObject::Removed:"<<this<<std::endl;
    for(std::set<AGBaseObject*>::iterator i=mReferences.begin();i!=mReferences.end();i++)
      (*i)->baseClear();
    mRubyObjects.erase(this);
    mRemovedRubyObjects.insert(this);
  }


/// override this function to mark your children
void AGRubyObject::mark()
  {
  }


// call this function with any object you want to mark.
// recursive should be set true only in one direction, otherwise you'll generate endless-loops (!!)
void AGRubyObject::markObject(AGRubyObject *o, bool recursive)
  {
    //std::cerr<<"markObject:"<<o<<std::endl;
    // o must be a valid ruby-object
    assert(mRubyObjects.find(o)!=mRubyObjects.end());

    // look up, if it's registered within ruby
    VALUE v=convertCpp2Ruby(o);
    if(v!=Qnil)
      {
        //std::cout<<"V:"<<v<<std::endl;
        // then mark it
        rb_gc_mark(v);
      }

    assert(o);
    // recurse through hierarchy
    if(recursive)
      o->mark(); // call this directly
  }

void AGRubyObject::clear()
  {
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
    if(!ptr)
      {
        std::cerr<<("WARNING: a ptr==0 was given in general_markfunc!")<<std::endl;
        return; // ignore this !
      }
    assert(ptr);
    // the given object must be a AGRubyObject and it must be valid (it's in mRubyObjects)
    AGRubyObject *o=static_cast<AGRubyObject*>(ptr);
    if(mRubyObjects.find(o)==mRubyObjects.end())
      std::cerr<<"OLD RUBYOBJ:"<<(mRemovedRubyObjects.find(o)!=mRemovedRubyObjects.end())<<":"<<o<<std::endl;
    assert(mRubyObjects.find(o)!=mRubyObjects.end());

#ifdef GCDEBUG
    printf("mark: 0x%lx\n",o->mRUBY);
#endif

    assert(o);
    o->mark();
  }


/**
   saveDelete is used to delete AGRubyObjects savely.
 */
bool saveDelete(AGRubyObject *o)
  {
    if(mRubyQuitting)
      return false; // we are quitting - so memory is discarded anyway - hopefully ;-)

    // check, if this object is existant any longer
    // in case we're quitting this the deletion order is not defined for ruby-objects !!
    if(mRubyObjects.find(o)==mRubyObjects.end())
      {
#ifdef GC_DEBUG
        std::cerr<<"RubyObject "<<o<<" no longer existant - maybe we're quitting ?!"<<std::endl;
#endif
        return false;
      }

    assert(o);
    // send object a message, that it will be deleted. This can help with detachin connections
    // between objects.
    o->clear();
    
    VALUE v=convertCpp2Ruby(o);
    if(v!=Qnil)
      return false; // do not delete - it's under ruby's control!
    
    
    delete o;
    return true;
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
