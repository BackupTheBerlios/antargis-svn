#ifndef AGSINGLETON
#define AGSINGLETON

#include <ag_rubyobj.h>

class AGSingleton:public AGRubyObject
{
 public:
  AGSingleton();
  ~AGSingleton();
};

#endif
