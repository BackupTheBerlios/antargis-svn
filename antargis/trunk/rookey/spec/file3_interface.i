%module(directors="1") file3
%feature("autodoc","1");



%include "stl.i"
%include "std_list.i"
%{
#include "../cpp/ag_rubyobj.h"
#include "a.h"
%}

%feature("director") AGRubyObject;
%feature("director") B;
%markfunc AGRubyObject "general_markfunc"
%markfunc B "general_markfunc"
%{
      /*
template<class T>
VALUE convertCpp2Ruby(T *cObject)
{
  return SWIG_RubyInstanceFor(cObject);
}
template<AGRubyObject>*/ 
VALUE convertCpp2Ruby(AGRubyObject *cObject)
{
  return SWIG_RubyInstanceFor(cObject);
}
  

%}


%{

%}
%insert("init") %{

%}


%typemaps_std_string(AGString, char, SWIG_AsCharPtrAndSize, SWIG_FromCharPtrAndSize, %checkcode(STDSTRING));

        
        %typemap(out) AGRubyObject *, AGRubyObject & {
          swig_type_info *ty = SWIG_TypeDynamicCast($1_descriptor,(void **) &$1);
          $result = SWIG_NewPointerObj($1, ty, $owner);
        }        
        %{
        
        swig_type_info* AGRubyObject_dynamic_cast(void **p)
        {
                AGRubyObject *a=(AGRubyObject*)(*p);
        
                
                {
	                B*b=dynamic_cast<B*>(a);
	                if(b)
	                {
	                        *p=(void*)b;
	                        return SWIGTYPE_p_B;
	                }
                }

                return 0;
        }
        
        %}
        DYNAMIC_CAST(SWIGTYPE_p_AGRubyObject, AGRubyObject_dynamic_cast);


%feature("trackobjects");

%include "../cpp/ag_rubyobj.h"
%include "a.h"      

