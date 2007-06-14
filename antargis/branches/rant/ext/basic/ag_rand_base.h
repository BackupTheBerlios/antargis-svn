#ifndef AG_RAND_BASE_H
#define AG_RAND_BASE_H

#include <ag_rubyobj.h>

class AGRandomizerBase:public AGRubyObject
{
 public:
#ifdef SWIG
  %rename(randFloat) operator()(float f);
  %rename(randInt) operator()(int i);
#endif
  virtual float operator()(float f);
  virtual int operator()(int i);

};

#endif

