#ifndef AGSINGLETON
#define AGSINGLETON

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include <ag_rubyobj.h>

class AGSingleton:public AGRubyObject
{
 public:
  AGSingleton();
  ~AGSingleton();
};

#endif
