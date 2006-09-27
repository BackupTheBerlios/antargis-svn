%module(directors="1") libantargis
%feature("director");
%include "typemaps.i"

%include "nantmarker.hh"

%{
#include "scene.h"
#include "antargisgui.h"
%}
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

%template(StringVector) std::vector<std::string>;
%template(SceneNodeVector) std::vector<SceneNodePtr>;
%template(PickResult) std::vector<PickNode>;
%template(EntityVector) std::vector<AntEntityPtr>;
%template(ResourceMap) std::map<std::string,float>;
%template(AGNodeList) std::vector<Node>;
%template(AGNodePList) std::vector<Node*>;
%template(AGMatrixVector) std::vector<AGMatrix4>;
%template(AGVector2List) std::vector<AGVector2>;

%include "antargisgui.h"


