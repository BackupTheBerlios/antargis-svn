%feature("director");
%include "typemaps.i"
%include "std_string.i"
%include "AGString.i"
%include "std_vector.i"
%include "std_pair.i"
%include "std_map.i"
%include "std_list.i"

%{
#include <ag_rubyobj.h>

SWIGRUNTIME VALUE
AG_NewPointerObj(void *ptr, swig_type_info *type, int flags)
{
	AGRubyObject *o=(AGRubyObject*)ptr;
	if(o->mRubyObject)
		return o->mRUBY;

	swig_type_info *t=SWIG_TypeDynamicCast(type,(void**)&o);

	o->mRubyObject=true;
	o->mRUBY=SWIG_NewPointerObj((void *) o, t,flags);
	return o->mRUBY;
}

%}

