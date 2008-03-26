require 'mkmf'

module Rookey
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