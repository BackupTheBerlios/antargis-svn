%module(directors="1") libantargisgui
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

%exception AGApplication::AGApplication {
	$action
	result->mRUBY=self;
  result->mRubyObject=true;
	printf("register:AGApplication\n");
}
%markfunc AGApplication "AGApplication_markfunc"


/*
%exception AGWidget::AGWidget {
        $action
        result->mRUBY = self;
	printf("setting mruby:widget\n");
    }
%exception AGScreen::AGScreen {
        $action
        result->mRUBY = self;
        printf("setting\n");
}
%exception AGWindow::AGWindow {
	$action
	result->mRUBY = self;
	printf("setting\n");
}*/


/*%markfunc AGWidget "AGWidget_markfunc";
%markfunc AGLayout "AGWidget_markfunc";*/
%{
#include "antargisgui.h"
%}
%include "std_string.i"
%include "std_vector.i"
%template(StringVector) std::vector<std::string>;
%template(LineVector) std::vector<AGLine>;
%include "antargisgui.h"


