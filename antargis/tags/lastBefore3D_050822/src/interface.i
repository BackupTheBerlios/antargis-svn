%module(directors="1") libantargis
%feature("director");
%include "typemaps.i"

/* documents how to return a self-defined class

%typemap(directorout) AGButton {
	AGButton *b;
	Data_Get_Struct($input,AGButton,b);
	$result=*b;
}
*/
%typemap(directorout) AGSurface {
	std::cout<<"test1"<<std::endl;
	AGSurface *b;
	Data_Get_Struct($input,AGSurface,b);
	$result=*b;
}
%include "marker.i"

%include "antmark.h"


%exception AGApplication::AGApplication {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
	printf("register:AGApplication\n");
}
%markfunc AGApplication "AGApplication_markfunc"




%{
#include "antargisgui.h"
#include "antargis.h"
%}
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

%template(StringVector) std::vector<std::string>;
%template(EntityVector) std::vector<AntEntityPtr>;
%template(ResourceMap) std::map<std::string,int>;
%include "antargisgui.h"
%include "antargis.h"


