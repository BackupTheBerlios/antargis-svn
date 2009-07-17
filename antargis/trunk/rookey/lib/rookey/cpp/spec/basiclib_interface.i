%module(directors="1") basiclib
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
#include "/Users/davidkamphausen/projects.alt/antargis/rookey/lib/./rookey/cpp/rk_rubyobj.h"
#include "/Users/davidkamphausen/projects.alt/antargis/rookey/lib/rookey/cpp/rk_logging.h"
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
        }%}

%feature("director") AGRubyObject;
%markfunc AGRubyObject "general_markfunc"
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



%template(StringVector) std::vector<std::string>;
%template(PStringVector) std::vector<std::string*>;


%feature("trackobjects");

%include "/Users/davidkamphausen/projects.alt/antargis/rookey/lib/./rookey/cpp/rk_rubyobj.h"
%include "/Users/davidkamphausen/projects.alt/antargis/rookey/lib/rookey/cpp/rk_logging.h"      

