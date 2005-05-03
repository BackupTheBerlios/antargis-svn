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
%include "marker.h"

%include "antmark.h"




%{
#include "antargisgui.h"
#include "antargis.h"
%}
%include "std_string.i"
%include "std_vector.i"
%template(StringVector) std::vector<std::string>;
//%template(EntityVector) std::vector<AntEntity*>;
%include "antargisgui.h"
%include "antargis.h"


