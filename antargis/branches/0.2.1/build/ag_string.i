//
// SWIG typemaps for std::string
// Luigi Ballabio
// Apr 8, 2002
//
// Ruby implementation


// ------------------------------------------------------------------------
// std::string is typemapped by value
// This can prevent exporting methods which return a string
// in order for the user to modify it.
// However, I think I'll wait until someone asks for it...
// ------------------------------------------------------------------------

%include exception.i

%{
#include <ag_string.h>
%}


    // Ruby wants class names to start with a capital letter
%rename(String) AGString;
class AGString;

    /* Overloading check */
    %typemap(typecheck) AGString = char *;
    %typemap(typecheck) const AGString & = char *;

    %typemap(in) AGString {
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

    %typemap(out) AGString {
        $result = rb_str_new($1.c_str(),$1.length());
    }

    %typemap(out) const AGString & {
        $result = rb_str_new($1->c_str(),$1->length());
    }

    %typemap(directorin) AGString, const AGString &, AGString & "$input=rb_str_new($1_name.c_str(),$1_name.length());";

    %typemap(directorin) AGString *, const AGString * "$1_name->c_str()";
    
    %typemap(directorout) AGString {
        if (TYPE($input) == T_STRING)
            $result = AGString(RSTRING($input)->ptr,RSTRING($input)->len);
        else
            throw Swig::DirectorTypeMismatchException("string expected");
    }
    
    %typemap(directorout) const AGString & (AGString temp) {
        if (TYPE($input) == T_STRING) {
            temp = AGString(RSTRING($input)->ptr,RSTRING($input)->len);
            $result = &temp;
        } else {
            throw Swig::DirectorTypeMismatchException("string expected");
        }
    }
/*
    %typemap(throws) AGString, const string &
        "rb_raise(rb_eRuntimeError, $1.c_str());";

    %typemap(throws) string *, const string *
        "rb_raise(rb_eRuntimeError, $1->c_str());";
*/

%{
AGString SWIG_RB2AGSTR(VALUE x) {
    return AGString(RSTRING_PTR(x), RSTRING_LEN(x));
}
VALUE SWIG_AGSTR2RB(const AGString& s) {
    return rb_str_new(s.data(), s.size());
}
%}
namespace std
{
	specialize_std_vector(AGString,SWIG_STRING_P,SWIG_RB2AGSTR,SWIG_AGSTR2RB);
	specialize_std_vector(AGFilename,SWIG_STRING_P,SWIG_RB2AGSTR,SWIG_AGSTR2RB);
	specialize_std_vector(AGData,SWIG_STRING_P,SWIG_RB2AGSTR,SWIG_AGSTR2RB);
}