#ifndef AG_VDEBUG
#define AG_VDEBUG

#include <ag_debug.h>

void agAssertGL(std::string s);
#define assertGL agAssertGL(::toString(__FILE__)+::toString(" ")+::toString(__LINE__)+::toString(" ")+::toString( __PRETTY_FUNCTION__))


#endif
