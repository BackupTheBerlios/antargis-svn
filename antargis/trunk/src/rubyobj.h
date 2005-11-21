#ifndef RUBY_OBJ_H
#define RUBY_OBJ_H

#include <ruby.h>

class RubyObject
{
 public:
  RubyObject();
  virtual ~RubyObject();

  virtual void mark();
  void markObject(RubyObject *o);
#ifndef SWIG
  bool mRubyObject;
  VALUE mRUBY;
#endif
};

void general_markfunc(void *ptr);

#endif
