#include "rk_rubyobj.h"

// The following keyword is used to define that this header-file is included into swig-based
// export to ruby
// INCLUDE_SWIG

// AGRubyObject is needed for any object that's owned cross-language.
// A good rule is that any class thats derived from should be derived from AGRubyObject
// in the first place.
class A:public AGRubyObject
  {
public:
  virtual std::string x();

  };
