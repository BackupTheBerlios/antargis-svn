#ifndef RUBY_OBJ_H
#define RUBY_OBJ_H

#ifdef WIN32
#undef connect
#undef close
#endif

/**
   AGRubyObject does handling between ruby and c++ memory management.
   Ruby has a garbage collector (GC) which is not really to be influenced.
   Every object that is created in ruby is handled by the GC.
   So we have to take care of that.

   Ruby's GC has no reference counting whatsoever but it uses a mark-and-sweep
   algorithm. When the GC is run the algorithm marks any known object and then calls
   all the objects' mark-functions which therefore call mark-functions all the objects it
   knowns. When anything known is marked, the algorithm kills any object that isn't marked.

   So all we have to do is to mark any object that is connected to the current object in any way.
   Normally you would mark any object that is a children of the current one.

   To make things easier AGRubyObject takes care of objects which are not handled by ruby. They get called
   recursively anyways. So all you have to do is overriding the mark() function and then calling 
   markObject(AGRubyObject *o) for any object you want to mark.

   If you need an explicit destructor-call at a specified time you must be content with a call to clear(), which
   you can override. For deletion of any AGRubyObject you should use saveDelete. This calls clear() and deletes the
   object, if it's not a ruby-managed one.

   @see saveDelete(AGRubyObject *o)
*/

class AGRubyObject
{
 public:
  /**
     This is copied from ruby.h, so that this header-file is not included all over the place.
     FIXME: this must eventually be changed - when the ruby implementation changes
  */
  typedef unsigned long VALUE;
 
  AGRubyObject();
  virtual ~AGRubyObject();

 protected:
#ifndef SWIG
  // functions shouldn't be exported and NEVER be used in ruby!
  virtual void clear();
  virtual void mark();
  void markObject(AGRubyObject *o);
#endif
 public:
#ifndef SWIG
  bool mRubyObject;
  VALUE mRUBY;
  bool mDeleted;
#endif
  friend void general_markfunc(void *ptr);
  friend bool saveDelete(AGRubyObject *o);
};

void general_markfunc(void *ptr);
bool saveDelete(AGRubyObject *o);

#endif
