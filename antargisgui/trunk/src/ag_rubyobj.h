#ifndef RUBY_OBJ_H
#define RUBY_OBJ_H

class AGRubyObject
{
 public:
  // FIXME: this must eventually be changed - this is only here, so that ruby.h mustn't be included all over the place
  typedef unsigned long VALUE;
 
  AGRubyObject();
  virtual ~AGRubyObject();

  virtual void clear();
 protected:
  virtual void mark();
  void markObject(AGRubyObject *o);
 public:
#ifndef SWIG
  bool mRubyObject;
  VALUE mRUBY;
  bool mDeleted;
#endif
  friend void general_markfunc(void *ptr);
};

void general_markfunc(void *ptr);
bool saveDelete(AGRubyObject *o);

#endif
