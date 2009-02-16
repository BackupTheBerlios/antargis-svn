%{
namespace swig
{
  /*
    Traits that provides the from method for an unknown type
  */
  template <int flags, class Type> struct traits_from_ptr {
    static SWIG_Object from SWIG_FROM_DECL_ARGS(Type *val) {
    
      // insert Dynamic_cast(...)
      swig_type_info *info=type_info<Type>();
      swig_type_info *ninfo=SWIG_TypeDynamicCast(info,val);
      if(ninfo==0)
        ninfo=info;
    
      return SWIG_NewPointerObj(val, ninfo, flags);
    }
  };
}
%}