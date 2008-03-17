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
#include <string>
%}

namespace std {

    // Ruby wants class names to start with a capital letter
    %rename(String) string;
    class string;

    /* Overloading check */
    %typemap(typecheck) string = char *;
    %typemap(typecheck) const string & = char *;

    %typemap(in) string {
        if (TYPE($input) == T_STRING) {
//            $1 = std::string(StringValuePtr($input));
            $1 = std::string(RSTRING($input)->ptr,RSTRING($input)->len);
        } else {
            SWIG_exception(SWIG_TypeError, "not a string");
        }
    }

    %typemap(in) const string & (std::string temp) {
        if (TYPE($input) == T_STRING) {
//            temp = std::string(StringValuePtr($input));
            temp = std::string(RSTRING($input)->ptr,RSTRING($input)->len);
            $1 = &temp;
        } else {
            SWIG_exception(SWIG_TypeError, "not a string");
        }
    }

    %typemap(out) string {
        $result = rb_str_new($1.c_str(),$1.length());
    }

    %typemap(out) const string & {
        $result = rb_str_new($1->c_str(),$1.length());
    }

    %typemap(directorin) string, const string &, string & "$input=rb_str_new2($1_name.c_str());";

    %typemap(directorin) string *, const string * "$1_name->c_str()";
    
    %typemap(directorout) string {
        if (TYPE($input) == T_STRING)
//            $result = std::string(StringValuePtr($input));
            $result = std::string(RSTRING($input)->ptr,RSTRING($input)->len);
        else
            throw Swig::DirectorTypeMismatchException("string expected");
    }
    
    %typemap(directorout) const string & (std::string temp) {
        if (TYPE($input) == T_STRING) {
//            temp = std::string(StringValuePtr($input));
            temp = std::string(RSTRING($input)->ptr,RSTRING($input)->len);
            $result = &temp;
        } else {
            throw Swig::DirectorTypeMismatchException("string expected");
        }
    }

    %typemap(throws) string, const string &
        "rb_raise(rb_eRuntimeError, $1.c_str());";

    %typemap(throws) string *, const string *
        "rb_raise(rb_eRuntimeError, $1->c_str());";
}

%{
int SWIG_AsVal_std_string(VALUE x,std::string *s) {
    s=new std::string(RSTRING_PTR(x), RSTRING_LEN(x));
return 0;
}
VALUE SWIG_From_std_string(const std::string& s) {
    return rb_str_new(s.data(), s.size());
}
%}
