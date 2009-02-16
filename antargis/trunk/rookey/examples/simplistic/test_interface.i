%module(directors="1") test
%feature("autodoc","1");

/*
%feature("director:except") {
 throw Swig::DirectorMethodException($error);
}
*/

      %include <std_vector.i>
      %include <std_list.i>
      


%include "stl.i"
%include "std_list.i"
%{
#include "rk_string.h"
#include "/Library/Ruby/Gems/1.8/gems/rookey-0.1/lib/./rookey/cpp/rk_rubyobj.h"
#include "a.h"
%}

%wrapper %{namespace swig{
        template <> struct traits_from_ptr<AGRubyObject> {
          static VALUE from (AGRubyObject *val,int owner=0) {
          
            // insert Dynamic_cast(...)
            swig_type_info *info=SWIGTYPE_p_AGRubyObject; //type_info<AGRubyObject>();
            swig_type_info *ninfo=SWIG_TypeDynamicCast(info,(void**)&val);
            if(ninfo==0)
              ninfo=info;
          
            return SWIG_NewPointerObj(val, ninfo, owner);
          }
        };        
        
        template <> struct traits_from_ptr<A> {
          static VALUE from (A *val,int owner=0) {
          
            // insert Dynamic_cast(...)
            swig_type_info *info=SWIGTYPE_p_A; //type_info<A>();
            swig_type_info *ninfo=SWIG_TypeDynamicCast(info,(void**)&val);
            if(ninfo==0)
              ninfo=info;
          
            return SWIG_NewPointerObj(val, ninfo, owner);
          }
        };        
        }%}

%feature("director") AGRubyObject;
%feature("director") A;
%markfunc AGRubyObject "general_markfunc"
%markfunc A "general_markfunc"
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

/*
template <>
 inline VALUE from<AGString>(const AGString& val) {
   return SWIG_FromCharPtrAndSize(val.c_str(),val.length());
   //return traits_from<Type>::from(val);
 }
 */
 // oder
 %wrapper %{
 namespace swig
 {
 template<> struct traits_from<AGString>
 {
	 inline static VALUE from(const AGString &val)
	 {
     return SWIG_FromCharPtrAndSize(val.c_str(),val.length());
	 }
};
}
%}


        
        %typemap(out) AGRubyObject *, AGRubyObject & {
          swig_type_info *ty = SWIG_TypeDynamicCast($1_descriptor,(void **) &$1);
          $result = SWIG_NewPointerObj($1, ty, $owner);
        }        
        %typemap(directorin) AGRubyObject *, AGRubyObject & {
          if($1)
          {
          swig_type_info *ty = SWIG_TypeDynamicCast($1_descriptor,(void **) &$1);
          $input = SWIG_NewPointerObj($1, ty, $owner);
          }
        }        
        %{
        
        swig_type_info* AGRubyObject_dynamic_cast(void **p)
        {
                AGRubyObject *a=(AGRubyObject*)(*p);
        
                
                {
	                A*b=dynamic_cast<A*>(a);
	                if(b)
	                {
	                        *p=(void*)b;
	                        return SWIGTYPE_p_A;
	                }
                }

                return 0;
        }
        
        %}
        DYNAMIC_CAST(SWIGTYPE_p_AGRubyObject, AGRubyObject_dynamic_cast);

%template(StringVector) std::vector<std::string>;
%template(PStringVector) std::vector<std::string*>;


%feature("trackobjects");

%include "/Library/Ruby/Gems/1.8/gems/rookey-0.1/lib/./rookey/cpp/rk_rubyobj.h"
%include "a.h"      

