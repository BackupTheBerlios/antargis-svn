%template(StdVectorAGRect2) std::vector<AGRect2>;
%template(StdVectorAGVector2) std::vector<AGVector2>;
%template(StdVectorAGVector3) std::vector<AGVector3>;
%template(StdVectorAGVector4) std::vector<AGVector4>;


%template(AGRect2List) std::list<AGRect2>;
%template(AGMatrixVector) std::vector<AGMatrix4>;

%include "ag_algebra.h"

%template(determinantAGMatrix3) determinant<AGMatrix3>;
%template(determinantAGMatrix4) determinant<AGMatrix4>;
%template(determinantAGMatrixN) determinant<AGMatrixN>;