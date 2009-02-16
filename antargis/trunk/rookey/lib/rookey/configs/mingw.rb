require 'mkmf'

module Rookey
  class MinGWConfig<Configurator
    provides :compiler
    
    def run(config)
      if ROOKEY_CONFIG[:extconfig]=='mingw32'
	      #config.add("INCLUDEDIRS",get("archdir")+" "+get("includedir"))
	      #config.add("CFLAGS",get("CFLAGS"))
	      #config.add("LDFLAGS",get("LIBS"))
	      #config.add("LDFLAGS",get("LIBRUBYARG"))
	      #config.add("LDSHAREDFLAGS",get("LDSHARED").sub(/^[^ ]+ /,""))
	      config["DLEXT"]="dll"
	      config["host_os"]="win32"      
	      config["GPP_BASE"]="i*mingw*g++"
	      config["GCC_BASE"]="i*mingw*gcc"
	
	      {"CPP"=>"GPP_BASE","CC"=>"GCC_BASE"}.each{|name,base|      
	        config[name]=searchProgram(config[base])
	      }
        base=config["CPP"].gsub(/bin\/[^\/]+/,"")
        baseMingw=Dir[base+"i*mingw*"]
        config.add("LDFLAGS","-L"+File.join(baseMingw,"lib"))
        config.add("LDFLAGS","-L"+installLibDir)
        config.add("INCLUDEDIRS",File.join(baseMingw,"include"))
        config.add("INCLUDEDIRS",installIncludeDir)
        config.add("CFLAGS","-mthreads")        
        
        #pp config
        #exit
      end
    end
  end    
end
