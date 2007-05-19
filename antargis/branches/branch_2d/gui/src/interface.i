%module(directors="1") libantargisgui
%feature("director");
%include "typemaps.i"

// documents how to return a self-defined class

/*
%typemap(directorout) AGSurface {
	std::cout<<"test1"<<std::endl;
	AGSurface *b;
	Data_Get_Struct($input,AGSurface,b);
	$result=*b;
}*/

%include "nantmarker.hh"

/*
%exception AGApplication::AGApplication {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
	printf("register:AGApplication\n");
}
%markfunc AGApplication "AGApplication_markfunc"
*/

%{
#include "antargisgui.h"
%}
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_list.i"

%template(StringVector) std::vector<std::string>;
%template(NodeVector) std::vector<Node*>;

%include "antargisgui.h"



