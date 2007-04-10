//
// SWIG typemaps for AGString
// Luigi Ballabio
// Apr 8, 2002
//
// Ruby implementation


// ------------------------------------------------------------------------
// AGString is typemapped by value
// This can prevent exporting methods which return a string
// in order for the user to modify it.
// However, I think I'll wait until someone asks for it...
// ------------------------------------------------------------------------

%include exception.i

%{
#include <ag_string.h>
%}

//namespace std {

    // Ruby wants class names to start with a capital letter
    //%rename(String) AGString;
    class AGString;

    /* Overloading check */
    %typemap(typecheck) AGString = char *;
    %typemap(typecheck) const AGString & = char *;

    %typemap(in) AGString {
        if (TYPE($input) == T_STRING) {
//            $1 = AGString(StringValuePtr($input));
            $1 = AGString(RSTRING($input)->ptr,RSTRING($input)->len);
        } else {
            SWIG_exception(SWIG_TypeError, "not a string");
        }
    }

    %typemap(in) const AGString & (AGString temp) {
        if (TYPE($input) == T_STRING) {
//            temp = AGString(StringValuePtr($input));
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

    %typemap(directorin) AGString, const AGString &, AGString & "$input=rb_str_new2($1_name.c_str());";

    %typemap(directorin) AGString *, const AGString * "$1_name->c_str()";
    
    %typemap(directorout) AGString {
        if (TYPE($input) == T_STRING)
//            $result = AGString(StringValuePtr($input));
            $result = AGString(RSTRING($input)->ptr,RSTRING($input)->len);
        else
            throw Swig::DirectorTypeMismatchException("string expected");
    }
    
    %typemap(directorout) const string & (AGString temp) {
        if (TYPE($input) == T_STRING) {
//            temp = AGString(StringValuePtr($input));
            temp = AGString(RSTRING($input)->ptr,RSTRING($input)->len);
            $result = &temp;
        } else {
            throw Swig::DirectorTypeMismatchException("string expected");
        }
    }

    %typemap(throws) AGString, const AGString &
        "rb_raise(rb_eRuntimeError, $1.c_str());";

    %typemap(throws) AGString *, const AGString *
        "rb_raise(rb_eRuntimeError, $1->c_str());";
//}
