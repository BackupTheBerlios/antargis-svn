%feature("director");
%include "typemaps.i"
%include "std_string.i"
%include "AGString.i"
%include "std_vector.i"
%include "std_pair.i"
%include "std_map.i"
%include "std_list.i"
//%include "ag_string.i"
%include "ag_filename.i"
%include "ag_data.i"

%{
#include <ag_rubyobj.h>

SWIGRUNTIME VALUE
AG_NewPointerObj(void *ptr, swig_type_info *type, int flags)
{
	AGRubyObject *o=(AGRubyObject*)ptr;
	printf("AG_NewPointerObj o:0x%lx , ruby:0x%lx vl:0x%lx\n",o,o->mRubyObject,o->mRUBY);
	if(o->mRubyObject)
		return o->mRUBY;

	swig_type_info *t=SWIG_TypeDynamicCast(type,(void**)&o);

	o->mRubyObject=true;
	o->mRUBY=SWIG_NewPointerObj((void *) o, t,flags);
	printf("NEW AG_NewPointerObj o:0x%lx , ruby:0x%lx vl:0x%lx\n",o,o->mRubyObject,o->mRUBY);
	return o->mRUBY;
}

%}

