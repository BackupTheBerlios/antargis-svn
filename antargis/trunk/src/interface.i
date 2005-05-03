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

%markfunc AntargisMap "AntargisMap_markfunc"
%exception AntargisMap::AntargisMap {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }

%markfunc AntEntity "AntEntity_markfunc"
%exception AntEntity::AntEntity {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }
%markfunc AntHouse "AntEntity_markfunc"
%exception AntHouse::AntHouse {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }
%markfunc AntMan "AntEntity_markfunc"
%exception AntMan::AntMan {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }

%markfunc AntHero "AntEntity_markfunc"
%exception AntHero::AntHero {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }
%exception Job::Job {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }
%exception MoveJob::MoveJob {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }
%exception FightJob::FightJob {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }
%exception FetchJob::FetchJob {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }
%exception RestJob::RestJob {
        $action
        result->mRUBY = self;
	result->mRubyObject=true;
	//printf("setting mruby:widget\n");
    }


%markfunc AntargisMap "AntargisMap_markfunc";
%{
#include "antargis.h"
%}
%include "std_string.i"
%include "antargis.h"


