def interface_template(moduleName,files,swigInput,addfiles=[])
<<EOT
%module(directors="1") #{moduleName}

// include common (stdc++) thingies and set options
%include "build/common.i"

%{
	void AG_Init_lib#{moduleName}();
%}
%insert("init") %{
	#{swigInput.collect{|i|"rb_require(\"antargis#{i.split("/")[-2]+".so"}\");"}.join("\n")}
	AG_Init_lib#{moduleName}(); 
%}

#{swigInput.collect{|i|"%import \"#{i}\""}.join("\n")}

%{
#{addfiles.collect{|f|"#include \"#{f}\""}.join("\n")}
#{files.collect{|f|"#include \"#{f}\""}.join("\n")}
%}
#{files.collect{|f|"%include \"#{f}\""}.join("\n")}


// FIXME: add templates !
EOT
end



def interface_templateBAK(moduleName,files)
<<EOT
%module(directors="1") #{moduleName}
//%include "build/common.i"








%feature("director");
%include "typemaps.i"
%include "std_string.i"
%include "AGString.i"
%include "std_vector.i"
%include "std_pair.i"
%include "std_map.i"
%include "std_list.i"

%{
#include <ag_rubyobj.h>

SWIGRUNTIME VALUE
AG_NewPointerObj(void *ptr, swig_type_info *type, int flags)
{
	AGRubyObject *o=(AGRubyObject*)ptr;
	if(o->mRubyObject)
		return o->mRUBY;

	swig_type_info *t=SWIG_TypeDynamicCast(type,(void**)&o);

	o->mRubyObject=true;
	o->mRUBY=SWIG_NewPointerObj((void *) o, t,flags);
	return o->mRUBY;
}

%}



%{
#include "headers.hh"
%}
%include "marker.i"
%{
#include "headers.hh"
%}
/*
%template(StringVector) std::vector<std::string>;
%template(AGStringVector) std::vector<AGString>;
%template(SceneNodeVector) std::vector<SceneNodePtr>;
%template(PickResult) std::vector<PickNode>;
%template(EntityVector) std::vector<AntEntityPtr>;
%template(ResourceMap) std::map<std::string,float>;
%template(AGResourceMap) std::map<AGString,float>;
%template(AGNodeList) std::vector<Node>;
%template(AGNodePList) std::vector<Node*>;
%template(AGMatrixVector) std::vector<AGMatrix4>;
%template(AGVector2List) std::vector<AGVector2>;
%template(AGPairVec2Surface) std::pair<AGVector2,AGSurface*>;
%template(AGRect2Vector) std::vector<AGRect2>;
%template(AGRect2List) std::list<AGRect2>;
%template(SceneNodeList) std::list<SceneNode*>;
*/
EOT
end
