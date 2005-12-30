#ifndef RUBY_OBJ_H
#define RUBY_OBJ_H

#ifdef __WIN32__
#include <winsock2.h> // fix problem with fd_set
#endif
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
bool saveDelete(RubyObject *o);

#ifdef __WIN32__ // fix problem with "my" connect and connect of winsock
#undef connect
#endif

#endif
