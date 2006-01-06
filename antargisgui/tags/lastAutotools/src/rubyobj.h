#ifndef RUBY_OBJ_H
#define RUBY_OBJ_H

class RubyObject
{
 public:
  // FIXME: this must eventually be changed - this is only here, so that ruby.h mustn't be included all over the place
  typedef unsigned long VALUE;
 
  RubyObject();
  virtual ~RubyObject();

  virtual void clear();
 protected:
  virtual void mark();
  void markObject(RubyObject *o);
 public:
#ifndef SWIG
  bool mRubyObject;
  VALUE mRUBY;
  bool mDeleted;
#endif
  friend void general_markfunc(void *ptr);

};

void general_markfunc(void *ptr);
bool saveDelete(RubyObject *o);

#endif
