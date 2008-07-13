module Rookey
  class Compiler
    def initialize(config)
      @config=config
    end
  
	  def Compiler.getDeps(t)
	    # FIXME
	    
	    parseDepsFiles
      
      return t unless @@deps
      p=File.join(Compiler.getDepsDir,t)
	    return @@deps[p] if @@deps[p]
      t
	  end
  
	  def compile(target,sources)
	    source=makeSource(target)
	    
      # FIXME check for c/c++
      
	    program=getCompiler(:cpp)

	    
	    options=[]
	
      pp @config
      
	    options << @config["CFLAGS"]
	    
	    includeDirs = @config["INCLUDEDIRS"].split(" ")
	    includes=includeDirs.map{|d|"-I"+d}
	    options+=includes
	    
      expanded_path=File.expand_path(source)
      sourcepath=source
      sourcepath=expanded_path if File.exists?(expanded_path) and not File.exists?(source)
      sourcepath="/"+sourcepath if File.exists?("/"+sourcepath) and not File.exists?(sourcepath)
      #pp sourcepath,source
      #exit
	    options << "-c "+sourcepath
	    options << "-o "+target
      
      
      depfile=File.join(Compiler.getDepsDir,source)
      depdir=File.split(depfile)[0]
      Rookey.mkdir(depdir)
      
      outdir=File.split(target)[0]      
      Rookey.mkdir(outdir)

      options << "-Wp,-MD,#{depfile}"
	            
	    call=program+" "+options.join(" ")
	    sh call
      puts
	  
	  end
    
    def linkEXE(name,objects)
      program=getCompiler(:cpp)
      options=[]
      options << "-o "+name
      options += objects.uniq
      options << @config["LDFLAGS"] 
      call=program+" "+options.join(" ")
      sh call
      puts
    end
      
	  
	  def linkDLL(t)
	    programcall=getCompiler(:cpp)+" " +@config["LDSHAREDFLAGS"]
      
      options=[]
      pp t.methods.sort
      pp t.prerequisites
      
      options << "-o "+t.name
      options += t.prerequisites
      options << @config["LDFLAGS"] 
      options << @config["LIBS"]
      options << @config["LIBRUBYARG"] 
      call=programcall+" "+options.join(" ")
      sh call
      puts
	  end
	  def linkSO(t)
	  end
	  def linkA(t)
	  end
	  
	  def dllName(base)
	    base+"."+@config["DLEXT"]
	  end
	  
    def exeName(base)
      if @config["host_os"]=~/win/ and not @config["host_os"]=~/darwin/
        base+".exe"
      else
        base
      end
    end
    
    # FIXME: include build-dirs
	  def makeObject(sourceName)
	    file=File.join(getBuildDir,sourceName+".o")
      dir=File.split(file)[0]
      begin
#        pp @config
#        pp "DIR:",dir
        #if dir
        Rookey.mkdir(dir) if dir.length>0
      rescue Object
        pp $!
      end
      file
	  end
	  
	  def makeSource(objectName)
	    objectName.sub(/\.o$/,"").sub(/^#{getBuildDir}[\/\\]/,"")
	  end
	  
    def getPlainBuildDir
      ".build_"+@config["host_os"]
    end
    
	  def getBuildDir
	    File.join(Dir.pwd,getPlainBuildDir)
	  end
    def Compiler.getDepsDir
      File.join(Dir.pwd,".deps")
    end
        
    private
    def getCompiler(type)
       #FiXME
      compiler= case type
        when :cpp
          @config["CPP"]
        when :c
          @config["CC"]
        else
          raise "Unknown compiler type #{type}"
      end
      ccache=@config["CCACHE"]
      compiler=ccache+" "+compiler if ccache
      compiler
    end
    
    def findProgram(pattern)
      
    end
    
    @@deps=nil
    def Compiler.parseDepsFiles
      return @@deps if @@deps
            
      files=Dir[File.join(getDepsDir,"**","*")]
      @@deps={}
      files.each{|file|
        unless File.directory?(file)
	        fd=File.open(file)
	        
	        @@deps[file]=fd.read.gsub("\\\n","").split(" ")[1..-1]
	        fd.close
        end
      }
    end
  end
  
  
end