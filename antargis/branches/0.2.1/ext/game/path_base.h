#ifndef PATH_BASE_H
#define PATH_BASE_H

// INCLUDE_SWIG - used to filter, which files are included in swig-interfacing

#include <ag_geometry.h>
#include <ag_rubyobj.h>

class AGEXPORT AGPathBase:public AGRubyObject
{
 public:
  virtual ~AGPathBase();
  virtual float weight(const AGVector2 &a,const AGVector2 &b);
};

#endif
