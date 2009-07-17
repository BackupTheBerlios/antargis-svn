/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * rubytracking.swg
 *
 * This file contains support for tracking mappings from 
 * Ruby objects to C++ objects.  This functionality is needed
 * to implement mark functions for Ruby's mark and sweep
 * garbage collector.
 * ----------------------------------------------------------------------------- */

#include <map>

#ifdef __cplusplus
extern "C" {
#endif



/* Global Ruby hash table to store Trackings from C/C++
   structs to Ruby Objects. 
*/
static std::map<void*, VALUE> swig_ruby_trackings;

/* Global variable that stores a reference to the ruby
   hash table delete function. */
static ID swig_ruby_hash_delete;

/* Setup a Ruby hash table to store Trackings */
SWIGRUNTIME void SWIG_RubyInitializeTrackings(void) {
  /* Create a ruby hash table to store Trackings from C++ 
     objects to Ruby objects. */

  /* Try to see if some other .so has already created a 
     tracking hash table, which we keep hidden in an instance var
     in the SWIG module.
     This is done to allow multiple DSOs to share the same
     tracking table.
  */
  VALUE verbose = rb_gv_get("VERBOSE");
  rb_gv_set("VERBOSE", Qfalse);
  rb_gv_set("VERBOSE", verbose);

  /* Now store a reference to the hash table delete function
     so that we only have to look it up once.*/
  swig_ruby_hash_delete = rb_intern("delete");
}

/* Add a Tracking from a C/C++ struct to a Ruby object */
SWIGRUNTIME void SWIG_RubyAddTracking(void* ptr, VALUE object) {
  /* In a Ruby hash table we store the pointer and
     the associated Ruby object.  The trick here is
     that we cannot store the Ruby object directly - if
     we do then it cannot be garbage collected.  So
     instead we typecast it as a unsigned long and
     convert it to a Ruby number object.*/

  /* Store the mapping to the global hash table. */
  swig_ruby_trackings[ptr]=object;
}

/* Get the Ruby object that owns the specified C/C++ struct */
SWIGRUNTIME VALUE SWIG_RubyInstanceFor(void* ptr) {
  std::map<void*,VALUE>::iterator i=swig_ruby_trackings.find(ptr);
  if(i==swig_ruby_trackings.end())
    return Qnil;
  return i->second;
}

/* Remove a Tracking from a C/C++ struct to a Ruby object.  It
   is very important to remove objects once they are destroyed
   since the same memory address may be reused later to create
   a new object. */
SWIGRUNTIME void SWIG_RubyRemoveTracking(void* ptr) {
  swig_ruby_trackings.erase(ptr);
}

/* This is a helper method that unlinks a Ruby object from its
   underlying C++ object.  This is needed if the lifetime of the
   Ruby object is longer than the C++ object */
SWIGRUNTIME void SWIG_RubyUnlinkObjects(void* ptr) {
  VALUE object = SWIG_RubyInstanceFor(ptr);

  if (object != Qnil) {
    DATA_PTR(object) = 0;
  }
}


#ifdef __cplusplus
}
#endif
