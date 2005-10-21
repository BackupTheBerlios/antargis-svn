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

%typemap(out) SceneNode *{
//	std::cout<<"test2"<<std::endl;
	if($1)
	{
		if($1->mRubyObject)
		{
          		$result=$1->mRUBY;
//			std::cout<<"found :"<<$result<<std::endl;
		}
		else
		{
			
			if(dynamic_cast<Mesh*>(result))
				vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Mesh,0);
			else if(dynamic_cast<TerrainMesh*>(result))
				vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_TerrainMesh,0);
			else
				vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_SceneNode,0);


//		        vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Mesh,0);
			//$1->mRubyObject=true;
			//$1->mRUBY=vresult;
//			std::cout<<"not found - but set:"<<vresult<<std::endl;
		}
	}
	else
	{
		vresult = Qnil;
		std::cout<<"nilled"<<std::endl;
	}
}
%typemap(out) Mesh *{
//	std::cout<<"test2"<<std::endl;
	if($1)
	{
		if($1->mRubyObject)
          		$result=$1->mRUBY;
		else
		{
		        vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_Mesh,0);
//			$1->mRubyObject=true;
//			$1->mRUBY=vresult;
		}
	}
	else
		vresult = Qnil;
}
%typemap(out) AntEntity *{
//	std::cout<<"test2"<<std::endl;
	if($1)
	{
		if($1->mRubyObject)
          		$result=$1->mRUBY;
		else
		{
		        vresult = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_AntEntity,0);
//			$1->mRubyObject=true;
//			$1->mRUBY=vresult;
			throw int();
		}
	}
	else
		vresult = Qnil;
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
%template(SceneNodeVector) std::vector<SceneNodePtr>;
%template(PickResult) std::vector<PickNode>;
%template(EntityVector) std::vector<AntEntityPtr>;
%template(ResourceMap) std::map<std::string,float>;
%include "antargisgui.h"
%include "antargis.h"


