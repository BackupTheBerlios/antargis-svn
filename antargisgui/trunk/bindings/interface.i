%module(directors="1") libantargisruby
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


%markfunc AGWidget "AGWidget_markfunc";
%{
#include "antargisgui.h"
%}
%include "std_string.i"
%include "antargisgui.h"


