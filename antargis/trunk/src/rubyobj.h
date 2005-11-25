#ifndef RUBY_OBJ_H
#define RUBY_OBJ_H

#include <ruby.h>

class RubyObject
{
 public:
  RubyObject();
  virtual ~RubyObject();

  virtual void clear();
  virtual void mark();
  void markObject(RubyObject *o);
#ifndef SWIG
  bool mRubyObject;
  VALUE mRUBY;
  bool mDeleted;
#endif
};

void general_markfunc(void *ptr);
void saveDelete(RubyObject *o);

#endif
