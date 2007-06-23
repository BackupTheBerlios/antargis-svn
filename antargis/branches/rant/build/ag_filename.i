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
#include <ag_fs.h>
%}


    // Ruby wants class names to start with a capital letter
    // %rename(String) string;
//    class string;

    /* Overloading check */
    %typemap(typecheck) AGFilename = char *;
    %typemap(typecheck) const AGFilename & = char *;

    %typemap(in) AGFilename {
        if (TYPE($input) == T_STRING) {
            $1 = AGFilename(RSTRING($input)->ptr,RSTRING($input)->len);
        } else {
            SWIG_exception(SWIG_TypeError, "not a string");
        }
    }

    %typemap(in) const AGFilename & (AGFilename temp) {
        if (TYPE($input) == T_STRING) {
            temp = AGFilename(RSTRING($input)->ptr,RSTRING($input)->len);
            $1 = &temp;
        } else {
            SWIG_exception(SWIG_TypeError, "not a string");
        }
    }

    %typemap(out) AGFilename {
        $result = rb_str_new($1.c_str(),$1.length());
    }

    %typemap(out) const AGFilename & {
        $result = rb_str_new($1->c_str(),$1->length());
    }

    %typemap(directorin) AGFilename, const AGFilename &, AGFilename & "$input=rb_str_new($1_name.c_str(),$1_name.length());";

    %typemap(directorin) AGFilename *, const AGFilename * "$1_name->c_str()";
    
    %typemap(directorout) AGFilename {
        if (TYPE($input) == T_STRING)
            $result = AGFilename(RSTRING($input)->ptr,RSTRING($input)->len);
        else
            throw Swig::DirectorTypeMismatchException("string expected");
    }
    
    %typemap(directorout) const AGFilename & (AGFilename temp) {
        if (TYPE($input) == T_STRING) {
            temp = AGFilename(RSTRING($input)->ptr,RSTRING($input)->len);
            $result = &temp;
        } else {
            throw Swig::DirectorTypeMismatchException("string expected");
        }
    }
/*
    %typemap(throws) AGFilename, const string &
        "rb_raise(rb_eRuntimeError, $1.c_str());";

    %typemap(throws) string *, const string *
        "rb_raise(rb_eRuntimeError, $1->c_str());";
*/
