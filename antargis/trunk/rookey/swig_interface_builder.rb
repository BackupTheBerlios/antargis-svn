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
    
    def generate

"%module(directors=\"1\") #{@moduleName}
%feature(\"autodoc\",\"1\");

#{importInterfaces}

#{stlIncludes}
%{
#{headerIncludes}
%}

#{directors}
#{markers}
%{
#{cpp2RubyFunctions}
%}
#{templates}

%{
#{initDefs}
%}
%insert(\"init\") %{
#{initCalls}
%}


%typemaps_std_string(AGString, char, SWIG_AsCharPtrAndSize, SWIG_FromCharPtrAndSize, %checkcode(STDSTRING));

#{dynamicCasts}

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
