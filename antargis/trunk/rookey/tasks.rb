require 'rake/clean.rb'
require File.join(File.split(__FILE__)[0],'rookey.rb')
require File.join(File.split(__FILE__)[0],'swig.rb')
require File.join(File.split(__FILE__)[0],'compile.rb')


require File.join(File.split(__FILE__)[0],'rookey_config.rb')
Rookey::Configure.load
Rookey::Configure.cleanTask

require File.join(File.split(__FILE__)[0],'config_generator.rb')
require File.join(File.split(__FILE__)[0],'cpp_hierarchy_parser.rb')
require File.join(File.split(__FILE__)[0],'swig_interface_builder.rb')

require 'rake/clean.rb'
require 'pp'

# For using Rookey you'll create a Rakefile and include rookey, e.g. like this:
#  require 'rookey/rookey.rb'
# After this you're free to use the public methods of the Rookey module to define your
# projects libraries, sources and so forth.
#
# All tasks are designed so that you can take their output as a new target, e.g.:
#
module Rookey

  # call like this
  # swig_interface "antargis_gui"=>Dir["ext/gui/*.h"]+additionalInterfaces,"initalizers"=>["func1","func2",...]
  # will generate an interface-file named "antargis_gui_interface.i" for module "antargis_gui"
  def Rookey.swig_interface(ops)
    target=""
    source=""
    inits=[]
    ops.each{|n,v|
      case n
        when :initializers
          inits=v
        else
		      target=n
		      source=v
      end
    }
    file=target+"_interface.i"
    templates=[source].flatten.select{|f|f=~/i$/}
    source=source.grep(/INCLUDE_SWIG/)+[getAGRubyObjectHeader]
    
    rule file=>source do
	    parser=CppHierarchyParser.new([source].flatten.select{|f|f=~/h$/})
	    classes=parser.getAllClasses
	    
	    files=(getOrderedCppHeaders(classes)+source).uniq
	    interfaceBuilder=SwigInterfaceBuilder.new(file,target,classes,files,source.select{|f|f=~/i$/},inits,templates)
	    interfaceBuilder.create(parser)
    end
    CLEAN << file
    [file]
  end
  
  def Rookey.swig(ops,config=nil)
	  target=""
	  source=""
	  ops.each{|n,v|
	    target=n
	    source=v
	  }
	  swigDirect(target,source,config)
	end
	
	def Rookey.swigDirect(output,interface,config=nil)
	  config||=Rookey::getConfig
	  swigCompiler=Rookey::Swig.new(config)
	  
	  
	  # FIXME: handle deps
	  
	  rule output=>Swig.getDeps(interface) do |t|
	    swigCompiler.swig(t,interface)
	  end
	  CLEAN << output << output.gsub(/\.cc?/,".h")
	  [output,getAGRubyObjectSource]
	end
  def Rookey.getAGRubyObjectSource
    File.join(File.split(__FILE__)[0],"cpp","ag_rubyobj.cc")
  end
  def Rookey.getAGRubyObjectHeader
    File.join(File.split(__FILE__)[0],"cpp","ag_rubyobj.h")
  end
	
	def Rookey.compile(files,config=nil)
	  files=[files].flatten
	  config||=Rookey::getConfig
	  compiler=Rookey::Compiler.new(config)
	  files.each{|file|
	    t=compiler.makeObject(file)
      s=Compiler.getDeps(file)
	    rule t=>s do |t|
	      compiler.compile(t.name,t.prerequisites)
	    end
	  }
	  targets=files.map{|file|compiler.makeObject(file)}
	  CLEAN << targets << compiler.getBuildDir << Compiler.getDepsDir
	  targets
	end
	
	def Rookey.link_dll(name,files)
    files.flatten!
    files.uniq!
	  config=Rookey::getConfig
	  
    linker=Rookey::Compiler.new(config)
    
    target=linker.dllName(name)
    
    desc "Link DLL #{name}"
    task target=>files do |t|
      linker.linkDLL(t)
    end
    CLEAN << target
    
    target
	end

  def Rookey.link_exe(name,files,config=nil)
    files.flatten!
    files << compile(File.join(File.split(__FILE__)[0],"cpp","ag_rubyobj.cc"),config)
    files << compile(File.join(File.split(__FILE__)[0],"cpp","swig_dummy.cc"),config) if files.select{|f|f=~/swig/}.length==0
    files.uniq!
    config||=Rookey::getConfig
    
    linker=Rookey::Compiler.new(config)
    
    target=linker.exeName(name)
    desc "Link EXE #{name}"
    task target=>files do |t|
      linker.linkEXE(t.name,t.prerequisites)
    end
    CLEAN << target
    
    target    
  end 

  
  def Rookey.ruby_ext(name,files,inits)
    headerFiles=files.select{|f|f=~/h$/}
    libs=files.select{|f|f=~/so$/ or f=~/bundle/}
    swigInterfaces=libs.map{|f|f.sub(/\.[a-zA-Z]+$/,".i")}
    swigInterfaces=files.select{|f|f=~/i$/}
    swigInterface = Rookey::swig_interface(name=>headerFiles+swigInterfaces,:initializers=>inits)
    cppFiles=files.select{|f|f=~/c$/}

    sourceDirs=files.map{|file|File.split(file)[0]}.sort.uniq
    
    config=Rookey::getConfig
    sourceDirs << "."
    config.add("INCLUDEDIRS",sourceDirs.join(" "))
    
    compiler=Rookey::Compiler.new(config)
        
    swigTarget = Rookey::swig({File.join(compiler.getPlainBuildDir,name+"_swig.cc")=>swigInterface},config)
    
    
    targets = Rookey::compile(cppFiles+[swigTarget],config)
    alllibs=libs
    
    lib=Rookey::link_dll(name,targets+alllibs)
  end
  
  def Rookey.checkedRequire(name)
    begin
    require name
    rescue LoadError => load_error
      puts "Library #{name} not found!"
      return false
    end
    return true
  end
  
end