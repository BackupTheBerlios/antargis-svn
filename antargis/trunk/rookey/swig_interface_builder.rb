module Rookey
  class SwigInterfaceBuilder
    def initialize(file,moduleName,classes,files,interfaces,inits,templates)
      @file=file
      @moduleName=moduleName
      @classes=classes
      @files=files
      @interfaces=interfaces
      @inits=inits
      @templates=templates
      #@information=information
    end
    
    
    def create(classes)
      fd=File.open(@file,"w")
      fd.puts generate()
      fd.close
    end
    
    def getFiles
      @files
    end
    
    def getClasses
      Rookey.getRubyClasses(@classes)
    end
    
    private
    
    def initDefs
      @inits.map{|i|"void "+i+"();"}.join("\n")    
    end
    
    def initCalls
      @inits.map{|i|i+"();"}.join("\n")    
    end
    
    def directors
      getClasses.map{|c|"%feature(\"director\") #{c.name};"}.join("\n")
    end
    
    def templates
      @templates.map{|t|"%include \"#{t}\""}.join("\n")
      #""
    end
    
    def cpp2RubyFunctions
#      r="template<class T>
#VALUE convertCpp2Ruby(T *cObject)
#{
#  return Qnil;
#}"
#
# r2=getClasses.map{|c| a="template<>
#VALUE convertCpp2Ruby(#{c.name} *cObject)
#{
#  return SWIG_RubyInstanceFor(cObject);
#}" }.join("\n")
#     
#     "%{"+r+r2+"%}"
      
      <<EOT
      /*
template<class T>
VALUE convertCpp2Ruby(T *cObject)
{
  return SWIG_RubyInstanceFor(cObject);
}
template<AGRubyObject>*/ 
VALUE convertCpp2Ruby(AGRubyObject *cObject)
{
  return SWIG_RubyInstanceFor(cObject);
}
  
EOT
      
    end
    
    # include dyn-casts for template-containers like std::vector<SceneNode*>
    # this is missing in the dyncast-implementation of swig (at the time of 1.3.34)
    def dynFroms
      str=""
      str<<"%wrapper %{"
      str<< "namespace swig{"
      getClasses.each{|c|
        name=c.name
        str << "
        template <> struct traits_from_ptr<#{name}> {
          static VALUE from (#{name} *val,int owner=0) {
          
            // insert Dynamic_cast(...)
            swig_type_info *info=SWIGTYPE_p_#{name}; //type_info<#{name}>();
            swig_type_info *ninfo=SWIG_TypeDynamicCast(info,(void**)&val);
            if(ninfo==0)
              ninfo=info;
          
            return SWIG_NewPointerObj(val, ninfo, owner);
          }
        };        
        "
      }
      str<<"}"
      str<<"%}"
      str
    end
    
    def generate

"%module(directors=\"1\") #{@moduleName}
%feature(\"autodoc\",\"1\");

/*
%feature(\"director:except\") {
 throw Swig::DirectorMethodException($error);
}
*/

#{importInterfaces}

#{stlIncludes}
%{
#{headerIncludes}
%}

#{dynFroms}

#{directors}
#{markers}
%{
#{cpp2RubyFunctions}
%}

%{
#{initDefs}
%}
%insert(\"init\") %{
#{initCalls}
%}


%typemaps_std_string(AGString, char, SWIG_AsCharPtrAndSize, SWIG_FromCharPtrAndSize, %checkcode(STDSTRING));

/*
template <>
 inline VALUE from<AGString>(const AGString& val) {
   return SWIG_FromCharPtrAndSize(val.c_str(),val.length());
   //return traits_from<Type>::from(val);
 }
 */
 // oder
 %wrapper %{
 namespace swig
 {
 template<> struct traits_from<AGString>
 {
	 inline static VALUE from(const AGString &val)
	 {
     return SWIG_FromCharPtrAndSize(val.c_str(),val.length());
	 }
};
}
%}


#{dynamicCasts}

#{templates}

%feature(\"trackobjects\");

#{headerSwigIncludes}      

"
    end
    def importInterfaces
      @interfaces.map{|i|
        "%import \"#{i}\""
      }.join("\n")
    end
    def stlIncludes
      "%include \"stl.i\"\n%include \"std_list.i\""
    end
    def headerIncludes
      getFiles.map{|file|"#include \"#{file}\""}.join("\n")
    end
    def headerSwigIncludes
      getFiles.map{|file|"%include \"#{file}\""}.join("\n")
    end
    def markers
      getClasses.map{|c|
        "%markfunc #{c.name} \"general_markfunc\""
      }.join("\n")
    end
    def dynamicCasts
      getClasses.map{|c|
        next if c.children.length ==0
        
        "        
        %typemap(out) #{c.name} *, #{c.name} & {
          swig_type_info *ty = SWIG_TypeDynamicCast($1_descriptor,(void **) &$1);
          $result = SWIG_NewPointerObj($1, ty, $owner);
        }        
        %typemap(directorin) #{c.name} *, #{c.name} & {
          if($1)
          {
          swig_type_info *ty = SWIG_TypeDynamicCast($1_descriptor,(void **) &$1);
          $input = SWIG_NewPointerObj($1, ty, $owner);
          }
        }        
        %{
        
        swig_type_info* #{c.name}_dynamic_cast(void **p)
        {
                #{c.name} *a=(#{c.name}*)(*p);
        
                "+

                c.children.map{|child|
                "
                {
	                #{child.name}*b=dynamic_cast<#{child.name}*>(a);
	                if(b)
	                {
	                        *p=(void*)b;
	                        return SWIGTYPE_p_#{child.name};
	                }
                }
"                  
                }.join("\n")+
                "
                return 0;
        }
        
        %}
        DYNAMIC_CAST(SWIGTYPE_p_#{c.name}, #{c.name}_dynamic_cast);"
      }.join("\n")
    end
  end  
end
