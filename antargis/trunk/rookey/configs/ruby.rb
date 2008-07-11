require 'mkmf'

module Rookey
  if ROOKEY_CONFIG[:use_mkmf]==true or ROOKEY_CONFIG[:use_mkmf].nil? 
	  class RubyConfig<Configurator
	    provides :compiler
	    
	    def run(config)
		      config.add("INCLUDEDIRS",get("archdir")+" "+get("includedir"))
		      config.add("CFLAGS",get("CFLAGS"))
		      config.add("LDFLAGS",get("LIBS"))
		      config.add("LDFLAGS",get("LIBRUBYARG"))
		      config.add("LDSHAREDFLAGS",get("LDSHARED").sub(/^[^ ]+ /,""))
		      config["DLEXT"]=get("DLEXT")
		      config["host_os"]=get("host_os")      
		      config["GPP_BASE"]="g++"
		      config["GCC_BASE"]="gcc"
		
		      {"CPP"=>"GPP_BASE","CC"=>"GCC_BASE"}.each{|name,base|      
		        config[name]=searchProgram(config[base])
		      }
	    end
	    
	  private
	    # recurse through ruby's CONFIG structure 
	    def get(n)
	      return "" if n.nil?
	      v=CONFIG[n]
	      return "" if v.nil?
	      v.gsub(/\$\(([^)]*\))/) {|a|
	        f=a[2..-2]
	        get(f)
	      }      
	    end
	    
	  end
	end
end