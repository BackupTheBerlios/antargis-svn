%include <SDL_keysym.h>

%template(StdVectorPNode) std::vector<Node*>;
%template(AGNodeList) std::vector<Node>;

//%template(StringVector) std::vector<std::string>;
%template(AGStringVector) std::vector<AGString>;
%template(AGStringUtf8Vector) std::vector<AGStringUtf8>;
%template(AGVector2List) std::list<AGVector2>;


%template(AGStringMap) std::map<AGString,AGString>;