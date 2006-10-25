%module(directors="1") libantargis
%feature("director");
%include "typemaps.i"

%include "nantmarker.hh"

%typemap(in) AGString () {
    if (TYPE($input) == T_STRING) {
         $1 = AGString(RSTRING($input)->ptr,RSTRING($input)->len);
     } else {
         SWIG_exception(SWIG_TypeError, "not a string");
     }
}
%typemap(in) const AGString & (AGString temp) {
    if (TYPE($input) == T_STRING) {
         temp = AGString(RSTRING($input)->ptr,RSTRING($input)->len);
         $1 = &temp;
     } else {
         SWIG_exception(SWIG_TypeError, "not a string");
     }
}

%typemap(out) const AGString & {
        $result = rb_str_new($1->c_str(),$1->length());
    }
%typemap(out) AGString {
        $result = rb_str_new($1.c_str(),$1.length());
    }




%typemap(directorin) AGString, const AGString &, AGString & "$input=rb_str_new2($1_name.c_str());";

    %typemap(directorin) AGString *, const AGString * "$1_name->c_str()";

    %typemap(directorout) AGString {
        if (TYPE($input) == T_STRING)
//            $result = AGString(StringValuePtr($input));
            $result = AGString(RSTRING($input)->ptr,RSTRING($input)->len);
        else
            throw Swig::DirectorTypeMismatchException("string expected");
    }

    %typemap(directorout) const AGString & (AGString temp) {
        if (TYPE($input) == T_STRING) {
//            temp = std::string(StringValuePtr($input));
            temp = AGString(RSTRING($input)->ptr,RSTRING($input)->len);
            $result = &temp;
        } else {
            throw Swig::DirectorTypeMismatchException("string expected");
        }
    }




%typemap(typecheck) AGString= char *;
%typemap(typecheck) const AGString &= char *;


%{
#include "scene.h"
#include "antargisgui.h"
%}
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

%template(StringVector) std::vector<std::string>;
%template(AGStringVector) std::vector<AGString>;
%template(SceneNodeVector) std::vector<SceneNodePtr>;
%template(PickResult) std::vector<PickNode>;
%template(EntityVector) std::vector<AntEntityPtr>;
%template(ResourceMap) std::map<std::string,float>;
%template(AGNodeList) std::vector<Node>;
%template(AGNodePList) std::vector<Node*>;
%template(AGMatrixVector) std::vector<AGMatrix4>;
%template(AGVector2List) std::vector<AGVector2>;

%include "antargisgui.h"


