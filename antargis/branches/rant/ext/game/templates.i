%{
AntEntity *SWIG_RB2AntEntity(VALUE x) {
	AntEntity *e=0;
	Data_Get_Struct(x,AntEntity,e);
	return 0;
}
VALUE SWIG_AntEntity2RB(AntEntity*e ) {
	return AG_NewPointerObj(e,SWIGTYPE_p_AntEntity,0);
}
bool SWIG_ANTENTITY_P(VALUE x) {
	return true; // FIXME ???
    //return TYPE(x) == T_STRING;
}

%}

namespace std
{
	specialize_std_vector(AntEntity*,SWIG_ANTENTITY_P,SWIG_RB2AntEntity,SWIG_AntEntity2RB);
	specialize_std_list(AntEntity*,SWIG_ANTENTITY_P,SWIG_RB2AntEntity,SWIG_AntEntity2RB);
}


%template(EntityPVector) std::vector<AntEntity*>;
%template(EntityPList) std::list<AntEntity*>;
%template(EntityVector) std::vector<AntEntityPtr>;
%template(ResourceMap) std::map<std::string,float>;
%template(AGResourceMap) std::map<AGString,float>;
