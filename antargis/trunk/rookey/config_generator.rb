
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
  
  # Rookeys Config-hash. 
  class Config
    def initialize(hash={})
      @hash=hash
    end
    def [](name)
      @hash[name]||""
    end
    def []=(name,value)
      @hash[name]=value
    end
    def add(name,value)
      @hash[name]||=""
      @hash[name]+=" "+value
    end
  end
  
  CONFIG_FILENAME="config_cache.rb"
  
  @@config=nil
  
  def Rookey.getConfig
    if @@config.nil?
      if File.exists?(Rookey::CONFIG_FILENAME)
        load Rookey::CONFIG_FILENAME
	    else 
	      runConfigure
        hibe(@@config,Rookey::CONFIG_FILENAME)
	    end
      CLEAN << Rookey::CONFIG_FILENAME
    end
    @@config
  end 
  
  # stores a dumpable ruby-object *what* into a file named *filename*
  def Rookey.hibe(what,filename)
    content=Marshal.dump(what).gsub("\"","\\\"")
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
    
    def searchProgram(program)
      searchPrograms(program)[0]
    end
    
    def searchPrograms(program)
      programs=[program].flatten
      result=programs.map{|program|
	      getPath.map{|dir|
	        
	        p=File.join(dir,program)
	        p=nil unless File.exists?(p)
          p
	      }
      }.flatten.uniq-[nil]
      pp result
      result
    end
    def checkProgram(program)      
    end
    def checkLibrary(config,lib,funcname)
      source="extern \"C\" void #{funcname}();int main(){return 0;}"
      testSource="test.c"
      fd=File.open(testSource,"w")
      fd.puts source
      fd.close
      compiler=Compiler.new(config)
      target=compiler.makeObject(testSource)
      compiler.compile(target,testSource)
      libadd="-l#{lib}"
      exeName=compiler.exeName("test")
      compiler.linkEXE(exeName,[target,libadd])
      `#{exeName}`
      if $?
        config.add("LDFLAGS",libadd)
      end
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
  end
  
  Dir[File.join(File.split(__FILE__)[0],"configs","*.rb")].each{|file|
    require file
  }
  
  def Rookey.log(*s)
    puts(*s)
  end
  
 
  def Rookey.getDescendantsOfClass(p)
    c=[]
    ObjectSpace.each_object(Class){|a|c.push(a) if a.ancestors.member?(p)}
    return c
  end
  
  
  def Rookey.runConfigure
    configurators=getDescendantsOfClass(Configurator)
    ok=[]
    run=[]
    config=Config.new
    while run.length<configurators.length
	    configurators.each{|c|
	      unless run.member?(c)
		      if c.needs.select{|s|not ok.member?(s)}.length == 0
		        log "Running configurator #{c}"
		        c.new.run(config)
		        ok+=c.provides
		        ok.uniq!
		        run << c
		      end
	      end
	    }
    end
    puts
    @@config=config
  end
end