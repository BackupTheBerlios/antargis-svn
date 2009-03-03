
require File.join(File.split(__FILE__)[0],'config_class.rb')

class Class
  def provides(s=nil)
    @provides||=[]
    @provides << s if s
    @provides
  end
  def needs(s=nil)
    @needs||=[]
    @needs << s if s
    @needs
  end     
end

module Rookey
  
  
  CONFIG_FILENAME=File.join(Compiler.rookeyWorkingDir,"config_cache.rb")
  @@configured=[]
  
  # stores a dumpable ruby-object *what* into a file named *filename*
  def Rookey.hibe(what,filename)
    content=Marshal.dump(what).gsub("\"","\\\"")
    mkdir(File.split(filename)[0])
    File.open(filename,"w") do |f|
      f.puts <<EOT
module Rookey
  @@config=Marshal.load("#{content}")
end
EOT
    end
  end
  
  class Configurator
    def initialize
    end
    def run(config)
    end
    
    def installLibDir
      File.join(File.split(__FILE__)[0],"externals","build","lib")
    end
    def installIncludeDir
      File.join(File.split(__FILE__)[0],"externals","build","include")
    end
    
    def searchProgram(program)
      searchPrograms(program)[0]
    end
    
    def searchPrograms(program)
      programs=[program].flatten
      result=programs.map{|program|
	      getPath.map{|dir|
	        
	        p=File.join(dir,program)
          list=Dir[p]
	        p=nil unless File.exists?(p)
          p||=list[0]
          p
	      }
      }.flatten.uniq-[nil]
      pp result
      result
    end
    def checkProgram(program)      
    end
    def checkLibrary(config,lib,funcname,includes=[])
      includes+=config["NEEDED_INCLUDES"].split(" ")
      includes=includes.map{|i|"#include <#{i}>"}.join("\n")
      source="#{includes}\nextern \"C\" void #{funcname}();int main(int argc,char*argv[]){return 0;}"
      pp source
      Rookey::mkdir(Compiler.rookeyTestDir)
      testSource=File.join(Compiler.rookeyTestDir,"test.c")
      fd=File.open(testSource,"w")
      fd.puts source
      fd.close
      compiler=Compiler.new(config)
      target=compiler.makeObject(testSource)
      compiler.compile(target,testSource)
      libadd="-l#{lib}"
      exeName=File.join(Compiler.rookeyTestDir,compiler.exeName("test"))
      begin
        compiler.linkEXE(exeName,[target,libadd])
        `#{exeName}`
        if $?
          config.add("LDFLAGS",libadd)
        end
      rescue RuntimeError => e
        log e,e.backtrace
        return false
      end
      true
    end


    private
    def getPath
      # add paths in externals/build/*/bin
      getEnvPath+Dir[File.expand_path(File.join(File.split(__FILE__)[0],"externals","built","*"))].map{|d|File.join(d,"bin")}
    end
    def getEnvPath
      p=ENV["PATH"]
      if File.join("a","b")=~/\\/
        p.split(";")
      else
        p.split(":")
      end
    end
    
    def install(package)
      #raise 1
      ruby(File.join(File.split(__FILE__)[0],"externals","tools.rb"),package)
    end    
  end
  
  Dir[File.join(File.split(__FILE__)[0],"configs","*.rb")].each{|file|
    require file
  }
  
  def Rookey.log(*s)
    Rookey.mkdir(Compiler::rookeyWorkingDir)
    @@logFile||=File.open(File.join(Compiler::rookeyWorkingDir,"rookey.log"),"w")
    @@logFile.puts(*s)
  end

  CLEAN << File.join(Compiler::rookeyWorkingDir,"rookey.log")
  CLEAN << ["test.c","test.o","test","test.exe"].map{|f|File.join(Compiler.rookeyWorkingDir,"test",f)}
  CLEAN << File.join(Compiler.rookeyWorkingDir,"test")
 
  def Rookey.getDescendantsOfClass(p)
    c=[]
    ObjectSpace.each_object(Class){|a|c.push(a) if a.ancestors.member?(p)}
    return c
  end
  
  
  def self.checkConfig(pClass)
    
  end
  
  def Rookey.runConfigure
    log caller
    configurators=getDescendantsOfClass(Configurator)
    ok=[]
    run=[]
    config=Config.new
    while run.length<configurators.length
	    configurators.each{|c|
	      unless run.member?(c)
		      if c.needs.select{|s|not ok.member?(s)}.length == 0
		        log "Running configurator #{c}"
            #pp config
            begin
		          c.new.run(config)
		          log config.inspect
              @@configured << c
            rescue Object=>e
              log "Configuration of #{c} failed !"
              log e,e.backtrace
            end
		        ok+=c.provides
		        ok.uniq!
		        run << c
		      end
	      end
	    }
    end
    @@config=config
  end
end